// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "symbol/VariableSymbol.h"
#include "config/Header.h"

VariableSymbol::VariableSymbol(const std::string &name, const std::string &type)
    : variableName_(name), variableType_(type) {}

const std::string &VariableSymbol::name() const { return variableName_; }

const std::string &VariableSymbol::type() const { return variableType_; }

std::string VariableSymbol::toString() const {
  return fmt::format("[ @VariableSymbol variableName_:{}, variableType_:{} ]",
                     variableName_, variableType_);
}
