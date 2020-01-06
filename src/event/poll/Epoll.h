// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Event.h"

#ifdef FEVENT_HAVE_EPOLL

#include "event/Poll.h"
#include <cstdint>
#include <sys/epoll.h>
#include <sys/time.h>

class EventLoopImpl;

class Epoll : public Poll {
public:
  Epoll(EventLoopImpl *evloop);
  virtual ~Epoll();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(int64_t fd, int event);
  virtual int remove(int64_t fd, int event);
  virtual int poll(int millisec);
  virtual std::string name() const;

private:
  int epfd_;
  struct epoll_event *fdset_;
  int capacity_;

  EventLoopImpl *evloop_;

  friend class EventLoopImpl;
};

#endif
