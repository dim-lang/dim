// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const std::string &name, const std::string &type);
  virtual ~VariableSymbol() = default;
  virtual const std::string &name() const;
  virtual const std::string &type() const;
  virtual std::string toString() const;

private:
  std::string variableName_;
  std::string variableType_;
};
