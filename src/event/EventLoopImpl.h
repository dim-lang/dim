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

class FileEvent : public boost::noncopyable {
public:
  FileEvent() {}
  virtual ~FileEvent() = default;

  int64_t &id() { return id_; }
  void setId(int64_t id) { id_ = id; }
  FileHandler &handler() { return handler_; }
  void setHandler(FileHandler handler) { handler_ = handler; }
  void *data() { return nullptr; }
  void setData(void *data) {}

private:
  int64_t id_; // fd
  FileHandler handler_;
};

class TimeoutEvent : public boost::noncopyable {
public:
  TimeoutEvent() {}
  virtual ~TimeoutEvent() = default;

  int64_t &id() { return id_; }
  void setId(int64_t id) { id_ = id; }
  int64_t &timestamp() { return timestamp_; }
  void setTimestamp(int64_t timestamp) { timestamp_ = timestamp; }
  int64_t &millisec() { return millisec_; }
  void setMillisec(int64_t millisec) { millisec_ = millisec; }
  TimeoutHandler &handler() { return handler_; }
  void setHandler(TimeoutHandler handler) { handler_ = handler; }
  int &repeat() { return repeat_; }
  void setRepeat(int repeat) { repeat_ = repeat; }
  void *data() { return nullptr; }
  void setData(void *data) {}

private:
  int64_t id_;        // timeout event id
  int64_t timestamp_; // system timestamp
  int64_t millisec_;
  TimeoutHandler handler_;
  int repeat_;
};

class TimeoutEventComparator {
public:
  // less at top
  int operator()(TimeoutEvent *&a, TimeoutEvent *&b) {
    return a->timestamp() < b->timestamp();
  }
};

class TriggerEvent {
public:
  virtual ~TriggerEvent() = default;
  int64_t &id() { return id_; }
  void setId(int64_t id) { id_ = id; }
  int &event() { return event_; }
  void setEvent(int event) { event_ = event; }

private:
  int64_t id_;
  int event_;
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
  virtual int addTimer(int64_t millisec, TimeoutHandler handler, void *data,
                       int repeat = 1);
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
