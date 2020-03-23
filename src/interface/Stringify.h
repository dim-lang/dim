// Copyright 2019- <collie.org>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include <string>

class Stringify {
public:
  virtual std::string toString() const = 0;
};
