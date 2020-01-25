// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "symbol/ClassSymbol.h"
#include "config/Header.h"

ClassSymbol::ClassSymbol(const std::string &functionName)
    : functionName_(functionName) {}

std::string ClassSymbol::name() const { return functionName_; }

std::string ClassSymbol::toString() const {
  return fmt::format("[ @ClassSymbol functionName_:{} ]", functionName_);
}
