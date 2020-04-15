// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#pragma once

#include "config/Platform.h"

#if defined(PLATFORM_BSD) || defined(PLATFORM_MACOS)
// kqueue, select
#define EVENT_HAVE_KQUEUE 1
#define EVENT_HAVE_SELECT 1

#elif defined(PLATFORM_LINUX)
// epoll, select
#define EVENT_HAVE_EPOLL 1
#define EVENT_HAVE_SELECT 1

#elif defined(PLATFORM_WINDOWS)
// winselect api
#define EVENT_HAVE_WINSELECT 1

#else
// select
#define EVENT_HAVE_SELECT 1

#endif
