// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "SymbolTable.h"

class Semantic {
public:
  static void build(SymbolTable *smanager, Ast *node);
  static void check(const SymbolTable *smanager, const Ast *node);
};
