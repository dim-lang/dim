// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#pragma once
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "fmt/format.h"
#include "interface/Stringify.h"
#include <string>

class Exception : public Stringify {
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

#define X_DEF(x)                                                               \
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

#ifdef NDEBUG

#define EX_ASSERT(cond, ...)                                                    \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::string exmsg =                                                      \
          fmt::format("Assert Fail! Condition:{}, Result:{}\n",                \
                      BOOST_PP_STRINGIZE(cond), fmt::format(__VA_ARGS__));     \
      throw Exception(__FILE__, __LINE__, __FUNCTION__, exmsg);                \
    }                                                                          \
  } while (0)

#else

#define EX_ASSERT(cond, ...)                                                    \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::string msg = fmt::format(__VA_ARGS__);                              \
      fmt::print("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n",          \
                 __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond),   \
                 msg);                                                         \
      std::string exmsg =                                                      \
          fmt::format("Assert Fail! Condition:{}, Result:{}\n",                \
                      BOOST_PP_STRINGIZE(cond), msg);                          \
      throw Exception(__FILE__, __LINE__, __FUNCTION__, exmsg);                \
    }                                                                          \
  } while (0)

#endif
