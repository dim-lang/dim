// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>

class Type {
public:
  virtual ~Type() = default;
  virtual const std::string &name() const = 0;
};
