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
    : maxfd_(0), size_(0), timeoutEventNextId_(-1),
      lastTime_(std::time(nullptr)), stop_(true), poll_(Poll::open()) {}

EventLoopImpl::~EventLoopImpl() {
  if (poll_) {
    Poll::close(poll_);
    poll_ = nullptr;
  }
  stop_ = true;
}

int EventLoopImpl::addFileEvent(uint64_t fd, int event, FileHandler readHandler,
                                FileHandler writeHandler, void *data) {
  FileEvent *fe = nullptr;

  auto it = fileEventMap_.find(fd);
  if (it != fileEventMap_.end()) {
    fe = it->second;
  } else {
    fe = new FileEvent();
    fe->fd_ = fd;
    fe->event_ = F_EVENT_NONE;
    fe->readHandler_ = nullptr;
    fe->writeHandler_ = nullptr;
    fe->data_ = data;
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

void EventLoopImpl::stop() { stop_ = true; }

int EventLoopImpl::process() { return 0; }

void EventLoopImpl::wait(int64_t millisec) {}

std::string EventLoopImpl::api() const { return poll_->name(); }

EventLoop *EventLoop::open() { return nullptr; }

void EventLoop::close(EventLoop *eventLoop) {}

} // namespace fastype
