// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include <memory>
#include <vector>

namespace fastype {

class Symbol : public Stringify {
public:
  enum SymbolType {
    BUILDIN = 1,
    VARIABLE = 2,
  };

  virtual ~Symbol() = default;
  virtual const icu::UnicodeString &name() const = 0;
  virtual SymbolType type() const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype
