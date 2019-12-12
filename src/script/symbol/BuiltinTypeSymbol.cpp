// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "script/Symbol.h"
#include <memory>
#include <vector>

namespace fastype {

BuiltinTypeSymbol::BuiltinTypeSymbol() {}

const icu::UnicodeString &BuiltinTypeSymbol::name() const {}

SymbolType BuiltinTypeSymbol::type() const {}

std::string BuiltinTypeSymbol::toString() const {}

} // namespace fastype
