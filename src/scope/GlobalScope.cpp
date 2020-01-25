// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "scope/GlobalScope.h"
#include "Log.h"
#include "Symbol.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

std::string GlobalScope::name() const { return "GlobalScope"; }

std::string GlobalScope::toString() const {
  return fmt::format("[ @GlobalScope name_:GlobalScope, symbolTable_#size:{} ]",
                     symbolTable_.size());
}

std::shared_ptr<Scope> GlobalScope::instance() {
  static std::shared_ptr<Scope> globalScopeInstance(new GlobalScope());
  return globalScopeInstance;
}
