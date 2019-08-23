// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Event.h"

#ifdef F_EVENT_HAVE_SELECT

#include "event/Poll.h"
#include <cstdint>
#include <sys/select.h>
#include <sys/time.h>

namespace fastype {

class EventLoopImpl;

class Select : public Poll {
public:
  Select(EventLoopImpl *evloop);
  virtual ~Select();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(int64_t fd, int event);
  virtual int remove(int64_t fd, int event);
  virtual int poll(int millisec);
  virtual std::string name() const;

private:
  fd_set readset_;
  fd_set readset2_;
  fd_set writeset_;
  fd_set writeset2_;
  int64_t maxfd_;

  EventLoopImpl *evloop_;
};

} // namespace fastype

#endif
