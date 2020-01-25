// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "scope/AnonymousScope.h"
#include "Log.h"
#include "Symbol.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

AnonymousScope::AnonymousScope(const std::string &scopeName)
    : scopeName_(scopeName) {}

std::string AnonymousScope::name() const { return scopeName_; }

std::string AnonymousScope::toString() const {
  return fmt::format(
      "[ @AnonymousScope name_:AnonymousScope, symtab_#size:{} ]",
      symtab_.size());
}
