// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "script/Symbol.h"
#include <memory>
#include <vector>

namespace fastype {

class BuiltinTypeSymbol : public Symbol {
public:
  BuiltinTypeSymbol();
  virtual ~BuiltinTypeSymbol() = default;
  virtual const icu::UnicodeString &name() const;
  virtual SymbolType type() const;
  virtual std::string toString() const;
};

} // namespace fastype
