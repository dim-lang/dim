// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ir.h"
#include "Symbol.h"
#include "boost/core/noncopyable.hpp"

class SymbolManager : private boost::noncopyable {
public:
  SymbolManager();
  virtual ~SymbolManager();

  void push(Scope *sc);
  void pop();
  int depth() const;

  Scope *global;  // global symbol scope
  Scope *current; // current symbol scope

private:
  int depth_; // symbol stack depth
};
