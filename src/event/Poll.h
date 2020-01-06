// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <cstdint>
#include <string>

#define FEVENT_NONE 0
#define FEVENT_READ 1
#define FEVENT_WRITE 2

class EventLoopImpl;

class Poll {
public:
  virtual ~Poll() = default;

  static Poll *open(EventLoopImpl *evloop);

  static void close(Poll *p);

  // @return 0 if success, -1 if fail
  virtual int expand(int size) = 0;

  // @return fd size
  virtual int capacity() const = 0;

  // add event, it's enabled after been added
  //
  // @param fd  file event fd
  // @return    0 if remove success, -1 if remove fail
  virtual int add(int64_t fd, int event) = 0;

  // remove event
  //
  // @param fd  file event fd
  // @return    0 if remove success, -1 if remove fail
  virtual int remove(int64_t fd, int event) = 0;

  // poll event
  //
  // @param millisec  poll timeout, -1 is forever
  // @return          trigger file event size
  virtual int poll(int millisec) = 0;

  // @return  api name
  virtual std::string name() const = 0;
};
