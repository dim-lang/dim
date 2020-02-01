// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual Ast *visit(Ast *node) const = 0;
};

class ProgramVisitor : public Visitor {
public:
  virtual ~ProgramVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  ProgramVisitor() = default;
};

class VariableDeclarationVisitor : public Visitor {
public:
  virtual ~VariableDeclarationVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  VariableDeclarationVisitor() = default;
};

class FunctionDeclarationVisitor : public Visitor {
public:
  virtual ~FunctionDeclarationVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static Visitor *instance();

private:
  FunctionDeclarationVisitor() = default;
};
