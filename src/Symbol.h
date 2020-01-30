// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>

class Symbol : public Stringify {
public:
  virtual ~Symbol() = default;
  virtual const std::string &name() const = 0;
  virtual const std::string &type() const = 0;
  virtual std::string toString() const = 0;
};
