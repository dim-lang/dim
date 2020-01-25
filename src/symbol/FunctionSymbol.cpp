// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "symbol/FunctionSymbol.h"
#include "config/Header.h"

FunctionSymbol::FunctionSymbol(const std::string &functionName)
    : functionName_(functionName) {}

std::string FunctionSymbol::name() const { return functionName_; }

std::string FunctionSymbol::toString() const {
  return fmt::format("[ @FunctionSymbol functionName_:{} ]", functionName_);
}
