// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/SymbolTable.h"
#include "script/symbol/BuiltinTypeSymbol.h"
#include <algorithm>
#include <utility>

namespace fastype {

SymbolTable::SymbolTable(const icu::UnicodeString &name, int level) : table_() {
  insert(BuiltinTypeSymbol::S_INTEGER);
  insert(BuiltinTypeSymbol::S_FLOATING);
}

void SymbolTable::insert(std::shared_ptr<Symbol> symbol) {
  table_.insert(std::make_pair(symbol->name(), symbol));
}

std::shared_ptr<Symbol>
SymbolTable::lookup(const icu::UnicodeString &name) const {
  auto s = table_.find(name);
  return s == table_.end() ? Symbol::S_NIL : s->second;
}

int SymbolTable::level() const { return level_; }

std::string SymbolTable::toString() const {
  return fmt::format("[ @SymbolTable table_#size:{}, level_:{} ]",
                     table_.size(), level_);
}

} // namespace fastype
