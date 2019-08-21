// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>
#include <string>

namespace fastype {

class EventLoop;

typedef void FileHandler(EventLoop *evloop, int64_t fd, void *data, int event);
typedef void TimeoutHandler(EventLoop *evloop, int64_t eventId, void *data);

class EventLoop {
public:
  virtual ~EventLoop() = default;

  static EventLoop *open();
  static void close(EventLoop *eventLoop);

  // @return fd
  virtual int addFileEvent(int64_t fd, int event, FileHandler readCb,
                           FileHandler writeCb, void *data) = 0;
  virtual int removeFileEvent(int64_t fd, int event) = 0;

  // @return timeout event id
  virtual int addTimeoutEvent(int64_t millisec, TimeoutHandler timeoutcb,
                              void *data) = 0;

  // @id timeout event id
  virtual int removeTimeoutEvent(int64_t id) = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  // @return processed event count
  virtual int process() = 0;

  virtual void wait(int64_t millisec) = 0;

  // @return system api call name
  virtual std::string api() const = 0;
};

} // namespace fastype
