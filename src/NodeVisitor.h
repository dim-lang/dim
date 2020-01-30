// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <memory>

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node) = 0;
};
