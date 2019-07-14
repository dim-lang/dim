// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <exception>
#include <stdexcept>
#include <string>

namespace fastype {

class NoSuchOrderException : public std::runtime_error {
public:
  NoSuchOrderException() : std::runtime_error("") {}
  NoSuchOrderException(const std::string &msg) : std::runtime_error(msg) {}
  const char *what() const throw() { return std::runtime_error::what(); }
};

} // namespace fastype
