// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/ApiConfig.h"

#ifdef F_EVENT_HAVE_KQUEUE

#include "event/EventLoop.h"
#include "event/KQueueApi.h"
#include <cstdlib>
#include <cstring>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

namespace fastype {

KQueueApi::KQueueApi(EventLoop *evloop)
    : fdset_(nullptr), size_(0), capacity_(0), evloop_(evloop) {
  kqfd_ = kqueue();
}

KQueueApi::~KQueueApi() {
  close(kqfd_);
  kqfd_ = -1;
  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  size_ = 0;
  capacity_ = 0;
  evloop_ = nullptr;
}

int KQueueApi::expand(int size) {
  if (size <= size_) {
    return 0;
  }

  int newCapacity = std::max<int>(
      ALIGN_UP, (int)boost::alignment::align_up(capacity_, ALIGN_UP));
  struct epoll_event *newFdSet =
      (struct epoll_event *)malloc(newCapacity * sizeof(struct epoll_event));
  struct epoll_event *newFdSet2 =
      (struct epoll_event *)malloc(newCapacity * sizeof(struct epoll_event));

  if (!newFdSet || !newFdSet2) {
    if (newFdSet) {
      free(newFdSet);
    }
    if (newFdSet2) {
      free(newFdSet2);
    }
    return -1;
  }

  std::memcpy(newFdSet, fdset_, capacity_ * sizeof(struct epoll_event));

  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  if (fdset2_) {
    free(fdset2_);
    fdset2_ = nullptr;
  }

  fdset_ = newFdSet;
  fdset2_ = newFdSet2;
  capacity_ = newCapacity;
  return 0;
}

int KQueueApi::capacity() const { return 32000; }

int KQueueApi::add(uint64_t fd, int event) {
  if (event & F_EVENT_READ) {
    FD_SET(myfd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_SET(myfd, &writeset_);
  }
  return 0;
}

int KQueueApi::remove(uint64_t fd, int event) {
  if (event & F_EVENT_READ) {
    FD_CLR(myfd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_CLR(myfd, &writeset_);
  }
  return 0;
}

int KQueueApi::poll(int millisec) {
  struct timeval *tvp = nullptr;
  struct timeval tv;
  if (millisec >= 0) {
    tv.tv_sec = millisec / 1000000L;
    tv.tv_usec = millisec % 1000000L;
    tvp = &tv;
  }

  int index = 0;
  int n = kevent(kqfd_, nullptr, 0, fdset_, capacity_, tvp);

  if (n > 0) {
    for (int i = 0; i <= evloop_->maxfd_; i++) {
      int mask = 0;
      struct kevent *ke = &fdset_[i];

      if (ke->filter & EVFILT_READ) {
        mask |= F_EVENT_READ;
      }
      if (ke->events & EVFILT_WRITE) {
        mask |= F_EVENT_WRITE;
      }
      evloop_->trigger(index, i, mask);
      index++;
    }
  }

  return index;
}

std::string KQueueApi::name() const { return "kqueue"; }

} // namespace fastype

#endif
