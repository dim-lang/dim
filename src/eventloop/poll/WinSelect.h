// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "eventloop/EventConfig.h"

#ifdef F_EVENT_HAVE_WINSELECT

#include "eventloop/Poll.h"
#include <WinSock2.h>
#include <cstdint>

namespace fastype {

class EventLoopImpl;

class WinSelect : public Poll {
public:
  WinSelect(EventLoopImpl *evloop);
  virtual ~WinSelect();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(uint64_t fd, int event);
  virtual int remove(uint64_t fd, int event);
  virtual int poll(int millisec);
  virtual std::string name() const;

private:
  fd_set readset_;
  fd_set writeset_;
  fd_set readset2_;
  fd_set writeset2_;

  EventLoopImpl *evloop_;

  friend class EventLoopImpl;
};

} // namespace fastype

#endif