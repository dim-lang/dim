// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "iface/Symbolizable.h"

Symbolizable::Symbolizable(Symbol *symbol) : symbolizable_(symbol) {}

Symbol *&Symbolizable::symbol() { return symbolizable_; }

Symbol *Symbolizable::symbol() const { return symbolizable_; }

void Symbolizable::resetSymbol() { symbolizable_ = nullptr; }
