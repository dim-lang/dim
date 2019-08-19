// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>
#include <string>

#define F_EVENT_NONE 0
#define F_EVENT_READ 1
#define F_EVENT_WRITE 2

namespace fastype {

class EventLoop;

class Api {
public:
  static Api *open(EventLoop *evloop);
  void close(Api *api);

  // @return 0 if success, -1 if fail
  virtual int expand(int size) = 0;

  // @return fd size
  virtual int capacity() const = 0;

  // add event
  virtual int add(uint64_t fd, int event) = 0;

  // remove event
  virtual int remove(uint64_t fd, int event) = 0;

  // poll event
  virtual int poll(int millisec) = 0;

  // api name
  virtual std::string name() const = 0;
};

} // namespace fastype
