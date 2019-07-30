// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "boost/assert.hpp"
#include "boost/chrono.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/timer/timer.hpp"
#include "fmt/format.h"
#include <string>

#ifdef NDEBUG

#ifndef F_CHECK
#define F_CHECK(cond, msg)
#endif
#ifndef F_CHECKF
#define F_CHECKF(cond, fmtMsg, ...)
#endif

#ifndef F_TIMER_START
#define F_TIMER_START(t)
#endif

#ifndef F_TIMER_STOP
#define F_TIMER_STOP(t)
#endif

#ifndef F_TIMER_RESUME
#define F_TIMER_RESUME(t)
#endif

#ifndef F_TIMER_TO_STRING
#define F_TIMER_TO_STRING(t)
#endif

#else

#ifndef F_CHECK
#define F_CHECK(cond, msg)                                                     \
  do {                                                                         \
    BOOST_ASSERT_MSG(                                                          \
        cond, fastype::detail::FormatLocation(F_LOG_LOCATION, msg).data());    \
  } while (0)
#endif

#ifndef F_CHECKF
#define F_CHECKF(cond, fmtMsg, ...)                                            \
  do {                                                                         \
    BOOST_ASSERT_MSG(cond, fmt::format(fastype::detail::FormatLocation(        \
                                           F_LOG_LOCATION, fmtMsg),            \
                                       __VA_ARGS__)                            \
                               .data());                                       \
  } while (0)
#endif

#ifndef F_TIMER_START
#define F_TIMER_START(t) boost::timer::cpu_timer BOOST_PP_CAT(fastype_timer_, t)
#endif

#ifndef F_TIMER_STOP
#define F_TIMER_STOP(t) BOOST_PP_CAT(fastype_timer_, t).stop()
#endif

#ifndef F_TIMER_RESUME
#define F_TIMER_RESUME(t) BOOST_PP_CAT(fastype_timer_, t).resume()
#endif

#ifndef F_TIMER_ELAPSE
#define F_TIMER_ELAPSE(t)                                                      \
  boost::chrono::nanoseconds(BOOST_PP_CAT(fastype_timer_, t).elapsed().user +  \
                             BOOST_PP_CAT(fastype_timer_, t).elapsed().system) \
      .count()
#endif

#endif
