// copyright 2019- <coli-lang>
// apache license version 2.0

#pragma once
#include "Scope.h"

class LocalScope : public Scope {
public:
  LocalScope(const std::string &scopeName,
             std::shared_ptr<Scope> enclosingScope);
  virtual ~LocalScope() = default;
};
