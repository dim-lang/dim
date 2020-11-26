// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once

#ifdef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif // #ifdef NDEBUG

#include "boost/preprocessor/stringize.hpp"
#include "fmt/format.h"
#include "infra/Cowstr.h"
#include "spdlog/spdlog.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <string>

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_ASSERT(cond, ...)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      Cowstr msg1 = fmt::format(__VA_ARGS__);                                  \
      fmt::print(stderr, "Assert Fail! {}:{} {} - Condition:{}, Result:{}\n",  \
                 __FILE__, __LINE__, __FUNCTION__,                             \
                 BOOST_PP_STRINGIZE(cond), msg1);                              \
      LOG_ERROR("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n", __FILE__, \
                __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg1);       \
      throw Cowstr(fmt::format(                                                \
          "Assert Fail! {}:{} {} - Condition:{}, Result:{}\n", __FILE__,       \
          __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg1));            \
    }                                                                          \
  } while (0)
