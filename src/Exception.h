// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include <string>

class Exception {
public:
  Exception(const char *fileName, int lineNumber, const char *functionName,
            const std::string &message);
  virtual ~Exception() = default;
  virtual const std::string &message() const;
  virtual const std::string &fileName() const;
  virtual const int &lineNumber() const;
  virtual const std::string &functionName() const;
  virtual std::string toString() const;

protected:
  virtual std::string stringify() const;

  std::string message_;
  std::string fileName_;
  int lineNumber_;
  std::string functionName_;
};

#define EX_DEFINE(x)                                                           \
  class x : public Exception {                                                 \
  public:                                                                      \
    x(const char *fileName, int lineNumber, const char *functionName,          \
      const std::string &message)                                              \
        : Exception(fileName, lineNumber, functionName, message) {}            \
    virtual ~x() = default;                                                    \
                                                                               \
  protected:                                                                   \
    virtual std::string stringify() const { return BOOST_PP_STRINGIZE(x); }    \
  }

#define EX_ASSERT(cond, ...)                                                   \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::string msg1 = fmt::format(__VA_ARGS__);                             \
      LOG_ERROR("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n", __FILE__, \
                __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg1);       \
      std::string msg2 = fmt::format("Assert Fail! Condition:{}, Result:{}\n", \
                                     BOOST_PP_STRINGIZE(cond), msg1);          \
      throw Exception(__FILE__, __LINE__, __FUNCTION__, msg2);                 \
    }                                                                          \
  } while (0)
