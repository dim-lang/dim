// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>
#include <string>

namespace fastype {

class EventLoop;

typedef void FileHandler(EventLoop *evloop, int64_t fd, void *data);
typedef void TimeoutHandler(EventLoop *evloop, int64_t id, void *data);

class EventLoop {
public:
  virtual ~EventLoop() = default;

  // @return  EventLoop instance pointer, nullptr if fail
  static EventLoop *open();

  // @param  EventLoop instance pointer, nullptr will do nothing
  static void close(EventLoop *eventLoop);

  // @return  file event fd
  virtual int addFile(int64_t fd, FileHandler handler, void *data) = 0;

  // @return  0 if remove success, -1 if remove fail
  virtual int removeFile(int64_t fd) = 0;

  // timeout event will be invoked only once
  //
  // @return  timeout event id
  virtual int addTimeout(int64_t millisec, TimeoutHandler handler,
                         void *data) = 0;

  // timeout event will be invoked multiple times
  //
  // @param repeat  invoked times, 0 is ignored, -1 is forever
  // @return        timeout event id
  virtual int addTimeout(int64_t millisec, TimeoutHandler handler, void *data,
                         int repeat) = 0;

  // @param id  timeout event id
  // @return    0 if remove success, -1 if remove fail
  virtual int removeTimeout(int64_t id) = 0;

  // notify EventLoop start
  virtual void start() = 0;

  // notify EventLoop stop
  virtual void stop() = 0;

  // @return  processed event count, -1 if process fail
  virtual int process() = 0;

  // @return  0 if wait success, -1 if wait fail
  virtual int wait(int64_t millisec) = 0;

  // @return  file event size
  virtual int fileSize() const = 0;

  // @return  timeout event size
  virtual int timeoutSize() const = 0;

  // @return  system api call name
  virtual std::string api() const = 0;

  // @return current cached time in milliseconds
  virtual int64_t cachedTime() const;
};

} // namespace fastype
