// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "eventloop/Poll.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace fastype {

class EventLoop;

typedef void fileCallback(EventLoop *evloop, uint64_t fd, void *data,
                          int event);
typedef void timeoutCallback(EventLoop *evloop, uint64_t eventId, void *data);
typedef void timeoutFinalizeCallback(EventLoop *evloop, uint64_t eventId,
                                     void *data);

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

struct TriggeredEvent {};

} // namespace detail

class EventLoop {
public:
  EventLoop();
  virtual ~EventLoop() = default;

  int addFileEvent(int fd, int event, fileCallback readCb, fileCallback writeCb,
                   void *data);
  int removeFileEvent(int fd, int event);

  // @return timeoutEventId
  int addTimeoutEvent(int64_t millisec, timeoutCallback timeoutcb, void *data,
                      timeoutFinalizeCallback timeoutFinalizeCb);

  int removeTimeoutEvent(int timeoutEventId);

  void start();
  void stop();
  void process();
  void wait(int64_t millisec);

  std::string pollApi() const;

private:
  detail::FileEvent *getFileEvent(uint64_t fd);
  detail::TimeoutEvent *getTimeoutEvent(uint64_t fd);
  void trigger(int index, uint64_t fd, int event);

  int maxfd_;
  int fdsize_;
  long long timeEventNextId;
  int64_t lastTime_;
  std::unordered_map<uint64_t, detail::FileEvent *> fileEventMap_;
  std::unordered_map<uint64_t, detail::TimeoutEvent *> timeoutEventMap_;
  std::vector<detail::TriggeredEvent *> triggeredEventList_;
  bool stop_;

  Poll *poll_;

  friend class Poll;
};

} // namespace fastype
