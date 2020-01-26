// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <memory>

class NodeVisitor {
public:
  NodeVisitor(std::shared_ptr<Ast> node);
  NodeVisitor(Ast *node);
  virtual ~NodeVisitor() = default;

  virtual std::shared_ptr<Ast> visit() = 0;

protected:
  std::shared_ptr<Ast> node_;
};

class FunctionCallExpressionVisitor : public NodeVisitor {
public:
  FunctionCallExpressionVisitor(std::shared_ptr<Ast> node);
  FunctionCallExpressionVisitor(Ast *node);
  virtual ~FunctionCallExpressionVisitor() = default;

  virtual std::shared_ptr<Ast> visit();
};
