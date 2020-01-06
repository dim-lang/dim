// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/poll/Kqueue.h"

#ifdef FEVENT_HAVE_KQUEUE

#include "event/EventLoopImpl.h"
#include <cstdlib>
#include <cstring>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define FALIGN_UP 16

Kqueue::Kqueue(EventLoopImpl *evloop)
    : fdset_(nullptr), capacity_(0), evloop_(evloop) {
  kqfd_ = kqueue();
}

Kqueue::~Kqueue() {
  ::close(kqfd_);
  kqfd_ = -1;
  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  capacity_ = 0;
  evloop_ = nullptr;
}

int Kqueue::expand(int size) {
  int newCapacity = std::max<int>(
      FALIGN_UP, (int)boost::alignment::align_up(size, FALIGN_UP));
  struct kevent *newSet =
      (struct kevent *)realloc(fdset_, newCapacity * sizeof(struct kevent));
  if (!newSet) {
    return -1;
  }
  fdset_ = newSet;
  capacity_ = newCapacity;
  return 0;
}

int Kqueue::capacity() const { return capacity_; }

int Kqueue::add(int64_t fd, int event) {
  struct kevent ke;

  if (event & FEVENT_READ) {
    EV_SET(&ke, (int)fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    if (kevent(kqfd_, &ke, 1, nullptr, 0, nullptr) == -1)
      return -1;
  }
  if (event & FEVENT_WRITE) {
    EV_SET(&ke, (int)fd, EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
    if (kevent(kqfd_, &ke, 1, nullptr, 0, nullptr) == -1)
      return -1;
  }
  return 0;
}

int Kqueue::remove(int64_t fd, int event) {
  struct kevent ke;

  if (event & FEVENT_READ) {
    EV_SET(&ke, (int)fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
    kevent(kqfd_, &ke, 1, nullptr, 0, nullptr);
  }
  if (event & FEVENT_WRITE) {
    EV_SET(&ke, (int)fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
    kevent(kqfd_, &ke, 1, nullptr, 0, nullptr);
  }
  return 0;
}

int Kqueue::poll(int millisec) {
  struct timespec *tsp = nullptr;
  struct timespec ts;
  if (millisec >= 0) {
    int64_t ns = ((int64_t)millisec) * 1000LL;
    ts.tv_sec = ns / 1000000000LL;
    ts.tv_nsec = ns % 1000000000LL;
    tsp = &ts;
  }

  int count = 0;
  int n = kevent(kqfd_, nullptr, 0, fdset_, capacity_, tsp);

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int event = FEVENT_NONE;
      struct kevent *ke = &fdset_[i];
      if (ke->filter & EVFILT_READ) {
        event |= FEVENT_READ;
      }
      if (ke->filter & EVFILT_WRITE) {
        event |= FEVENT_WRITE;
      }
      if (event != FEVENT_NONE) {
        evloop_->trigger(ke->ident, event);
        count++;
      }
    }
  }

  return count;
}

std::string Kqueue::name() const { return "kqueue"; }

#endif
