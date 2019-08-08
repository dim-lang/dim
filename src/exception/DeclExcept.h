// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <exception>
#include <stdexcept>
#include <string>

#ifndef F_DECL_EXCEPT
#define F_DECL_EXCEPT(x)                                                       \
  class x : public std::exception {                                            \
  public:                                                                      \
    x(const std::string &msg) : std::exception(), message_(msg) {}             \
    x(const x &) = default;                                                    \
    x &operator=(const x &) = default;                                         \
    x(x &&) = default;                                                         \
    x &operator=(x &&) = default;                                              \
    virtual ~x() = default;                                                    \
    virtual const char *what() const throw() { return message_.data(); }       \
                                                                               \
  private:                                                                     \
    std::string message_;                                                      \
  }
#endif
