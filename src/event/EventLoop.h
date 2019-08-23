// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>
#include <string>

namespace fastype {

class EventLoop;

typedef void (*DataHandler)(void *data);
typedef void (*FileHandler)(EventLoop *evloop, int64_t fd, void *data);
typedef void (*TimeoutHandler)(EventLoop *evloop, int64_t id, void *data);

class EventLoop {
public:
  virtual ~EventLoop() = default;

  // @return  EventLoop instance pointer, nullptr if fail
  static EventLoop *open();

  // @param  EventLoop instance pointer, nullptr will do nothing
  static void close(EventLoop *eventLoop);

  // @return  read file event fd
  virtual int addReader(int64_t fd, FileHandler handler, void *data,
                        DataHandler datafree) = 0;

  // @return  0 if remove success, -1 if remove fail
  virtual int removeReader(int64_t fd) = 0;

  // @return  write file event fd
  virtual int addWriter(int64_t fd, FileHandler handler, void *data,
                        DataHandler datafree) = 0;

  // @return  0 if remove success, -1 if remove fail
  virtual int removeWriter(int64_t fd) = 0;

  // timeout event will be invoked only once
  //
  // @return  timeout event id
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data,
                       DataHandler datafree) = 0;

  // timer event will be invoked multiple times
  //
  // @param repeat  repeat times, 0 is ignored, -1 is forever
  // @return        timer event id
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data,
                       DataHandler datafree, int repeat) = 0;

  // @param id  timer event id
  // @return    0 if remove success, -1 if remove fail
  virtual int removeTimer(int64_t id) = 0;

  // notify EventLoop start
  virtual void start() = 0;

  // notify EventLoop stop
  virtual void stop() = 0;

  // @return  processed event count, -1 if process fail
  virtual int process() = 0;

  // @return  read file event size
  virtual int readerSize() const = 0;

  // @return  write file event size
  virtual int writerSize() const = 0;

  // @return  timeout event size
  virtual int timerSize() const = 0;

  // @return  system api call name
  virtual std::string api() const = 0;

  // @return current cached time in milliseconds
  virtual int64_t cachedTime() const = 0;
};

} // namespace fastype
