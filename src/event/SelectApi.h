// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "event/ApiConfig.h"

#ifdef F_EVENT_HAVE_SELECT

#include "event/Api.h"
#include <cstdint>
#include <sys/select.h>
#include <sys/time.h>

namespace fastype {

class EventLoop;

class SelectApi : public Api {
public:
  SelectApi(EventLoop *evloop);
  virtual ~SelectApi();

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

  EventLoop *evloop_;

  friend class EventLoop;
};

} // namespace fastype

#endif