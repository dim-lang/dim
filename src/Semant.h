// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"

class AstVisitor {
public:
  virtual ~AstVisitor() = default;
  virtual Ast *visit(Ast *node) const = 0;
};

class ProgramVisitor : public AstVisitor {
public:
  virtual ~ProgramVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static AstVisitor *instance();

private:
  ProgramVisitor() = default;
};

class VariableDeclarationVisitor : public AstVisitor {
public:
  virtual ~VariableDeclarationVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static AstVisitor *instance();

private:
  VariableDeclarationVisitor() = default;
};

class FunctionDeclarationVisitor : public AstVisitor {
public:
  virtual ~FunctionDeclarationVisitor() = default;
  virtual Ast *visit(Ast *node) const;
  static AstVisitor *instance();

private:
  FunctionDeclarationVisitor() = default;
};

class Semant {
public:
  Semant();
  virtual ~Semant();
  virtual void build(Ast *node);
  virtual void check(Ast *node);

private:
  Symtab *global_;
  Symtab *current_;
};
