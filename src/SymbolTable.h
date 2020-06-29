// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"
#include "boost/core/noncopyable.hpp"

class SymbolTable : private boost::noncopyable {
public:
  SymbolTable();
  virtual ~SymbolTable();

  void push(Scope *sc);
  void pop();
  int depth() const;

  Scope *global;  // global symbol scope
  Scope *current; // current symbol scope

private:
  int depth_; // symbol stack depth
};
