// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"

class BuiltinTypeSymbol : public Symbol {
public:
  BuiltinTypeSymbol(const std::string &name, const std::string &type);
  virtual ~BuiltinTypeSymbol() = default;
  virtual const std::string &name() const;
  virtual const std::string &type() const;
  virtual std::string toString() const;

  static BuiltinTypeSymbol *i64Instance();
  static BuiltinTypeSymbol *stringInstance();

private:
  std::string variableName_;
  std::string variableType_;
};
