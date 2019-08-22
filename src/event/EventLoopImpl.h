// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "event/EventLoop.h"
#include "event/Poll.h"
#include <list>
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

enum TriggerEventType { TG_READ = 1, TG_WRITE = 2 };

class TriggerEvent {
public:
  int64_t id_;
  enum TriggerEventType type_;
};

class EventLoopImpl : public EventLoop {
public:
  /* public api */

  EventLoopImpl();

  virtual ~EventLoopImpl();

  virtual int addReader(int64_t fd, FileHandler handler, void *data);
  virtual int removeReader(int64_t fd);
  virtual int addWriter(int64_t fd, FileHandler handler, void *data);
  virtual int removeWriter(int64_t fd);
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data);
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data,
                       int repeat);
  virtual int removeTimeout(int64_t id);
  virtual void start();
  virtual void stop();
  virtual int process();
  virtual int readerSize() const;
  virtual int writerSize() const;
  virtual int timerSize() const;
  virtual std::string api() const;
  virtual int64_t cachedTime() const;

public:
  /* inner api */

  // trigger file event
  virtual void trigger(int64_t id, enum TriggerEventType type);

  // contains file event
  virtual bool containsReader(int64_t fd) const;
  virtual bool containsWriter(int64_t fd) const;

public:
  /* member */

  // file event
  std::unordered_map<int64_t, FileEvent *> readerMap_;
  std::unordered_map<int64_t, std::list<FileEvent *> *> writerMap_;

  // timeout event
  int64_t timestamp_;
  std::unordered_map<int64_t, TimeoutEvent *> timerMap_;
  std::priority_queue<TimeoutEvent *, std::vector<TimeoutEvent *>,
                      TimeoutEventComparator>
      timerQueue_;

  // trigger event
  std::vector<TriggerEvent> triggerList_;

  bool stop_;
  Poll *poll_;
};

} // namespace fastype
