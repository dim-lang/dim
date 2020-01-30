// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"
#include "scope/GlobalScope.h"
#include "scope/LocalScope.h"

class FunctionSymbol : public Symbol, Scope {
public:
  FunctionSymbol(
      const std::string &name, const std::string &type,
      std::shared_ptr<Scope> enclosingScope = GlobalScope::instance());
  virtual ~FunctionSymbol() = default;
  virtual const std::string &type() const;
  virtual std::string toString() const;

private:
  std::string functionType_;
};
