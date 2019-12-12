// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Hasher.h"
#include "Stringify.h"
#include "config/Header.h"
#include "script/NodeVisitor.h"
#include "script/Symbol.h"
#include "script/SymbolTable.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace fastype {

class SymbolTableBuilder : public NodeVisitor {
public:
  SymbolTableBuilder();
  virtual ~SymbolTableBuilder() = default;
  virtual void travel();
  virtual std::string toString() const = 0;

private:
  virtual void visit(std::shared_ptr<Ast> node);

  virtual void visitProgram(std::shared_ptr<Ast> node);
  virtual void visitStatementList(std::shared_ptr<Ast> node);

  virtual void visitVariableDeclaration(std::shared_ptr<Ast> node);
  virtual void visitFunctionDeclaration(std::shared_ptr<Ast> node);
  virtual void visitClassDeclaration(std::shared_ptr<Ast> node);

  virtual void visitCompoundStatement(std::shared_ptr<Ast> node);
  virtual void visitAssignmentStatement(std::shared_ptr<Ast> node);
  virtual void visitEmptyStatement(std::shared_ptr<Ast> node);
  virtual void visitReturnStatement(std::shared_ptr<Ast> node);

  virtual std::shared_ptr<Ast> visitExpression(std::shared_ptr<Ast> node);
  virtual std::shared_ptr<Ast> visitBinaryOp(std::shared_ptr<Ast> node);
  virtual std::shared_ptr<Ast> visitUnaryOp(std::shared_ptr<Ast> node);
  virtual std::shared_ptr<Ast> visitVariable(std::shared_ptr<Ast> node);

  SymbolTable symbolTable_;
};

} // namespace fastype
