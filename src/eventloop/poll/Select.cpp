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

Select::Select(EventLoopImpl *evloop) : evloop_(evloop) {
  FD_ZERO(&readset_);
  FD_ZERO(&writeset_);
}

Select::~Select() {
  FD_ZERO(&readset_);
  FD_ZERO(&writeset_);
  evloop_ = nullptr;
}

int Select::expand(int size) { return size >= FD_SETSIZE ? -1 : 0; }

int Select::capacity() const { return FD_SETSIZE; }

int Select::add(int64_t fd, int event) {
  if (event & F_EVENT_READ) {
    FD_SET((int)fd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_SET((int)fd, &writeset_);
  }
  return 0;
}

int Select::remove(int64_t fd, int event) {
  if (event & F_EVENT_READ) {
    FD_CLR((int)fd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_CLR((int)fd, &writeset_);
  }
  return 0;
}

int Select::poll(int millisec) {
  std::memcpy(&readset2_, &readset_, sizeof(fd_set));
  std::memcpy(&writeset2_, &writeset_, sizeof(fd_set));

  struct timeval tv;
  struct timeval *tvp = nullptr;
  if (millisec >= 0) {
    tv.tv_sec = millisec / 1000000L;
    tv.tv_usec = millisec % 1000000L;
    tvp = &tv;
  }

  int count = 0;
  int n = select(evloop_->maxfd_ + 1, &readset2_, &writeset2_, nullptr, tvp);

  if (n > 0) {
    for (int i = 0; i <= evloop_->maxfd_; i++) {
      int event = 0;
      FileEvent *fe = evloop_->fileEventMap_[(int64_t)i];
      if (fe->event == F_EVENT_NONE) {
        continue;
      }
      if (fe->event & F_EVENT_READ && FD_ISSET(i, &readset2_)) {
        event |= F_EVENT_READ;
      }
      if (fe->event & F_EVENT_WRITE && FD_ISSET(i, &writeset2_)) {
        event |= F_EVENT_WRITE;
      }
      evloop_->triggerEventList_[count].id = (int64_t)i;
      evloop_->triggerEventList_[count].event = event;
      count++;
    }
  }

  return count;
}

std::string Select::name() const { return "select"; }

} // namespace fastype

#endif
