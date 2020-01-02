// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Hasher.h"
#include "Stringify.h"
#include "config/Header.h"
#include "script/NodeVisitor.h"
#include "script/Parser.h"
#include "script/ast/Scope.h"
#include <unordered_map>

namespace fastype {

class Interpreter : public NodeVisitor {
public:
  Interpreter(std::shared_ptr<Parser> parser);
  virtual ~Interpreter();
  virtual void interpret();
  virtual std::string toString() const;

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

  std::shared_ptr<Ast> tree_;
  std::shared_ptr<Parser> parser_;
  // std::unordered_map<icu::UnicodeString, std::shared_ptr<Ast>> globalScope_;
  std::shared_ptr<Scope> scope_;
};

} // namespace fastype
