// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "eventloop/poll/Select.h"

#ifdef F_EVENT_HAVE_SELECT

#include "eventloop/EventLoopImpl.h"
#include <cstdlib>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>

namespace fastype {

Select::Select(EventLoopImpl *evloop) : maxfd_(-1), evloop_(evloop) {
  FD_ZERO(&readset_);
}

Select::~Select() {
  FD_ZERO(&readset_);
  evloop_ = nullptr;
}

int Select::expand(int size) { return size >= FD_SETSIZE ? -1 : 0; }

int Select::capacity() const { return FD_SETSIZE; }

int Select::add(int64_t fd) {
  maxfd_ = std::max(maxfd_, fd);
  FD_SET((int)fd, &readset_);
  return 0;
}

int Select::remove(int64_t fd) {
  if (fd == maxfd_) {
    maxfd_--;
  }
  FD_CLR((int)fd, &readset_);
  return 0;
}

int Select::poll(int millisec) {
  std::memcpy(&readset2_, &readset_, sizeof(fd_set));

  struct timeval tv;
  struct timeval *tvp = nullptr;
  if (millisec >= 0) {
    tv.tv_sec = millisec / 1000000L;
    tv.tv_usec = millisec % 1000000L;
    tvp = &tv;
  }

  int count = 0;
  int n = select(maxfd_ + 1, &readset2_, nullptr, nullptr, tvp);

  if (n > 0) {
    for (int i = 0; i <= maxfd_; i++) {
      if (FD_ISSET(i, &readset2_)) {
        evloop_->trigger((int64_t)i);
        count++;
      }
    }
  }

  return count;
}

std::string Select::name() const { return "select"; }

} // namespace fastype

#endif
