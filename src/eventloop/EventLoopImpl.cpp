// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "eventloop/EventLoopImpl.h"
#include "eventloop/Poll.h"
#include <atomic>
#include <chrono>
#include <ctime>
#include <string>
#include <unordered_map>
#include <vector>

namespace fastype {

EventLoopImpl::EventLoopImpl() : timestamp_(-1), stop_(true), poll_(nullptr) {
  poll_ = Poll::open();
  updateTime();
}

EventLoopImpl::~EventLoopImpl() {
  // free memory in fileMap_ and timeoutMap_
  for (std::unordered_map<int64_t, FileEvent *>::iterator i = 0;
       i != fileMap_.end(); i++) {
    FileEvent *fe = i->second;
    i->second = nullptr;
    delete fe;
  }
  for (std::unordered_map<int64_t, TimeoutEvent *>::iterator i = 0;
       i != timeoutMap_.end(); i++) {
    TimeoutEvent *te = i->second;
    i->second = nullptr;
    delete te;
  }
  if (poll_) {
    Poll::close(poll_);
    poll_ = nullptr;
  }
  stop_ = true;
}

int EventLoopImpl::addFile(int64_t fd, FileHandler handler, void *data) {
  // fd already exist
  auto it = fileMap_.find(fd);
  if (it != fileMap_.end()) {
    return -1;
  }

  FileEvent *fe = new FileEvent();
  if (!fe) {
    return -1;
  }
  fe->fd_ = fd;
  fe->handler_ = handler;
  fe->data_ = data;

  fileMap_.insert(std::make_pair(fd, fe));
  poll_->add(fd);
  return 0;
}

int EventLoopImpl::removeFile(int64_t fd) {
  // fd not exist
  auto it = fileMap_.find(fd);
  if (it == fileMap_.end()) {
    return -1;
  }

  delete it->second;
  fileMap_.erase(it);

  return 0;
}

static uint64_t nextTimeoutId() {
  static std::atomic_ullong timeoutIdGen = 0;
  return timeoutIdGen.fetch_add(1);
}

int EventLoopImpl::addTimeout(int64_t millisec, TimeoutHandler handler,
                              void *data, int repeat) {

  updateTime();
  int64_t now = cachedTime();
  TimeoutEvent *te = new TimeoutEvent();
  if (!te) {
    return -1;
  }
  millisec = millisec < 0 ? -1 : millisec;

  te->id_ = nextTimeoutId();
  te->millisec_ = millisec;
  te->timestamp_ = now + millisec;
  te->handler_ = handler;
  te->data_ = data;
  te->repeat_ = repeat;

  timeoutMap_.insert(std::make_pair(te->id_, te));
  timeoutQueue_.push(te);

  return 0;
}

int EventLoopImpl::addTimeout(int64_t millisec, TimeoutHandler handler,
                              void *data) {
  return addTimeout(millisec, handler, data, 1);
}

int EventLoopImpl::removeTimeout(int64_t id) {
  // we don't remove timeout event from timeoutQueue_ here
  // remove operation will be done while process

  auto it = timeoutMap_.find(id);
  if (it == timeoutMap_.end()) {
    // id not exist
    return -1;
  }
  delete it->second;
  timeoutMap_.erase(it);

  return 0;
}

void EventLoopImpl::start() { stop_ = false; }

void EventLoopImpl::stop() { stop_ = true; }

int EventLoopImpl::process() {

  // already stopped
  if (!stop_) {
    return -1;
  }

  updateTime();

  int millisec = -1;
  int n = 0;

  // latest timeout event
  if (!timeoutQueue_.empty()) {
    TimeoutEvent *te = timeoutQueue_.top();
    millisec = te->millisec_;
  }

  triggerList_.clear();
  int p = poll_->poll(millisec);
  (void)p;

  // process file events
  for (int i = 0; i < triggerList_.size(); i++) {
    int64_t tfd = triggerList_[i];
    auto it = fileMap_.find(tfd);
    if (it == fileMap_.end()) {
      continue;
    }

    FileEvent *fe = it->second;
    FileHandler handler = fe->handler_;
    handler(this, fe->id_, fe->data_);
    n++;
  }

  // process timeout events
  while (!timeoutQueue_.empty()) {
    TimeoutEvent *te = timeoutQueue_.top();
    timeoutQueue_.pop();

    // if `te->id_` not found in timeoutMap_
    // it's been removed in `EventLoopImpl::removeTimeout`
    // destory it here
    auto it = timeoutMap_.find(te->id_);
    if (it == timeoutMap_.end()) {
      delete te;
      continue;
    }

    if (te->timestamp_ > cachedTime()) {
      break;
    }

    TimeoutHandler handler = te->handler_;
    handler(this, te->id_, te->data_);
    n++;

    // repeat timeout event again
    if (te->repeat_ - 1 > 0) {
      te->timestamp_ += te->millisec_;
      te->repeat_--;
      timeoutQueue_.push(te);
    } else {
      // destroy time event
      delete te;
    }
  }

  // processed event count
  return n;
}

int EventLoopImpl::fileSize() const { return fileMap_.size(); }

int EventLoopImpl::timeoutSize() const { return timeoutMap_.size(); }

std::string EventLoopImpl::api() const { return poll_->name(); }

void EventLoopImpl::trigger(int64_t id) { triggerList_.push_back(id); }

void EventLoopImpl::updateTime() {
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  auto dura = now.time_since_epoch();
  timestamp_ =
      std::chrono::duration_cast<std::chrono::milliseconds>(dura).count();
}

// @return cached time in milliseconds
int64_t EventLoopImpl::cachedTime() const { return timestamp_; }

EventLoop *EventLoop::open() { return new EventLoopImpl(); }

void EventLoop::close(EventLoop *eventLoop) {
  if (eventLoop) {
    delete (EventLoopImpl *)eventLoop;
  }
}

} // namespace fastype
