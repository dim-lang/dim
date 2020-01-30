// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "symbol/FunctionSymbol.h"
#include "config/Header.h"

FunctionSymbol::FunctionSymbol(const std::string &name, const std::string &type,
                               std::shared_ptr<Scope> enclosingScope)
    : Scope(name, enclosingScope), functionType_(type) {}

const std::string &FunctionSymbol::type() const { return functionType_; }

std::string FunctionSymbol::toString() const {
  return fmt::format("[ @FunctionSymbol functionName_:{}, functionType_:{}, "
                     "enclosingScope_:{} ]",
                     name(), type(),
                     enclosingScope_ ? enclosingScope_->toString() : "null");
}
