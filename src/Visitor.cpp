// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Visitor.h"
#include "Log.h"
#include "symbol/VariableSymbol.h"
#include <algorithm>

Ast *ProgramVisitor::visit(Ast *node) const {
  LOG_CHECK(node, "node is null:{}", (void *)node);
  LOG_CHECK(node->type() == A_PROGRAM, "invalid node type:{}",
            node->toString());

  AstProgram *e = dynamic_cast<AstProgram *>(node);

  // create global scope and current scope
  Scope *globalScope = new Scope("GlobalScope");
  Scope::resetGlobalScope(globalScope);
  Scope::push(globalScope);

  for (int i = 0; i < e->size(); i++) {
    Ast *p = e->get(i);
    if (!p)
      continue;
    switch (p->type()) {
    case A_VARIABLE_DECLARATION:
      VariableDeclarationVisitor::instance()->visit(p);
      break;
    case A_FUNCTION_DECLARATION:
      FunctionDeclarationVisitor::instance()->visit(p);
      break;
    default:
      LOG_CHECK(false, "invalid node type:{}", p->toString());
    }
  }
}

Visitor *ProgramVisitor::instance() {
  static ProgramVisitor *programVisitor = new ProgramVisitor();
  return programVisitor;
}

Ast *VariableDeclarationVisitor::visit(Ast *node) const {
  LOG_CHECK(node, "node is null:{}", (void *)node);

  AstVariableDeclaration *e = dynamic_cast<AstVariableDeclaration *>(node);
  AstExpressionList *identifierList = e->identifierList();
  AstExpressionList *expressionList = e->expressionList();

  LOG_CHECK(identifierList, "identifierList is null:{}",
            (void *)identifierList);
  LOG_CHECK(expressionList, "expressionList is null:{}",
            (void *)expressionList);
  LOG_CHECK(identifierList->size() == expressionList->size(),
            "identifierList expressionList same size, 1:{}, 2:{}",
            identifierList->size(), expressionList->size());
  LOG_CHECK(identifierList->size() > 0, "identifierList#size > 0:{}",
            identifierList->size());
  LOG_CHECK(expressionList->size() > 0, "expressionList#size > 0:{}",
            expressionList->size());

  for (int i = 0; i < identifierList->size(); i++) {
    AstIdentifierConstant *id =
        dynamic_cast<AstIdentifierConstant *>(identifierList->get(i));
    AstExpression *expr = expressionList->get(i);
    Scope *cs = Scope::currentScope();
    LOG_CHECK(cs, "cs is null:{}", (void *)cs);
    cs->define(new VariableSymbol(id->value()));
  }
}

Visitor *VariableDeclarationVisitor::instance() {
  static VariableDeclarationVisitor *variableDeclarationVisitor =
      new VariableDeclarationVisitor();
  return variableDeclarationVisitor;
}

Ast *FunctionDeclarationVisitor::visit(Ast *node) const {}

Visitor *FunctionDeclarationVisitor::instance() {
  static FunctionDeclarationVisitor *functionDeclarationVisitor =
      new FunctionDeclarationVisitor();
  return FunctionDeclarationVisitor;
}
