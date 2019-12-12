// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/SymbolTable.h"

namespace fastype {

SymbolTable::SymbolTable() : symbols_() {}

void SymbolTable::define(std::shared_ptr<Symbol> symbol) {
  symbols_[symbol->name()] = symbol;
}

std::shared_ptr<Symbol>
SymbolTable::lookup(const icu::UnicodeString &name) const {
  if (symbols_.find(name) == symbols_.end()) {
    return std::shared_ptr<Symbol>(nullptr);
  }
  return symbols_[name];
}

} // namespace fastype
