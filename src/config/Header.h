// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "config/Platform.h"

#if PLATFORM_WINDOWS

#include "boost/align/align_up.hpp"
#include "boost/core/noncopyable.hpp"
#include "boost/filesystem.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"
#include "fmt/format.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "unicode/uchar.h"
#include "unicode/unistr.h"
#include "unicode/ustdio.h"
#include "unicode/ustring.h"
#include "unicode/utypes.h"

#else

#include <boost/align/align_up.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

#endif
