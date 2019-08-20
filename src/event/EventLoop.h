// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "event/Api.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace fastype {

class EventLoop;

typedef void fileCallback(EventLoop *evloop, uint64_t fd, void *data,
                          int event);
typedef void timeoutCallback(EventLoop *evloop, uint64_t eventId, void *data);

namespace detail {

struct FileEvent {
  int event;
  fileCallback *readcb;
  fileCallback *writecb;
  void *data;
};

struct TimeoutEvent {
  uint64_t eventId;
  int64_t millisec;
  timeoutCallback *timeoutcb;
  void *data;
};

struct TriggerEvent {
  uint64_t fd;
  int event;
};

} // namespace detail

class EventLoop {
public:
  EventLoop();
  virtual ~EventLoop() = default;

  int addFileEvent(uint64_t fd, int event, fileCallback readCb,
                   fileCallback writeCb, void *data);

  int removeFileEvent(uint64_t fd, int event);

  // @return timeoutEventId
  int addTimeoutEvent(int64_t millisec, timeoutCallback timeoutcb, void *data);

  int removeTimeoutEvent(uint64_t timeoutEventId);

  void start();
  void stop();
  void process();
  void wait(int64_t millisec);

  std::string api() const;

private:
  detail::FileEvent *fileEvent(uint64_t fd);
  detail::TimeoutEvent *timeoutEvent(uint64_t fd);
  void trigger(int index, uint64_t fd, int event);

  int maxfd_;
  int fdsize_;
  long long timeEventNextId;
  int64_t lastTime_;
  std::unordered_map<uint64_t, detail::FileEvent *> fileEventMap_;
  std::unordered_map<uint64_t, detail::TimeoutEvent *> timeoutEventMap_;
  std::vector<detail::TriggerEvent> triggerEventList_;
  bool stop_;

  Api *api_;

  friend class Api;
};

} // namespace fastype
