// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual Ast *visit(Ast *node) const = 0;
};

class ExpressionVisitor : public Visitor {
public:
  virtual ~ExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  ExpressionVisitor() = default;
};

class PrimaryExpressionVisitor : public Visitor {
public:
  virtual ~PrimaryExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  PrimaryExpressionVisitor() = default;
};

class FunctionCallExpressionVisitor : public Visitor {
public:
  virtual ~FunctionCallExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  FunctionCallExpressionVisitor() = default;
};

class UnaryExpressionVisitor : public Visitor {
public:
  virtual ~UnaryExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  UnaryExpressionVisitor() = default;
};

class BinaryExpressionVisitor : public Visitor {
public:
  virtual ~BinaryExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  BinaryExpressionVisitor();
};

class ConditionalExpressionVisitor : public Visitor {
public:
  virtual ~ConditionalExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  ConditionalExpressionVisitor() = default;
};

class AssignmentExpressionVisitor : public Visitor {
public:
  virtual ~AssignmentExpressionVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  AssignmentExpressionVisitor() = default;
};
