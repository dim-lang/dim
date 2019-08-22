// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "eventloop/EventLoop.h"
#include "eventloop/Poll.h"
#include <queue>
#include <unordered_map>
#include <vector>

namespace fastype {

class FileEvent {
public:
  int64_t id_; // fd
  FileHandler *handler_;
  void *data_;
};

class TimeoutEvent {
public:
  int64_t id_;        // timeout event id
  int64_t timestamp_; // system timestamp
  int64_t millisec_;
  TimeoutHandler *handler_;
  void *data_;
  int repeat_;
};

class TimeoutEventComparator {
public:
  // less at top
  int operator()(const TimeoutEvent &a, const TimeoutEvent &b) {
    return a.timestamp_ < b.timestamp_;
  }
};

class EventLoopImpl : public EventLoop {
public:
  ///
  /// api
  ///

  EventLoopImpl();

  virtual ~EventLoopImpl();

  virtual int addFile(int64_t fd, FileHandler handler, void *data);
  virtual int removeFile(int64_t fd);
  virtual int addTimeout(int64_t millisec, TimeoutHandler handler, void *data);
  virtual int addTimeout(int64_t millisec, TimeoutHandler handler, void *data,
                         int repeat);
  virtual int removeTimeout(int64_t id);
  virtual void start();
  virtual void stop();
  virtual int process();
  virtual int fileSize() const;
  virtual int timeoutSize() const;
  virtual std::string api() const;
  virtual int64_t cachedTime() const;

public:
  ///
  /// inner poll api
  ///

  // trigger file event
  virtual void trigger(int64_t id);

  // update time
  virtual void updateTime();

public:
  ///
  /// member
  ///

  // file event attribute
  std::unordered_map<int64_t, FileEvent *> fileMap_;

  // timeout event attribute
  int64_t timestamp_;
  std::unordered_map<int64_t, TimeoutEvent *> timeoutMap_;
  std::priority_queue<TimeoutEvent *, std::vector<TimeoutEvent *>,
                      TimeoutEventComparator>
      timeoutQueue_;

  // other attribute
  std::vector<int64_t> triggerList_;
  bool stop_;
  Poll *poll_;
};

} // namespace fastype
