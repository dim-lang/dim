// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Platform.h"

#ifdef F_PLATFORM_LINUX || F_PLATFORM_UNIX || F_PLATFORM_MACOS

#include "eventloop/EventLoop.h"
#include "eventloop/SelectApi.h"
#include <cstdlib>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>

namespace fastype {

SelectApi::SelectApi(EventLoop *evloop) : evloop_(evloop) {
  FD_ZERO(&readset_);
  FD_ZERO(&writeset_);
}

SelectApi::~SelectApi() {
  FD_ZERO(&readset_);
  FD_ZERO(&writeset_);
  evloop_ = nullptr;
}

int SelectApi::expand(int size) { return size >= FD_SETSIZE ? -1 : 0; }

int SelectApi::capacity() const { return FD_SETSIZE; }

int SelectApi::add(uint64_t fd, int event) {
  int myfd = (int)fd;
  if (event & F_EVENT_READ) {
    FD_SET(myfd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_SET(myfd, &writeset_);
  }
  return 0;
}

int SelectApi::remove(uint64_t fd, int event) {
  int myfd = (int)fd;
  if (event & F_EVENT_READ) {
    FD_CLR(myfd, &readset_);
  }
  if (event & F_EVENT_WRITE) {
    FD_CLR(myfd, &writeset_);
  }
  return 0;
}

int SelectApi::poll(int millisec) {
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
