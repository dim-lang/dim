// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "symbol/BuiltinTypeSymbol.h"
#include "config/Header.h"

BuiltinTypeSymbol::BuiltinTypeSymbol(const std::string &name,
                                     const std::string &type)
    : variableName_(name), variableType_(type) {}

const std::string &BuiltinTypeSymbol::name() const { return variableName_; }

const std::string &BuiltinTypeSymbol::type() const { return variableType_; }

std::string BuiltinTypeSymbol::toString() const {
  return fmt::format(
      "[ @BuiltinTypeSymbol variableName_:{}, variableType_:{} ]",
      variableName_, variableType_);
}
