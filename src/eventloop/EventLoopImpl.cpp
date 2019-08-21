// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "eventloop/EventLoopImpl.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace fastype {

EventLoopImpl::EventLoopImpl() {}

EventLoopImpl::~EventLoopImpl() {}

int EventLoopImpl::addFileEvent(uint64_t fd, int event, FileHandler readCb,
                                FileHandler writeCb, void *data) {
  return 0;
}

int EventLoopImpl::removeFileEvent(uint64_t fd, int event) { return 0; }

int EventLoopImpl::addTimeoutEvent(int64_t millisec, TimeoutHandler timeoutcb,
                                   void *data) {
  return 0;
}

int EventLoopImpl::removeTimeoutEvent(uint64_t id) { return 0; }

void EventLoopImpl::start() {}

void EventLoopImpl::stop() {}

int EventLoopImpl::process() { return 0; }

void EventLoopImpl::wait(int64_t millisec) {}

std::string EventLoopImpl::api() const { return poll_->name(); }

EventLoop *EventLoop::open() { return nullptr; }

void EventLoop::close(EventLoop *eventLoop) {}

} // namespace fastype
