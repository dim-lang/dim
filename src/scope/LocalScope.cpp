// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "scope/LocalScope.h"
#include "Log.h"
#include "Symbol.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

LocalScope::LocalScope(const std::string &scopeName) : scopeName_(scopeName) {}

std::string LocalScope::name() const { return scopeName_; }

std::string LocalScope::toString() const {
  return fmt::format("[ @LocalScope name_:LocalScope, symtab_#size:{} ]",
                     symtab_.size());
}
