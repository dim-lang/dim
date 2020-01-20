// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

#include "config/Platform.h"

#if defined(FPLATFORM_BSD) || defined(FPLATFORM_MACOS)
// kqueue, select
#define FEVENT_HAVE_KQUEUE 1
#define FEVENT_HAVE_SELECT 1

#elif defined(FPLATFORM_LINUX)
// epoll, select
#define FEVENT_HAVE_EPOLL 1
#define FEVENT_HAVE_SELECT 1

#elif defined(FPLATFORM_WINDOWS)
// winselect api
#define FEVENT_HAVE_WINSELECT 1

#elif defined(FPLATFORM_UNIX)
// select
#define FEVENT_HAVE_SELECT 1

#endif
