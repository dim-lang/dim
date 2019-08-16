// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>

#ifndef F_EVENT_NONE
#define F_EVENT_NONE 0
#endif
#ifndef F_EVENT_READ
#define F_EVENT_READ 1
#endif
#ifndef F_EVENT_WRITE
#define F_EVENT_WRITE 2
#endif

namespace fastype {

class EventLoop;

class Api {
public:
  // @return Api* if success, nullptr if fail
  static Api *open(EventLoop *evloop);
  static void close(Api *api);

  // @return 0 if success, -1 if fail
  virtual int expand(int size) = 0;

  // @return fd size
  virtual int capacity() const = 0;

  virtual int add(uint64_t fd, int event) = 0;
  virtual int remove(uint64_t fd, int event) = 0;
  virtual int poll(int millisec) = 0;
};

} // namespace fastype
