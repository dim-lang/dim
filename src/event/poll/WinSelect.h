// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "event/EventConfig.h"

#ifdef F_EVENT_HAVE_WINSELECT

#include "event/Poll.h"
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
  virtual int add(int64_t fd);
  virtual int remove(int64_t fd);
  virtual int poll(int millisec);
  virtual std::string name() const;

private:
  fd_set readset_;
  fd_set readset2_;

  EventLoopImpl *evloop_;

  friend class EventLoopImpl;
};

} // namespace fastype

#endif
