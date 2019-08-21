// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "eventloop/EventLoop.h"
#include "eventloop/Poll.h"
#include <unordered_map>
#include <vector>

namespace fastype {

class FileEvent {
public:
  uint64_t id; // fd
  int event;
  FileHandler *readHandler;
  FileHandler *writeHandler;
  void *data;
};

class TimeoutEvent {
public:
  uint64_t id; // timeout event id
  int64_t millisec;
  TimeoutHandler *timeoutHandler;
  void *data;
};

class TriggerEvent {
public:
  uint64_t id;
  int event;
};

class EventLoopImpl : public EventLoop {
public:
  EventLoopImpl();

  virtual ~EventLoopImpl();

  // @return fd
  virtual int addFileEvent(uint64_t fd, int event, FileHandler readCb,
                           FileHandler writeCb, void *data) = 0;
  virtual int removeFileEvent(uint64_t fd, int event) = 0;

  // @return timeout event id
  virtual int addTimeoutEvent(int64_t millisec, TimeoutHandler timeoutcb,
                              void *data) = 0;

  // @id timeout event id
  virtual int removeTimeoutEvent(uint64_t id) = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  // @return processed event count
  virtual int process() = 0;

  virtual void wait(int64_t millisec) = 0;

  // @return system api call name
  virtual std::string api() const = 0;

  /// member

  uint64_t maxfd_; // highest file event fd

  int size_; // file event size

  int64_t timeoutEventNextId_;

  int64_t lastTime_; /* Used to detect system clock skew */

  std::unordered_map<uint64_t, FileEvent *> fileEventMap_;

  std::unordered_map<uint64_t, TimeoutEvent *> timeoutEventMap_;

  std::vector<TriggerEvent> triggerEventList_;

  bool stop_;

  Poll *poll_; // system api call
};

} // namespace fastype
