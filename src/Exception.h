// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "boost/preprocessor/cat.hpp"
#include <exception>
#include <stdexcept>
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
  virtual std::string stringify() const = 0;

  std::string message_;
  std::string fileName_;
  int lineNumber_;
  std::string functionName_;
};

#ifndef CEXCEPT
#define CEXCEPT(x)                                                             \
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
#endif

#ifndef CTHROW
#define CTHROW(...)                                                            \
  throw fmt::format("{}:{} {} - {}", __FILE__, __LINE__, __FUNCTION__,         \
                    fmt::format(__VA_ARGS__))
#endif
