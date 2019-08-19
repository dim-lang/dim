// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/ApiConfig.h"

#ifdef F_EVENT_HAVE_EPOLL

#include "event/EPollApi.h"
#include "event/EventLoop.h"
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>

namespace fastype {

EPollApi::EPollApi(EventLoop *evloop)
    : fdset_(nullptr), size_(0), capacity_(0), evloop_(evloop) {
  epfd_ = epoll_create1(EPOLL_CLOEXEC);
  std::memset(&ev_, 0, sizeof(struct epoll_event));
}

EPollApi::~EPollApi() {
  close(epfd_);
  epfd_ = -1;
  if (fdset_) {
    free(fdset_);
    fdset_ = nullptr;
  }
  size_ = 0;
  capacity_ = 0;
  std::memset(&ev_, 0, sizeof(struct epoll_event));
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
  std::memset(newFdSet, 0, newCapacity * sizeof(struct epoll_event));

  if (!newFdSet) {
    if (newFdSet) {
      free(newFdSet);
    }
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

int EPollApi::capacity() const { return capacity_; }

int EPollApi::add(uint64_t fd, int event) {
  struct epoll_event ee = {0};

  ee.events = 0;
  event |= fdset_[fd].mask;
  if (event & F_EVENT_READ) {
    ee.events |= EPOLLIN;
  }
  if (event & F_EVENT_WRITE) {
    ee.events |= EPOLLOUT;
  }
  ee.data.fd = (int)fd;

  int op = fdset_[fd].mask == F_EVENT_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

  return epoll_ctl(epfd_, op, (int)fd, &ee) == -1 : -1 : 0;
}

int EPollApi::remove(uint64_t fd, int event) {
  struct epoll_event ee = {0};

  ee.events = 0;
  if (event & F_EVENT_READ) {
    ee.events |= EPOLLIN;
  }
  if (event & F_EVENT_WRITE) {
    ee.events |= EPOLLOUT;
  }
  ee.data.fd = (int)fd;

  int op = (fdset_[fd].mask & (~event)) == F_EVENT_NONE ? EPOLL_CTL_DEL
                                                        : EPOLL_CTL_MOD;

  return epoll_ctl(epfd_, op, (int)fd, &ee) == -1 ? -1 : 0;
}

int EPollApi::poll(int millisec) {
  if (millisec < 0) {
    millisec = -1;
  }

  int eventNumber = 0;
  int n = epoll_wait(epfd_, fdset_, capacity_, millisec);

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int mask = 0;
      FileEvent *fe = evloop_->getFileEvent((uint64_t)fdset_[i].fd);
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

std::string EPollApi::name() const { return "epoll"; }

} // namespace fastype

#endif
