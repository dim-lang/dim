// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/EventLoop.h"
#include "event/Api.h"
#include <string>

namespace fastype {

EventLoop::EventLoop() {}

int EventLoop::addFileEvent(int fd, int event, fileCallback readCb,
                            fileCallback writeCb, void *data) {}

int EventLoop::removeFileEvent(uint64_t fd, int event) {
  fileEventMap_.erase(fd);
}

int EventLoop::addTimeoutEvent(int64_t millisec, timeoutCallback timeoutcb,
                               void *data) {}

int EventLoop::removeTimeoutEvent(uint64_t timeoutEventId) {
  timeoutEventMap_.erase(timeoutEventId);
  return 0;
}

void EventLoop::start() {}

void EventLoop::stop() {}

void EventLoo::process() {}

void EventLoo::wait(int64_t millisec) {}

std::string EventLoop::api() const { return api_->name(); }

detail::FileEvent *EventLoop::fileEvent(uint64_t fd) {
  return fileEventMap_[fd];
}

detail::TimeoutEvent *EventLoop::timeoutEvent(uint64_t fd) {
  return timeoutEventMap_[fd];
}

void EventLoop::trigger(int index, uint64_t fd, int event) {
  triggerEventList_[index].fd = fd;
  triggerEventList_[index].event = event;
}

} // namespace fastype
