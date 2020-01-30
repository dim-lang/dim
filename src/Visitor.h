// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual Ast *visit(Ast *node) const = 0;
};
