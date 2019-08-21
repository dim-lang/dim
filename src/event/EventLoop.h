// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "event/Api.h"
#include "event/ApiConfig.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace fastype {

class EventLoop;

typedef void FileHandler(EventLoop *evloop, uint64_t fd, void *data, int event);
typedef void TimeoutHandler(EventLoop *evloop, uint64_t eventId, void *data);

namespace detail {

struct FileEvent {
  int event;
  FileHandler *readHandler;
  FileHandler *writeHandler;
  void *data;
};

struct TimeoutEvent {
  uint64_t eventId;
  int64_t millisec;
  TimeoutHandler *timeoutHandler;
  void *data;
};

struct TriggerEvent {
  uint64_t fd;
  int event;
};

} // namespace detail

class EventLoop {
public:
  static EventLoop *open();
  static void close(EventLoop *eventLoop);

  int addFileEvent(uint64_t fd, int event, FileHandler readCb,
                   FileHandler writeCb, void *data);
  int removeFileEvent(uint64_t fd, int event);

  // @return timeoutEventId
  int addTimeoutEvent(int64_t millisec, TimeoutHandler timeoutcb, void *data);

  int removeTimeoutEvent(uint64_t timeoutEventId);

  void start();
  void stop();
  void process();
  void wait(int64_t millisec);

  std::string api() const;

  // api
  detail::FileEvent *fileEvent(uint64_t fd);
  detail::TimeoutEvent *timeoutEvent(uint64_t fd);
  void trigger(int index, uint64_t fd, int event);
};

} // namespace fastype
