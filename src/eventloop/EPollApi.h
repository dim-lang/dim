// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Platform.h"

#ifdef F_PLATFORM_LINUX

#include "eventloop/Api.h"
#include <cstdint>
#include <sys/epoll.h>
#include <sys/time.h>

namespace fastype {

class EventLoop;

class EPollApi : public Api {
public:
  EPollApi(EventLoop *evloop);
  virtual ~EPollApi();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(uint64_t fd, int event);
  virtual int remove(uint64_t fd, int event);
  virtual int poll(int millisec);

private:
  int epollfd_;
  struct epoll_event *fdset_;
  int size_;
  int capacity_;

  EventLoop *evloop_;

  friend class EventLoop;
};

} // namespace fastype

#endif
