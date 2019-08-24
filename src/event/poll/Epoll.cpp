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

#define F_ALIGN_UP 16

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
  int newCapacity = std::max<int>(
      F_ALIGN_UP, (int)boost::alignment::align_up(size, F_ALIGN_UP));
  struct epoll_event *newSet = (struct epoll_event *)realloc(
      fdset_, newCapacity * sizeof(struct epoll_event));
  if (!newSet) {
    return -1;
  }
  fdset_ = newSet;
  capacity_ = newCapacity;
  return 0;
}

int Epoll::capacity() const { return capacity_; }

int Epoll::add(int64_t fd, int event) {
  int op = EPOLL_CTL_ADD;

  if (evloop_->hasReader(fd)) {
    event |= F_EVENT_READ;
    op = EPOLL_CTL_MOD;
  }
  if (evloop_->hasWriter(fd)) {
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
  int ev = F_EVENT_NONE;
  if (evloop_->hasReader(fd)) {
    ev |= F_EVENT_READ;
  }
  if (evloop_->hasWriter(fd)) {
    ev |= F_EVENT_WRITE;
  }
  event = ev & (~event);

  struct epoll_event ee = {0};
  ee.data.fd = (int)fd;
  if (event & F_EVENT_READ) {
    ee.events |= EPOLLIN;
  }
  if (event & F_EVENT_WRITE) {
    ee.events |= EPOLLOUT;
  }
  int op = event == F_EVENT_NONE ? EPOLL_CTL_DEL : EPOLL_CTL_MOD;
  return epoll_ctl(epfd_, op, (int)fd, &ee) == -1 ? -1 : 0;
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
