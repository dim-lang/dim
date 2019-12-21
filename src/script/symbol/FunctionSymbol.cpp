// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/symbol/FunctionSymbol.h"

namespace fastype {

FunctionSymbol::FunctionSymbol(
    const icu::UnicodeString &name,
    const std::vector<std::shared_ptr<Symbol>> &parameterList,
    std::shared_ptr<Symbol> type)
    : name_(name), parameterList_(parameterList), type_(type) {}

const icu::UnicodeString &FunctionSymbol::name() const { return name_; }

const std::vector<std::shared_ptr<Symbol>> &
FunctionSymbol::parameterList() const {
  return parameterList_;
}

std::shared_ptr<Symbol> FunctionSymbol::type() const { return Symbol::S_NIL; }

bool FunctionSymbol::equal(std::shared_ptr<Symbol> s) const {
  return s && type_->equal(s->type()) && (int)name_.compare(s->name()) == 0;
}

std::string FunctionSymbol::toString() const {
  std::string _1;
  return fmt::format("<FunctionSymbol {}>", name_.toUTF8String(_1));
}

} // namespace fastype
