// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "SymbolManager.h"

class Semantic {
public:
  static void build(SymbolManager *smanager, Ast *node);
  static void check(SymbolManager *smanager, Ast *node);
};
