// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/Api.h"
#include "event/ApiConfig.h"
#include "event/EPollApi.h"
#include "event/KQueueApi.h"
#include "event/SelectApi.h"
#include "event/WinSelectApi.h"

namespace fastype {

Api *Api::open(EventLoop *evloop) {
#if defined(F_EVENT_HAVE_EPOLL)
  return new EPollApi(evloop);
#elif defined(F_EVENT_HAVE_KQUEUE)
  return new KQueueApi(evloop);
#elif defined(F_PLATFORM_WINDOWS)
  return new WinSelectApi(evloop);
#else
  return new SelectApi(evloop);
#endif
}

void Api::close(Api *api) {
#if defined(F_EVENT_HAVE_EPOLL)
  delete (EPollApi *)api;
#elif defined(F_EVENT_HAVE_KQUEUE)
  delete (KQueueApi *)api;
#elif defined(F_PLATFORM_WINDOWS)
  delete (WinSelectApi *)api;
#else
  delete (SelectApi *)api;
#endif
}

} // namespace fastype
