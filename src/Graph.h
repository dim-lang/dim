// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
class Ast;

class Graph {
public:
  static int drawAst(Ast *ast, const Cowstr &output);
};
