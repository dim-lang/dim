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
  virtual ~FileEvent() {
    if (resourceHandler_) {
      resourceHandler_(data_);
    }
    resourceHandler_ = nullptr;
    data_ = nullptr;
  }

  int64_t id_; // fd
  FileHandler handler_;
  ResourceHandler resourceHandler_;
  void *data_;
};

class TimeoutEvent {
public:
  virtual ~TimeoutEvent() {
    if (resourceHandler_) {
      resourceHandler_(data_);
    }
    resourceHandler_ = nullptr;
    data_ = nullptr;
  }

  int64_t id_;        // timeout event id
  int64_t timestamp_; // system timestamp
  int64_t millisec_;
  TimeoutHandler handler_;
  ResourceHandler resourceHandler_;
  void *data_;
  int repeat_;
};

class TimeoutEventComparator {
public:
  // less at top
  int operator()(TimeoutEvent *&a, TimeoutEvent *&b) {
    return a->timestamp_ < b->timestamp_;
  }
};

class TriggerEvent {
public:
  int64_t id_;
  int event_;
};

class EventLoopImpl : public EventLoop {
public:
  /* public api */

  EventLoopImpl();
  virtual ~EventLoopImpl();

  virtual int addReader(int64_t fd, FileHandler handler, void *data,
                        ResourceHandler resourceHandler);
  virtual int removeReader(int64_t fd);
  virtual int addWriter(int64_t fd, FileHandler handler, void *data,
                        ResourceHandler resourceHandler);
  virtual int removeWriter(int64_t fd);
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data,
                       ResourceHandler resourceHandler);
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data,
                       ResourceHandler resourceHandler, int repeat);
  virtual int removeTimer(int64_t id);
  virtual void start();
  virtual void stop();
  virtual int process();
  virtual int readerSize() const;
  virtual int writerSize() const;
  virtual int timerSize() const;
  virtual std::string api() const;
  virtual int64_t cachedTime() const;
  virtual bool hasReader(int64_t fd) const;
  virtual bool hasWriter(int64_t fd) const;
  virtual bool hasTimer(int64_t id) const;

public:
  /* inner api */

  // trigger file event
  virtual void trigger(int64_t id, int event);

private:
  virtual void freeReader(int64_t fd);
  virtual void freeWriter(int64_t fd, bool freeList);
  virtual void freeTimer(int64_t id);

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
