// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/EventLoopImpl.h"
#include "DateTime.h"
#include "event/Poll.h"
#include <atomic>
#include <chrono>
#include <ctime>
#include <string>
#include <unordered_map>
#include <vector>

namespace fastype {

EventLoopImpl::EventLoopImpl() : timestamp_(-1), stop_(true), poll_(nullptr) {
  poll_ = Poll::open(this);
  timestamp_ = DateTime::millinow();
}

EventLoopImpl::~EventLoopImpl() {
  // free memory in readerMap_ and timerMap_
  for (std::unordered_map<int64_t, FileEvent *>::iterator i =
           readerMap_.begin();
       i != readerMap_.end(); i++) {
    freeReader(i->first);
  }
  for (std::unordered_map<int64_t, std::list<FileEvent *> *>::iterator i =
           writerMap_.begin();
       i != writerMap_.end(); i++) {
    freeWriter(i->first, true);
  }
  for (std::unordered_map<int64_t, TimeoutEvent *>::iterator i =
           timerMap_.begin();
       i != timerMap_.end(); i++) {
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

int EventLoopImpl::addReader(int64_t fd, FileHandler handler, void *data) {
  // fd already exist
  auto it = readerMap_.find(fd);
  if (it != readerMap_.end()) {
    return -1;
  }

  FileEvent *fe = new FileEvent();
  if (!fe) {
    return -1;
  }
  fe->setId(fd);
  fe->setHandler(handler);
  fe->setData(data);

  readerMap_.insert(std::make_pair(fd, fe));
  poll_->add(fd, F_EVENT_READ);
  return 0;
}

int EventLoopImpl::removeReader(int64_t fd) {
  // fd not exist
  auto it = readerMap_.find(fd);
  if (it == readerMap_.end()) {
    return -1;
  }

  freeReader(fd);
  readerMap_.erase(it);

  return 0;
}

int EventLoopImpl::addWriter(int64_t fd, FileHandler handler, void *data) {
  // fd not exist
  auto it = writerMap_.find(fd);
  if (it == writerMap_.end()) {
    std::list<FileEvent *> *writeList = new std::list<FileEvent *>();
    if (!writeList) {
      return -1;
    }
    writerMap_.insert(std::make_pair(fd, writeList));
  }

  FileEvent *fe = new FileEvent();
  if (!fe) {
    return -1;
  }
  fe->setId(fd);
  fe->setHandler(handler);
  fe->setData(data);

  writerMap_[fd]->push_front(fe);
  poll_->add(fd, F_EVENT_WRITE);
  return 0;
}

int EventLoopImpl::removeWriter(int64_t fd) {
  // fd not exist
  auto it = writerMap_.find(fd);
  if (it == writerMap_.end()) {
    return -1;
  }

  freeWriter(fd, true);
  writerMap_.erase(it);

  return 0;
}

static uint64_t nextTimeoutId() {
  static std::atomic_ullong timeoutIdGen(0);
  return timeoutIdGen.fetch_add(1);
}

int EventLoopImpl::addTimer(int64_t millisec, TimeoutHandler handler,
                            void *data, int repeat) {

  timestamp_ = DateTime::millinow();
  int64_t now = cachedTime();
  TimeoutEvent *te = new TimeoutEvent();
  if (!te) {
    return -1;
  }
  millisec = millisec < 0 ? -1 : millisec;

  te->setId(nextTimeoutId());
  te->setMillisec(millisec);
  te->setTimestamp(now + millisec);
  te->setHandler(handler);
  te->setData(data);
  te->setRepeat(repeat);

  timerMap_.insert(std::make_pair(te->id(), te));
  timerQueue_.push(te);

  return 0;
}

int EventLoopImpl::removeTimer(int64_t id) {
  // we don't remove timeout event from timerQueue_ here
  // remove operation will be done while process

  auto it = timerMap_.find(id);
  if (it == timerMap_.end()) {
    // id not exist
    return -1;
  }
  freeTimer(id);
  timerMap_.erase(it);

  return 0;
}

void EventLoopImpl::start() { stop_ = false; }

void EventLoopImpl::stop() { stop_ = true; }

int EventLoopImpl::process() {

  // already stopped
  if (!stop_) {
    return -1;
  }

  timestamp_ = DateTime::millinow();

  int millisec = -1;
  int n = 0;

  // latest timeout event
  if (!timerQueue_.empty()) {
    TimeoutEvent *te = timerQueue_.top();
    millisec = te->millisec();
  }

  triggerList_.clear();
  int p = poll_->poll(millisec);
  (void)p;

  // process file events
  for (int i = 0; i < triggerList_.size(); i++) {
    TriggerEvent te = triggerList_[i];
    if (te.event() & F_EVENT_READ) {
      auto it = readerMap_.find(te.id());
      if (it == readerMap_.end()) {
        continue;
      }
      FileEvent *fe = it->second;
      FileHandler handler = fe->handler();
      handler(this, fe->id(), fe->data());
      n++;
    }
    if (te.event() & F_EVENT_WRITE) {
      auto it = writerMap_.find(te.id());
      if (it == writerMap_.end()) {
        continue;
      }
      std::list<FileEvent *> *writeList = it->second;
      for (std::list<FileEvent *>::iterator i = writeList->begin();
           i != writeList->end(); i++) {
        FileEvent *fe = *i;
        FileHandler handler = fe->handler();
        handler(this, fe->id(), fe->data());
        n++;
      }
      freeWriter(te.id(), false);
    }
  }

  // process timeout events
  while (!timerQueue_.empty()) {
    TimeoutEvent *te = timerQueue_.top();
    timerQueue_.pop();

    // if `te->id_` not found in timerMap_
    // it's been removed in `EventLoopImpl::removeTimeout`
    // destory it here
    auto it = timerMap_.find(te->id());
    if (it == timerMap_.end()) {
      delete te;
      continue;
    }

    if (te->timestamp() > cachedTime()) {
      break;
    }

    TimeoutHandler handler = te->handler();
    handler(this, te->id(), te->data());
    n++;

    // repeat timeout event again
    if (te->repeat() - 1 > 0) {
      te->setTimestamp(te->timestamp() + te->millisec());
      te->setRepeat(te->repeat() - 1);
      timerQueue_.push(te);
    } else {
      // destroy time event
      delete te;
    }
  }

  // processed event count
  return n;
}

int EventLoopImpl::readerSize() const { return readerMap_.size(); }

int EventLoopImpl::writerSize() const { return writerMap_.size(); }

int EventLoopImpl::timerSize() const { return timerMap_.size(); }

std::string EventLoopImpl::api() const { return poll_->name(); }

void EventLoopImpl::trigger(int64_t id, int event) {
  TriggerEvent te;
  te.setId(id);
  te.setEvent(event);
  triggerList_.push_back(te);
}

int64_t EventLoopImpl::cachedTime() const { return timestamp_; }

bool EventLoopImpl::hasReader(int64_t fd) const {
  return readerMap_.find(fd) != readerMap_.end();
}

bool EventLoopImpl::hasWriter(int64_t fd) const {
  return writerMap_.find(fd) != writerMap_.end();
}

bool EventLoopImpl::hasTimer(int64_t id) const {
  return timerMap_.find(id) != timerMap_.end();
}

void EventLoopImpl::freeReader(int64_t fd) {
  // fd not exist
  auto it = readerMap_.find(fd);
  if (it == readerMap_.end()) {
    return;
  }
  delete it->second;
  it->second = nullptr;
}

void EventLoopImpl::freeWriter(int64_t fd, bool freeList) {
  // fd not exist
  auto it = writerMap_.find(fd);
  if (it == writerMap_.end()) {
    return;
  }

  std::list<FileEvent *> *writeList = it->second;

  for (std::list<FileEvent *>::iterator i = writeList->begin();
       i != writeList->end(); i++) {
    FileEvent *fe = *i;
    delete fe;
  }
  writeList->clear();
  if (freeList) {
    delete writeList;
    it->second = nullptr;
  }
}

void EventLoopImpl::freeTimer(int64_t id) {
  auto it = timerMap_.find(id);
  if (it == timerMap_.end()) {
    // id not exist
    return;
  }
  delete it->second;
  it->second = nullptr;
}

EventLoop *EventLoop::open() { return (EventLoop *)new EventLoopImpl(); }

void EventLoop::close(EventLoop *eventLoop) {
  if (eventLoop) {
    delete (EventLoopImpl *)eventLoop;
  }
}

} // namespace fastype
