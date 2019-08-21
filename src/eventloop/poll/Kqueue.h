// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "eventloop/EventConfig.h"

#ifdef F_EVENT_HAVE_KQUEUE

#include "eventloop/Poll.h"
#include <cstdint>
#include <poll.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

namespace fastype {

class EventLoopImpl;

class Kqueue : public Poll {
public:
  Kqueue(EventLoopImpl *evloop);
  virtual ~Kqueue();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(int64_t fd, int event);
  virtual int remove(int64_t fd, int event);
  virtual int poll(int millisec);
  virtual std::string name() const;

private:
  int kqfd_;
  struct kevent *fdset_;
  int size_;
  int capacity_;

  EventLoopImpl *evloop_;

  friend class EventLoopImpl;
};

} // namespace fastype

#endif
