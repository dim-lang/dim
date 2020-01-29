// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "scope/GlobalScope.h"
#include "Log.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

GlobalScope::GlobalScope() : Scope("GlobalScope") {}

std::shared_ptr<Scope> GlobalScope::instance() {
  static std::shared_ptr<Scope> globalScopeInstance(new GlobalScope());
  return globalScopeInstance;
}
