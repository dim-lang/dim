// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/TypeSymbolizable.h"

TypeSymbolizable::TypeSymbolizable(TypeSymbol *typeSymbol)
    : typeSymbolizable_(typeSymbol) {}

TypeSymbol *&TypeSymbolizable::typeSymbol() { return typeSymbolizable_; }

TypeSymbol *TypeSymbolizable::typeSymbol() const { return typeSymbolizable_; }

void TypeSymbolizable::resetTypeSymbol() { typeSymbolizable_ = nullptr; }
