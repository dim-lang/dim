// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/symbol/BuiltinTypeSymbol.h"

namespace fastype {

BuiltinTypeSymbol::BuiltinTypeSymbol(const icu::UnicodeString &name)
    : name_(name) {}

const icu::UnicodeString &BuiltinTypeSymbol::name() const { return name_; }

int &BuiltinTypeSymbol::type() const { return F_TYPE_BUILTIN_SYMBOL; }

std::string BuiltinTypeSymbol::toString() const {
  std::string _1;
  return fmt::format("<BuiltinTypeSymbol:{}>", name_.toUTF8String(_1));
}

} // namespace fastype