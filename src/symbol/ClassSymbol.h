// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Scope.h"
#include "Symbol.h"

class ClassSymbol : public Symbol, Scope {
public:
  ClassSymbol(const std::string &functionName);
  virtual ~ClassSymbol() = default;
  virtual std::string name() const;
  virtual std::string toString() const;

private:
  std::string functionName_;
  std::string functionType_;
};
