// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <string>

class Type {
public:
  virtual std::string name() const = 0;
};
