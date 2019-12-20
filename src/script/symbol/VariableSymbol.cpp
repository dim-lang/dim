// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/symbol/VariableSymbol.h"

namespace fastype {

VariableSymbol::VariableSymbol(const icu::UnicodeString &name, int type)
    : name_(name), type_(type) {}

const icu::UnicodeString &VariableSymbol::name() const { return name_; }

int VariableSymbol::type() const { return type_; }

std::string VariableSymbol::toString() const {
  std::string _1;
  return fmt::format("<VariableSymbol {}:{}>", name_.toUTF8String(_1),
                     Type::nameUTF8(type_));
}

} // namespace fastype
