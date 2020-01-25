// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const std::string &name);
  virtual ~VariableSymbol() = default;
  virtual std::string name() const;
  virtual std::string toString() const;

private:
  std::string variableName_;
  std::string variableType_;
};
