// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <exception>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

#ifndef F_EXCEPTION
#define F_EXCEPTION(x)                                                         \
  class x : public Stringify {                                                 \
  public:                                                                      \
    x(const char *fileName, int lineNumber, const char *functionName,          \
      const std::string &message)                                              \
        : message_(message), fileName_(fileName), lineNumber_(lineNumber),     \
          functionName_(functionName) {}                                       \
    virtual ~x() = default;                                                    \
    virtual const std::string &message() const { return message_; }            \
    virtual const std::string &fileName() const { return fileName_; }          \
    virtual const int &lineNumber() const { return lineNumber_; }              \
    virtual const std::string &functionName() const { return functionName_; }  \
    virtual std::string toString() const {                                     \
      return fmt::format("[{}:{}] {} - {}", fileName_, lineNumber_,            \
                         functionName_, message_);                             \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    std::string message_;                                                      \
    std::string fileName_;                                                     \
    int lineNumber_;                                                           \
    std::string functionName_;                                                 \
  }
#endif

namespace fastype {

class BaseException : public Stringify {
public:
  BaseException(const char *fileName, int lineNumber, const char *functionName,
                const std::string &message)
      : message_(message), fileName_(fileName), lineNumber_(lineNumber),
        functionName_(functionName) {}
  virtual ~BaseException() = default;
  virtual const std::string &message() const { return message_; }
  virtual const std::string &fileName() const { return fileName_; }
  virtual const int &lineNumber() const { return lineNumber_; }
  virtual const std::string &functionName() const { return functionName_; }
  virtual std::string toString() const {
    return fmt::format("[{}:{}] {} - {}", fileName_, lineNumber_, functionName_,
                       message_);
  }

protected:
  std::string message_;
  std::string fileName_;
  int lineNumber_;
  std::string functionName_;
};

} // namespace fastype

#ifndef F_THROW
#define F_THROW(ex, msg, ...)                                                  \
  throw ex(__FILE__, __LINE__, __FUNCTION__, fmt::format(msg, __VA_ARGS__))
#endif

#ifndef F_THROW_MSG
#define F_THROW_MSG(msg) throw ex(__FILE__, __LINE__, __FUNCTION__, msg)
#endif
