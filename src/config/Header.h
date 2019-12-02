// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Platform.h"

#if F_PLATFORM_WINDOWS

#include "boost/assert.hpp"
#include "boost/core/noncopyable.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/program_options.hpp"
#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include "unicode/uchar.h"
#include "unicode/unistr.h"
#include "unicode/ustdio.h"
#include "unicode/ustring.h"
#include "unicode/utypes.h"

#else

#include <boost/assert.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/program_options.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

#endif
