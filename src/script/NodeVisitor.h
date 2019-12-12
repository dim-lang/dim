// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "script/Parser.h"

namespace fastype {

class NodeVisitor : public Stringify {
public:
  virtual ~NodeVisitor() = default;
  virtual std::string toString() const = 0;

protected:
  // general
  virtual void visit(std::shared_ptr<Ast> node) = 0;

  // program
  virtual void visitProgram(std::shared_ptr<Ast> node) = 0;
  virtual void visitStatementList(std::shared_ptr<Ast> node) = 0;

  // declaration
  virtual void visitVariableDeclaration(std::shared_ptr<Ast> node) = 0;
  virtual void visitFunctionDeclaration(std::shared_ptr<Ast> node) = 0;
  virtual void visitClassDeclaration(std::shared_ptr<Ast> node) = 0;

  // statement
  virtual void visitCompoundStatement(std::shared_ptr<Ast> node) = 0;
  virtual void visitAssignmentStatement(std::shared_ptr<Ast> node) = 0;
  virtual void visitEmptyStatement(std::shared_ptr<Ast> node) = 0;
  virtual void visitReturnStatement(std::shared_ptr<Ast> node) = 0;

  // expression
  virtual std::shared_ptr<Ast> visitExpression(std::shared_ptr<Ast> node) = 0;
  virtual std::shared_ptr<Ast> visitBinaryOp(std::shared_ptr<Ast> node) = 0;
  virtual std::shared_ptr<Ast> visitUnaryOp(std::shared_ptr<Ast> node) = 0;
  virtual std::shared_ptr<Ast> visitVariable(std::shared_ptr<Ast> node) = 0;
};

} // namespace fastype
