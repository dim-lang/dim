// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "event/ApiConfig.h"

#ifdef F_EVENT_HAVE_KQUEUE

#include "event/Api.h"
#include <cstdint>
#include <poll.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

namespace fastype {

class EventLoop;

class KQueueApi : public Api {
public:
  KQueueApi(EventLoop *evloop);
  virtual ~KQueueApi();

  virtual int expand(int size);
  virtual int capacity() const;
  virtual int add(uint64_t fd, int event);
  virtual int remove(uint64_t fd, int event);
  virtual int poll(int64_t millisec);
  virtual std::string name() const;

private:
  int kqfd_;
  struct pollfd *fdset_;
  int size_;
  int capacity_;

  EventLoop *evloop_;

  friend class EventLoop;
};

} // namespace fastype

#endif
