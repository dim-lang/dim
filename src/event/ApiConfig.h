// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "Platform.h"

#if defined(F_PLATFORM_BSD) || defined(F_PLATFORM_MACOS)
// kqueue, select
#define F_EVENT_HAVE_KQUEUE 1
#define F_EVENT_HAVE_SELECT 1

#elif defined(F_PLATFORM_LINUX)
// epoll, select
#define F_EVENT_HAVE_EPOLL 1
#define F_EVENT_HAVE_SELECT 1

#elif defined(F_PLATFORM_WINDOWS)
// winselect api
#define F_EVENT_HAVE_WINSELECT 1

#elif defined(F_PLATFORM_UNIX)
// select
#define F_EVENT_HAVE_SELECT 1

#endif
