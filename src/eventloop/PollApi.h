// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Platform.h"

#ifdef F_PLATFORM_LINUX || F_PLATFORM_UNIX || F_PLATFORM_MACOS

#include "eventloop/Api.h"
#include <cstdint>
#include <poll.h>

namespace fastype {

class EventLoop;

class PollApi : public Api {
public:
  PollApi(EventLoop *evloop);
  virtual ~PollApi();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(uint64_t fd, int event);
  virtual int remove(uint64_t fd, int event);
  virtual int poll(int64_t millisec);

private:
  struct pollfd *fdset_;
  struct pollfd *fdset2_;
  int size_;
  int capacity_;

  EventLoop *evloop_;

  friend class EventLoop;
};

} // namespace fastype

#endif
