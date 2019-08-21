// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "eventloop/EventLoopImpl.h"
#include "eventloop/Poll.h"
#include <ctime>
#include <string>
#include <unordered_map>
#include <vector>

namespace fastype {

EventLoopImpl::EventLoopImpl()
    : maxfd_(-1), size_(0), timeoutEventNextId_(-1), lastTime_(std::time()),
      stop_(true), poll_(Poll::open()) {}

EventLoopImpl::~EventLoopImpl() {
  if (poll_) {
    Poll::close(poll_);
    poll_ = nullptr;
  }
  stop_ = true;
}

int EventLoopImpl::addFileEvent(int64_t fd, int event, FileHandler readCb,
                                FileHandler writeCb, void *data) {

  FileEvent *fe = new FileEvent();
  return 0;
}

int EventLoopImpl::removeFileEvent(int64_t fd, int event) { return 0; }

int EventLoopImpl::addTimeoutEvent(int64_t millisec, TimeoutHandler timeoutcb,
                                   void *data) {
  return 0;
}

int EventLoopImpl::removeTimeoutEvent(int64_t id) { return 0; }

void EventLoopImpl::start() {}

void EventLoopImpl::stop() {}

int EventLoopImpl::process() { return 0; }

void EventLoopImpl::wait(int64_t millisec) {}

std::string EventLoopImpl::api() const { return poll_->name(); }

EventLoop *EventLoop::open() { return nullptr; }

void EventLoop::close(EventLoop *eventLoop) {}

} // namespace fastype
