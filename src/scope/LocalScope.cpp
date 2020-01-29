// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "scope/LocalScope.h"
#include "Log.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

LocalScope::LocalScope(const std::string &scopeName,
                       std::shared_ptr<Scope> enclosingScope)
    : Scope(scopeName, enclosingScope) {}
