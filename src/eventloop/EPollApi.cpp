// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Platform.h"

#ifdef F_PLATFORM_LINUX

#include "eventloop/EPollApi.h"
#include "eventloop/EventLoop.h"
#include <cstdlib>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

namespace fastype {

EPollApi::EPollApi(EventLoop *evloop)
    : fdset_(nullptr), fdset2_(nullptr), size_(0), capacity_(0),
      evloop_(evloop) {
  epollfd_ = epoll_create(32000);
}

EPollApi::~EPollApi() {
  close(epollfd_);
  epollfd_ = -1;
  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  if (fdset2_) {
    free(fdset2_);
    fdset2_ = nullptr;
  }
  size_ = 0;
  capacity_ = 0;
  evloop_ = nullptr;
}

int EPollApi::expand(int size) {
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

int EPollApi::capacity() const { return capacity_; }

int EPollApi::add(uint64_t fd, int event) {
  int myfd = (int)fd;
  if (event & F_EVENT_READ) {
    FD_SET(myfd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_SET(myfd, &writeset_);
  }
  return 0;
}

int EPollApi::remove(uint64_t fd, int event) {
  int myfd = (int)fd;
  if (event & F_EVENT_READ) {
    FD_CLR(myfd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_CLR(myfd, &writeset_);
  }
  return 0;
}

int EPollApi::poll(int millisec) {
  std::memcpy(&readset2_, &readset_, sizeof(fd_set));
  std::memcpy(&writeset2_, &writeset_, sizeof(fd_set));

  struct timeval tv;
  struct timeval *tvp = nullptr;
  if (millisec >= 0) {
    tv.tv_sec = millisec / 1000000L;
    tv.tv_usec = millisec % 1000000L;
    tvp = &tv;
  }

  int eventNumber = 0;
  int n = select(evloop_->maxfd_ + 1, &readset2_, &writeset2_, nullptr, tvp);

  if (n > 0) {
    for (int i = 0; i <= evloop_->maxfd_; i++) {
      int mask = 0;
      FileEvent *fe = evloop_->getFileEvent((uint64_t)i);
      if (fe->event == F_EVENT_NONE) {
        continue;
      }
      if (fe->event & F_EVENT_READ && FD_ISSET(i, &readset2_)) {
        mask |= F_EVENT_READ;
      }
      if (fe->event & F_EVENT_WRITE && FD_ISSET(i, &writeset2_)) {
        mask |= F_EVENT_WRITE;
      }

      evloop_->trigger(eventNumber, i, mask);
      eventNumber++;
    }
  }

  return eventNumber;
}

} // namespace fastype

#endif
