// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/poll/Epoll.h"

#ifdef F_EVENT_HAVE_EPOLL

#include "boost/align/align_up.hpp"
#include "event/EventLoopImpl.h"
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>

#define ALIGN_UP 16

namespace fastype {

Epoll::Epoll(EventLoopImpl *evloop)
    : fdset_(nullptr), capacity_(0), evloop_(evloop) {
  epfd_ = epoll_create(1024);
}

Epoll::~Epoll() {
  ::close(epfd_);
  epfd_ = -1;
  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  capacity_ = 0;
  evloop_ = nullptr;
}

int Epoll::expand(int size) {
  if (size <= capacity_) {
    return 0;
  }

  int newCapacity = std::max<int>(
      ALIGN_UP, (int)boost::alignment::align_up(capacity_, ALIGN_UP));
  struct epoll_event *newFdSet =
      (struct epoll_event *)malloc(newCapacity * sizeof(struct epoll_event));
  std::memset(newFdSet, 0, newCapacity * sizeof(struct epoll_event));

  if (!newFdSet) {
    return -1;
  }

  std::memcpy(newFdSet, fdset_, capacity_ * sizeof(struct epoll_event));

  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }

  fdset_ = newFdSet;
  capacity_ = newCapacity;
  return 0;
}

int Epoll::capacity() const { return capacity_; }

int Epoll::add(int64_t fd, int event) {
  int op = EPOLL_CTL_ADD;

  if (evloop_->containsReader(fd)) {
    event |= F_EVENT_READ;
    op = EPOLL_CTL_MOD;
  }
  if (evloop_->containsWriter(fd)) {
    event |= F_EVENT_WRITE;
    op = EPOLL_CTL_MOD;
  }

  struct epoll_event ee = {0};
  ee.events = 0;
  ee.data.fd = (int)fd;

  if (event & F_EVENT_READ) {
    ee.events |= EPOLLIN;
  }
  if (event & F_EVENT_WRITE) {
    ee.events |= EPOLLOUT;
  }

  return epoll_ctl(epfd_, op, (int)fd, &ee) == -1 ? -1 : 0;
}

int Epoll::remove(int64_t fd, int event) {
  int op = EPOLL_CTL_MOD;

  struct epoll_event ee = {0};
  ee.events = EPOLLIN;
  ee.data.fd = (int)fd;
  return epoll_ctl(epfd_, EPOLL_CTL_DEL, (int)fd, &ee) == -1 ? -1 : 0;
}

int Epoll::poll(int millisec) {
  if (millisec < 0) {
    millisec = -1;
  }

  int count = 0;
  int n = epoll_wait(epfd_, fdset_, capacity_, millisec);

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      struct epoll_event *ee = &fdset_[i];
      if (ee->events & EPOLLIN) {
        evloop_->trigger(ee->data.fd);
        count++;
      }
    }
  }

  return count;
}

std::string Epoll::name() const { return "epoll"; }

} // namespace fastype

#endif
