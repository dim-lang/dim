// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <string>

namespace fastype {

class Stringify {
public:
  virtual std::string toString() const = 0;
};

} // namespace fastype
