// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "eventloop/Api.h"
#include "Platform.h"

#include "eventloop/EPollApi.h"
#include "eventloop/KQueueApi.h"
#include "eventloop/PollApi.h"
#include "eventloop/SelectApi.h"
#include "eventloop/WinSelectApi.h"

namespace fastype {

Api *Api::open(EventLoop *evloop) {
#ifdef F_PLATFORM_LINUX
  Api *api = new EPollApi(evloop);
#elif defined(F_PLATFORM_MACOS)
  Api *api = new KQueueApi(evloop);
#elif defined(F_PLATFORM_WINDOWS)
  Api *api = new WinSelectApi(evloop);
#else
  Api *api = new SelectApi(evloop);
#endif

  return api;
}

void Api::close(Api *api) { delete api; }

} // namespace fastype
