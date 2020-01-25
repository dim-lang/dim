// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "symbol/VariableSymbol.h"
#include "config/Header.h"

VariableSymbol::VariableSymbol(const std::string &name) : variableName_(name) {}

std::string VariableSymbol::name() const { return variableName_; }

std::string VariableSymbol::toString() const {
  return fmt::format("[ @VariableSymbol variableName_:{} ]", variableName_);
}
