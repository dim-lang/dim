// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Platform.h"

#ifdef F_PLATFORM_LINUX || F_PLATFORM_UNIX || F_PLATFORM_MACOS

#include "boost/align.hpp"
#include "event/EventLoop.h"
#include "event/PollApi.h"
#include <cstdlib>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>

#define ALIGN_UP 16

namespace fastype {

PollApi::PollApi(EventLoop *evloop)
    : fdset_(nullptr), fdset2_(nullptr), size_(0), capacity_(0),
      evloop_(evloop) {}

PollApi::~PollApi() {
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

int PollApi::expand(int size) {
  if (size <= size_) {
    return 0;
  }

  int newCapacity = std::max<int>(
      ALIGN_UP, (int)boost::alignment::align_up(capacity_, ALIGN_UP));
  struct pollfd *newFdSet =
      (struct pollfd *)malloc(newCapacity * sizeof(struct pollfd));
  struct pollfd *newFdSet2 =
      (struct pollfd *)malloc(newCapacity * sizeof(struct pollfd));

  if (!newFdSet || !newFdSet2) {
    if (newFdSet) {
      free(newFdSet);
    }
    if (newFdSet2) {
      free(newFdSet2);
    }
    return -1;
  }

  std::memcpy(newFdSet, fdset_, capacity_ * sizeof(struct pollfd));

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

int PollApi::capacity() const { return capacity_; }

int PollApi::add(uint64_t fd, int event) {
  int myfd = (int)fd;
  fdset_[size_].fd = myfd;
  if (event & F_EVENT_READ) {
    fdset_[size_].events |= POLLIN;
  }
  if (event & F_EVENT_WRITE) {
    fdset_[size_].events |= POLLOUT;
  }
  size_++;
  return 0;
}

int PollApi::remove(uint64_t fd, int event) {
  int myfd = (int)fd;

  for (int i = 0; i < size_; i++) {
    if (fdset_[i].fd != myfd) {
      continue;
    }
    if (event & F_EVENT_READ) {
      fdset_[i].events &= ~POLLIN;
    }
    if (event & F_EVENT_WRITE) {
      fdset_[i].events &= ~POLLOUT;
    }
    if (!(fdset_[i].events & POLLIN) && !(fdset_[i].events & POLLOUT)) {
      std::swap(fdset_[size_], fdset_[i]);
      size_--;
    }
    break;
  }
  return 0;
}

int PollApi::poll(int millisec) {
  int size2_ = size_;
  std::memcpy(fdset2_, fdset_, capacity_ * sizeof(struct pollfd));

  millisec = std::max(millisec, -1);

  int eventNumber = 0;
  int n = poll(fdset2_, size2_, millisec);

  if (n > 0) {
    for (int i = 0; i < size2_; i++) {
      int mask = 0;
      FileEvent *fe = evloop_->getFileEvent((uint64_t)fdset2_[i].fd);
      if (fe->event == F_EVENT_NONE) {
        continue;
      }
      if (fe->event & F_EVENT_READ && fdset2_[i].revents & POLLIN) {
        mask |= F_EVENT_READ;
      }
      if (fe->event & F_EVENT_WRITE && fdset2_[i].revents & POLLOUT) {
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
