// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/symbol/FunctionSymbol.h"

namespace fastype {

FunctionSymbol::FunctionSymbol(
    const icu::UnicodeString &name,
    const std::vector<std::shared_ptr<Symbol>> &parameterList)
    : name_(name), parameterList_(parameterList) {}

const icu::UnicodeString &FunctionSymbol::name() const { return name_; }

const std::vector<std::shared_ptr<Symbol>> &
FunctionSymbol::parameterList() const {
  return parameterList_;
}

std::shared_ptr<Symbol> FunctionSymbol::type() const { return Symbol::S_NIL; }

bool FunctionSymbol::equal(std::shared_ptr<Symbol> s) const {
  return s && s == Symbol::S_NIL;
}

std::string FunctionSymbol::toString() const {
  std::string _1;
  return fmt::format("<FunctionSymbol {}>", name_.toUTF8String(_1));
}

} // namespace fastype
