// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ScopeSymbolTable.h"
#include "script/symbol/BuiltinTypeSymbol.h"
#include <algorithm>
#include <utility>

namespace fastype {

ScopeSymbolTable::ScopeSymbolTable(const icu::UnicodeString &name, int level)
    : table_() {
  insert(BuiltinTypeSymbol::S_INTEGER);
  insert(BuiltinTypeSymbol::S_FLOATING);
}

void ScopeSymbolTable::insert(std::shared_ptr<Symbol> symbol) {
  table_.insert(std::make_pair(symbol->name(), symbol));
}

std::shared_ptr<Symbol>
ScopeSymbolTable::lookup(const icu::UnicodeString &name) const {
  auto s = table_.find(name);
  return s == table_.end() ? Symbol::S_NIL : s->second;
}

int ScopeSymbolTable::level() const { return level_; }

std::string ScopeSymbolTable::toString() const {
  return fmt::format("[ @ScopeSymbolTable table_#size:{}, level_:{} ]",
                     table_.size(), level_);
}

} // namespace fastype
