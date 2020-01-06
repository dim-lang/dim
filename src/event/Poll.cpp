// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/Poll.h"
#include "config/Event.h"
#include "event/poll/Epoll.h"
#include "event/poll/Kqueue.h"
#include "event/poll/MswinSelect.h"
#include "event/poll/Select.h"

Poll *Poll::open(EventLoopImpl *evloop) {
#if defined(FEVENT_HAVE_EPOLL)
  return new Epoll(evloop);
#elif defined(FEVENT_HAVE_KQUEUE)
  return new Kqueue(evloop);
#elif defined(FPLATFORM_WINDOWS)
  return new MswinSelect(evloop);
#else
  return new Select(evloop);
#endif
}

void Poll::close(Poll *p) {
#if defined(FEVENT_HAVE_EPOLL)
  delete (Epoll *)p;
#elif defined(FEVENT_HAVE_KQUEUE)
  delete (Kqueue *)p;
#elif defined(FPLATFORM_WINDOWS)
  delete (MswinSelect *)p;
#else
  delete (Select *)p;
#endif
}
