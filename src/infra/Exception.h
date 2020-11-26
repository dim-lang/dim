// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#pragma once
#include "infra/Cowstr.h"
#include "infra/Log.h"

class Exception {
public:
  Exception(const Cowstr &fileName, int lineNumber, const Cowstr &function,
            const Cowstr &message);
  virtual ~Exception() = default;
  virtual const Cowstr &message() const;
  virtual const Cowstr &fileName() const;
  virtual const int &lineNumber() const;
  virtual const Cowstr &function() const;

protected:
  Cowstr message_;
  Cowstr fileName_;
  int lineNumber_;
  Cowstr function_;
};

#define MUST(cond, ...)                                                        \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw Exception(__FILE__, __LINE__, __FUNCTION__,                        \
                      fmt::format(__VA_ARGS__));                               \
    }                                                                          \
  } while (0)

#define FAIL(...)                                                              \
  do {                                                                         \
    throw Exception(__FILE__, __LINE__, __FUNCTION__,                          \
                    fmt::format(__VA_ARGS__));                                 \
  } while (0)
