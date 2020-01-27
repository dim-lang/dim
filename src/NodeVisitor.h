// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <memory>

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;
  virtual std::shared_ptr<Ast> visit() = 0;
};

class FunctionCallExpressionVisitor : public NodeVisitor {
public:
  FunctionCallExpressionVisitor(std::shared_ptr<Ast> node);
  virtual ~FunctionCallExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit();

private:
  std::shared_ptr<AstFunctionCallExpression> node_;
};

class UnaryExpressionVisitor : public NodeVisitor {
public:
  UnaryExpressionVisitor(std::shared_ptr<Ast> node);
  virtual ~UnaryExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit();

private:
  std::shared_ptr<AstUnaryExpression> node_;
};
