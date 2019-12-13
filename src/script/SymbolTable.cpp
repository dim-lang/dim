// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/SymbolTable.h"
#include <algorithm>
#include <sstream>
#include <utility>

namespace fastype {

SymbolTable::SymbolTable() : symbols_() {
  symbols_.insert(std::make_pair(Symbol::S_INTEGER->name(), Symbol::S_INTEGER));
  symbols_.insert(
      std::make_pair(Symbol::S_FLOATING->name(), Symbol::S_FLOATING));
}

void SymbolTable::define(std::shared_ptr<Symbol> symbol) {
  symbols_[symbol->name()] = symbol;
}

std::shared_ptr<Symbol>
SymbolTable::lookup(const icu::UnicodeString &name) const {
  if (symbols_.find(name) == symbols_.end()) {
    return Symbol::S_NIL;
  }
  return symbols_[name];
}

std::string SymbolTable::toString() const {
  std::stringstream ss;
  ss << fmt::format("[ @SymbolTable ");
  for (auto i = symbols_.begin(); i != symbols_.end(); i++) {
    std::string _1;
    ss << fmt::format("{}:{}, ", i->first.toUTF8String(_1),
                      i->second->toString());
  }
  ss << "]";
  return ss.str();
}

} // namespace fastype
