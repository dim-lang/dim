// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "NodeVisitor.h"
#include <memory>

class ExpressionVisitor : public NodeVisitor {
public:
  virtual ~ExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  ExpressionVisitor() = default;
};

class PrimaryExpressionVisitor : public NodeVisitor {
public:
  virtual ~PrimaryExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  PrimaryExpressionVisitor() = default;
};

class FunctionCallExpressionVisitor : public NodeVisitor {
public:
  virtual ~FunctionCallExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  FunctionCallExpressionVisitor() = default;
};

class UnaryExpressionVisitor : public NodeVisitor {
public:
  virtual ~UnaryExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  UnaryExpressionVisitor() = default;
};

class BinaryExpressionVisitor : public NodeVisitor {
public:
  virtual ~BinaryExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  BinaryExpressionVisitor();
};

class ConditionalExpressionVisitor : public NodeVisitor {
public:
  virtual ~ConditionalExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  ConditionalExpressionVisitor() = default;
};

class AssignmentExpressionVisitor : public NodeVisitor {
public:
  virtual ~AssignmentExpressionVisitor() = default;
  virtual std::shared_ptr<Ast> visit(std::shared_ptr<Ast> node);
  static std::shared_ptr<NodeVisitor> instance();

private:
  AssignmentExpressionVisitor() = default;
};
