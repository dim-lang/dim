// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/ApiConfig.h"

#ifdef F_EVENT_HAVE_KQUEUE

#include "boost/align/align_up.hpp"
#include "event/EventLoop.h"
#include "event/KQueueApi.h"
#include <cstdlib>
#include <cstring>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define ALIGN_UP 16

namespace fastype {

KQueueApi::KQueueApi(EventLoop *evloop)
    : fdset_(nullptr), capacity_(0), evloop_(evloop) {
  kqfd_ = kqueue();
}

KQueueApi::~KQueueApi() {
  close(kqfd_);
  kqfd_ = -1;
  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  capacity_ = 0;
  evloop_ = nullptr;
}

int KQueueApi::expand(int size) {
  if (size <= capacity_) {
    return 0;
  }

  int newCapacity = std::max<int>(
      ALIGN_UP, (int)boost::alignment::align_up(capacity_, ALIGN_UP));
  struct kevent *newFdSet =
      (struct kevent *)malloc(newCapacity * sizeof(struct kevent));

  if (!newFdSet) {
    return -1;
  }

  std::memcpy(newFdSet, fdset_, capacity_ * sizeof(struct kevent));

  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }

  fdset_ = newFdSet;
  capacity_ = newCapacity;
  return 0;
}

int KQueueApi::capacity() const { return 32000; }

int KQueueApi::add(uint64_t fd, int event) {
  struct kevent ke = {0};
  if (event & F_EVENT_READ) {
    EV_SET(&ke, (int)fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    if (kevent(kqfd_, &ke, 1, nullptr, 0, nullptr) == -1)
      return -1;
  }
  if (event & F_EVENT_WRITE) {
    EV_SET(&ke, (int)fd, EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
    if (kevent(kqfd_, &ke, 1, nullptr, 0, nullptr) == -1)
      return -1;
  }
  return 0;
}

int KQueueApi::remove(uint64_t fd, int event) {
  struct kevent ke = {0};

  if (event & F_EVENT_READ) {
    EV_SET(&ke, (int)fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
    kevent(kqfd_, &ke, 1, nullptr, 0, nullptr);
  }
  if (event & F_EVENT_WRITE) {
    EV_SET(&ke, (int)fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
    kevent(kqfd_, &ke, 1, nullptr, 0, nullptr);
  }
  return 0;
}

int KQueueApi::poll(int millisec) {
  struct timespec *tsp = nullptr;
  struct timespec ts;
  if (millisec >= 0) {
    ts.tv = millisec / 1000000L;
    ts.tv_usec = millisec % 1000000L;
    tvp = &tv;
  }

  int count = 0;
  int n = kevent(kqfd_, nullptr, 0, fdset_, capacity_, tvp);

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int mask = 0;
      struct kevent *ke = &fdset_[i];

      if (ke->filter & EVFILT_READ) {
        mask |= F_EVENT_READ;
      }
      if (ke->filter & EVFILT_WRITE) {
        mask |= F_EVENT_WRITE;
      }
      evloop_->triggerEventList_[count].fd = ke->ident;
      evloop_->triggerEventList_[count].event = mask;
      count++;
    }
  }

  return count;
}

std::string KQueueApi::name() const { return "kqueue"; }

} // namespace fastype

#endif
