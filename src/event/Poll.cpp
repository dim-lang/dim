// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/Poll.h"
#include "config/Event.h"
#include "event/poll/Epoll.h"
#include "event/poll/Kqueue.h"
#include "event/poll/Select.h"
#include "event/poll/WinSelect.h"

namespace fastype {

Poll *Poll::open(EventLoopImpl *evloop) {
#if defined(F_EVENT_HAVE_EPOLL)
  return new Epoll(evloop);
#elif defined(F_EVENT_HAVE_KQUEUE)
  return new Kqueue(evloop);
#elif defined(F_PLATFORM_WINDOWS)
  return new WinSelect(evloop);
#else
  return new Select(evloop);
#endif
}

void Poll::close(Poll *p) {
#if defined(F_EVENT_HAVE_EPOLL)
  delete (Epoll *)p;
#elif defined(F_EVENT_HAVE_KQUEUE)
  delete (Kqueue *)p;
#elif defined(F_PLATFORM_WINDOWS)
  delete (WinSelect *)p;
#else
  delete (Select *)p;
#endif
}

} // namespace fastype
