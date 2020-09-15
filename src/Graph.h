// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "container/Cowstr.h"
#include <memory>
class Ast;
class Symbol;

class Graph {
public:
  static int drawAst(Ast *ast, const Cowstr &output);
  static int drawSymbol(std::shared_ptr<Symbol> symbol, const Cowstr &output);
};
