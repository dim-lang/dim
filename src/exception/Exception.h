// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <exception>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

#ifndef F_EXCEPTION
#define F_EXCEPTION(x)                                                         \
  class x : public std::exception {                                            \
  public:                                                                      \
    x(const std::string &msg)                                                  \
        : std::exception(), message_(msg), fileName_(nullptr),                 \
          lineNumber_(-1), functionName_(nullptr) {}                           \
    x(const char *fileName, int lineNumber, const char *functionName,          \
      const std::string &msg)                                                  \
        : std::exception(), message_(msg), fileName_(fileName),                \
          lineNumber_(lineNumber), functionName_(functionName) {}              \
    x(const x &) = default;                                                    \
    x &operator=(const x &) = default;                                         \
    x(x &&) = default;                                                         \
    x &operator=(x &&) = default;                                              \
    virtual ~x() = default;                                                    \
    virtual const char *what() const throw() { return message_.data(); }       \
                                                                               \
  private:                                                                     \
    std::string message_;                                                      \
    const char *fileName_;                                                     \
    int lineNumber_;                                                           \
    const char *functionName_;                                                 \
  }
#endif

#define F_THROW(ex, msg, ...)                                                  \
  throw new ex(__FILE__, __LINE__, __FUNCTION__, fmt::format(msg, __VA_ARGS__))

#define F_THROW_MSG(msg) throw new ex(__FILE__, __LINE__, __FUNCTION__, msg)
