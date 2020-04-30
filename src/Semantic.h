// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Type.h"

class Semantic {
public:
  static void build(SymbolTable *symtable, Ast *node);
  static void check(SymbolTable *symtable, Ast *node);
};
