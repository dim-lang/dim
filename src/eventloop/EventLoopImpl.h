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
  uint64_t id_; // fd
  int event_;
  FileHandler *readHandler_;
  FileHandler *writeHandler_;
  void *data_;
};

class TimeoutEvent {
public:
  uint64_t id_; // timeout event id
  int64_t millisec_;
  TimeoutHandler *timeoutHandler_;
  void *data_;
};

class TriggerEvent {
public:
  uint64_t id_;
  int event_;
};

class EventLoopImpl : public EventLoop {
public:
  ///
  /// method
  ///

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

  virtual int fileSize() const;
  virtual int timeoutSize() const;

  // @return system api call name
  virtual std::string api() const = 0;

  ///
  /// member
  ///

  // file event attribute
  uint64_t maxfd_; // highest file event fd
  int fdsize_;     // file event size
  std::unordered_map<uint64_t, FileEvent *> fileEventMap_;
  // timeout event attribute
  int64_t timeoutEventNextId_;
  int64_t lastTime_; /* Used to detect system clock skew */
  std::unordered_map<uint64_t, TimeoutEvent *> timeoutEventMap_;
  // other attribute
  std::vector<TriggerEvent> triggerEventList_;
  bool stop_;
  Poll *poll_; // poll api
};

} // namespace fastype
