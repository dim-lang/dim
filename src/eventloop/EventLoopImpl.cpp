// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "eventloop/EventLoopImpl.h"
#include "eventloop/Poll.h"
#include <ctime>
#include <string>
#include <unordered_map>
#include <vector>

namespace fastype {

FileEvent::FileEvent(uint64_t id, int event, FileHandler *readHandler,
                     FileHandler *writeHandler_, void *data)
    : id_(id), event_(event), readHandler_(readHandler),
      writeHandler_(writeHandler), data_(data) {}

TimeoutEvent::TimeoutEvent(uint64_t id, int64_t millisec,
                           TimeoutHandler *TimeoutHandler,
                           FileHandler *writeHandler_, void *data)
    : id_(id), event_(event), readHandler_(readHandler),
      writeHandler_(writeHandler), data_(data) {}

TriggerEvent::TriggerEvent(uint64_t id, int event) : id_(id), event_(event) {}

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

int EventLoopImpl::addFileEvent(uint64_t fd, int event, FileHandler readHandler,
                                FileHandler writeHandler, void *data) {

  FileEvent *fe = new FileEvent(fd, event, readHandler, writeHandler, data);
  auto it = fileEventMap_.find(fd);
  if (it != fileEventMap_.end()) {
    delete it->second;
  }
  fileEventMap_.insert_or_assign(std::make_pair(fd, fe));
  poll_->add(fd, event);
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
