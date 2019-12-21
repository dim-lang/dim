// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/symbol/VariableSymbol.h"

namespace fastype {

VariableSymbol::VariableSymbol(const icu::UnicodeString &name,
                               std::shared_ptr<Symbol> type)
    : name_(name), type_(type) {}

const icu::UnicodeString &VariableSymbol::name() const { return name_; }

std::shared_ptr<Symbol> VariableSymbol::type() const { return type_; }

bool VariableSymbol::equal(std::shared_ptr<Symbol> s) const {
  return s && type_->equal(s->type()) && (int)name_.compare(s->name()) == 0;
}

std::string VariableSymbol::toString() const {
  std::string _1, _2;
  return fmt::format("<VariableSymbol {}:{}>", name_.toUTF8String(_1),
                     type_->name().toUTF8String(_2));
}

} // namespace fastype
