// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/symbol/BuiltinTypeSymbol.h"

namespace fastype {

const BuiltinTypeSymbol
    BuiltinTypeSymbol::S_INTEGER(UNICODE_STRING_SIMPLE("INTEGER"));
const BuiltinTypeSymbol
    BuiltinTypeSymbol::S_FLOATING(UNICODE_STRING_SIMPLE("FLOATING"));

BuiltinTypeSymbol::BuiltinTypeSymbol(const icu::UnicodeString &name)
    : name_(name) {}

const icu::UnicodeString &BuiltinTypeSymbol::name() const { return name_; }

std::shared_ptr<Symbol> BuiltinTypeSymbol::type() const {
  return Symbol::S_NIL;
}

bool BuiltinTypeSymbol::equal(std::shared_ptr<Symbol> s) const {
  return s && s->type() == Symbol::S_NIL && (int)name_.compare(s->name()) == 0;
}

std::string BuiltinTypeSymbol::toString() const {
  std::string _1;
  return fmt::format("<BuiltinTypeSymbol:{}>", name_.toUTF8String(_1));
}

} // namespace fastype
