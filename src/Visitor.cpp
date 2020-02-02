// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Visitor.h"
#include "Log.h"
#include "Scope.h"
#include "Symbol.h"
#include <algorithm>

Ast *ProgramVisitor::visit(Ast *node) const {
  CASSERT(node, "node is null:{}", (void *)node);
  CASSERT(node->type() == A_PROGRAM, "invalid node type:{}",
            node->toString());

  AstProgram *e = dynamic_cast<AstProgram *>(node);

  // create global scope
  Scope::push(new GlobalScope());

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
      CASSERT(false, "invalid node type:{}", p->toString());
    }
  }
  return nullptr;
}

Visitor *ProgramVisitor::instance() {
  static ProgramVisitor *programVisitor = new ProgramVisitor();
  return programVisitor;
}

Ast *VariableDeclarationVisitor::visit(Ast *node) const {
  CASSERT(node, "node is null:{}", (void *)node);

  AstVariableDeclaration *e = dynamic_cast<AstVariableDeclaration *>(node);
  AstExpressionList *identifierList = e->identifierList();
  AstExpressionList *expressionList = e->expressionList();

  CASSERT(identifierList, "identifierList is null:{}",
            (void *)identifierList);
  CASSERT(expressionList, "expressionList is null:{}",
            (void *)expressionList);
  CASSERT(identifierList->size() == expressionList->size(),
            "identifierList expressionList same size, 1:{}, 2:{}",
            identifierList->size(), expressionList->size());
  CASSERT(identifierList->size() > 0, "identifierList#size > 0:{}",
            identifierList->size());
  CASSERT(expressionList->size() > 0, "expressionList#size > 0:{}",
            expressionList->size());

  for (int i = 0; i < identifierList->size(); i++) {
    AstIdentifierConstant *id =
        dynamic_cast<AstIdentifierConstant *>(identifierList->get(i));
    AstExpression *expr = expressionList->get(i);
    CASSERT(Scope::currentScope(), "Scope#currentScope is null:{}",
              (void *)Scope::currentScope());
    switch (expr->type()) {
    case A_I64_CONSTANT: {
      Scope::currentScope()->define(
          new VariableSymbol(id->value(), BuiltinTypeSymbol::i64Instance()));
      break;
    }
    case A_F64_CONSTANT: {
      Scope::currentScope()->define(
          new VariableSymbol(id->value(), BuiltinTypeSymbol::f64Instance()));
      break;
    }
    case A_STRING_CONSTANT: {
      Scope::currentScope()->define(
          new VariableSymbol(id->value(), BuiltinTypeSymbol::stringInstance()));
      break;
    }
    case A_BOOLEAN_CONSTANT: {
      Scope::currentScope()->define(new VariableSymbol(
          id->value(), BuiltinTypeSymbol::booleanInstance()));
      break;
    }
    case A_IDENTIFIER_CONSTANT: {
      break;
    }
    default:
      CASSERT(false, "invalid expression type, {}:{}", i,
                expr ? expr->toString() : "null");
    }
  }
  return nullptr;
}

Visitor *VariableDeclarationVisitor::instance() {
  static VariableDeclarationVisitor *variableDeclarationVisitor =
      new VariableDeclarationVisitor();
  return variableDeclarationVisitor;
}

Ast *FunctionDeclarationVisitor::visit(Ast *node) const {
  CASSERT(node, "node is null:{}", (void *)node);

  AstFunctionDeclaration *e = dynamic_cast<AstFunctionDeclaration *>(node);
  FunctionSymbol *funcSymbol =
      new FunctionSymbol(e->identifier(), BuiltinTypeSymbol::funcInstance(),
                         Scope::currentScope());
  Scope::push(funcSymbol);

  AstExpressionList *argumentList = e->argumentList();
  for (int i = 0; i < argumentList->size(); i++) {
    AstIdentifierConstant *argId =
        dynamic_cast<AstIdentifierConstant *>(argumentList->get(i));
    Scope::currentScope()->define(
        new VariableSymbol(argId->value(), BuiltinTypeSymbol::i64Instance()));
  }

  AstCompoundStatement *compoundStatement = e->compoundStatement();
  AstStatementList *statementList = compoundStatement->statementList();
  CASSERT(compoundStatement, "compoundStatement is null: {}",
            (void *)compoundStatement);
  CASSERT(statementList, "statementList is null: {}", (void *)statementList);
  return nullptr;
}

Visitor *FunctionDeclarationVisitor::instance() {
  static FunctionDeclarationVisitor *functionDeclarationVisitor =
      new FunctionDeclarationVisitor();
  return functionDeclarationVisitor;
}
