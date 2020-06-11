// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Exception.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"

#define DC(x, y) dynamic_cast<x *>(y)

void Semantic::build(SymbolManager *smanager, Ast *node) {
  EX_ASSERT(smanager, "smanager is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      build(smanager, e->get(i));
    }
  } break;
  case AstType::VariableDefinition: {
    AstVariableDefinition *e = DC(AstVariableDefinition, node);
    if (e->definitionList()) {
      for (int i = 0; i < e->definitionList()->size(); i++) {
        build(smanager, e->definitionList()->get(i));
      }
    }
  } break;
  case AstType::VariableInitialDefinition: {
    AstVariableInitialDefinition *e = DC(AstVariableInitialDefinition, node);
    VariableSymbol *varsym = new VariableSymbol(e->identifier());
    BuiltinType *varty = nullptr;
    switch (e->expression()->type()) {
    case AstType::Int64Constant:
      varty = BuiltinType::ty_int64();
      break;
    case AstType::Float64Constant:
      varty = BuiltinType::ty_float64();
      break;
    case AstType::StringConstant:
      varty = BuiltinType::ty_string();
      break;
    case AstType::BooleanConstant:
      varty = BuiltinType::ty_boolean();
      break;
    default:
      varty = BuiltinType::ty_void();
      break;
    }
    smanager->current->define(Scope::make_snode(varsym, varty, e));
  } break;
  case AstType::FunctionDefinition: {
    AstFunctionDefinition *e = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *signe = e->signature();
    FunctionSymbol *funcsym =
        new FunctionSymbol(signe->identifier(), smanager->current);
    std::vector<Type *> fargTypeList;
    EX_ASSERT(signe->argumentList(), "signe->argumentList is null");
    for (int i = 0; i < signe->argumentList()->size(); i++) {
      AstFunctionArgumentDefinition *arge =
          DC(AstFunctionArgumentDefinition, signe->argumentList()->get(i));
      EX_ASSERT(arge, "arge is null");
      fargTypeList.push_back(BuiltinType::ty_void());
    }
    FunctionType *functy =
        new FunctionType(fargTypeList, BuiltinType::ty_void());
    smanager->current->define(Scope::make_snode(funcsym, functy, e));
    smanager->push(funcsym);
    if (signe->argumentList()) {
      for (int i = 0; i < signe->argumentList()->size(); i++) {
        build(smanager, signe->argumentList()->get(i));
      }
    }
    EX_ASSERT(e->statement(), "e#statement is null");
    build(smanager, e->statement());
    smanager->pop();
  } break;
  case AstType::FunctionArgumentDefinition: {
    AstFunctionArgumentDefinition *e = DC(AstFunctionArgumentDefinition, node);
    FunctionArgumentSymbol *fargsym =
        new FunctionArgumentSymbol(e->identifier());
    smanager->current->define(
        Scope::make_snode(fargsym, BuiltinType::ty_void(), e));
  } break;
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    LocalScope *loc = new LocalScope(e->name(), smanager->current);
    smanager->current->define(Scope::make_snode(loc, ScopeType::ty_local(), e));
    smanager->push(loc);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        build(smanager, e->statementList()->get(i));
      }
    }
    smanager->pop();
  } break;
  case AstType::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    build(smanager, e->condition());
    build(smanager, e->thens());
    build(smanager, e->elses());
  } break;
  case AstType::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    EX_ASSERT(e->statement(), "e->statement is null");
    build(smanager, e->statement());
  } break;
  case AstType::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    LocalScope *loc = new LocalScope(e->name(), smanager->current);
    smanager->current->define(Scope::make_snode(loc, ScopeType::ty_local(), e));
    smanager->push(loc);
    build(smanager, e->start());
    build(smanager, e->step());
    build(smanager, e->end());
    build(smanager, e->statement());
    smanager->pop();
  } break;
  case AstType::ReturnStatement: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    build(smanager, e->expression());
  } break;
  case AstType::ExpressionStatement: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    build(smanager, e->expression());
  } break;
  default:
    /* EX_ASSERT(false, "invalid node:{}", node->toString()); */
    LOG_INFO("do nothing for node:{}", node->toString());
    break;
  }
}

void Semantic::check(SymbolManager *smanager, Ast *node) {
  EX_ASSERT(smanager, "smanager is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(smanager, e->get(i));
    }
  } break;
  case AstType::IdentifierConstant: {
    AstIdentifierConstant *e = DC(AstIdentifierConstant, node);
    Scope::SNode snode = smanager->current->resolve(e->value());
    EX_ASSERT(Scope::sym(snode) && Scope::ty(snode) && Scope::ast(snode),
              "sematic check failure: identifier symbol {} not found",
              e->value());
  } break;
  case AstType::CallExpression: {
    AstCallExpression *e = DC(AstCallExpression, node);
    Scope::SNode snode = smanager->current->resolve(e->identifier());
    EX_ASSERT(Scope::sym(snode) && Scope::ty(snode) && Scope::ast(snode),
              "sematic check failure: function symbol {} not found",
              e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        AstFunctionArgumentDefinition *farge =
            DC(AstFunctionArgumentDefinition, e->argumentList()->get(i));
        Scope::SNode argnode = smanager->current->resolve(farge->identifier());
        EX_ASSERT(
            Scope::sym(argnode) && Scope::ty(argnode) && Scope::ast(argnode),
            "sematic check failure: function argument symbol {} not found",
            farge->identifier());
      }
    }
  } break;
  case AstType::UnaryExpression: {
    AstUnaryExpression *e = DC(AstUnaryExpression, node);
    check(smanager, e->expression());
  } break;
  case AstType::BinaryExpression: {
    AstBinaryExpression *e = DC(AstBinaryExpression, node);
    check(smanager, e->left());
    check(smanager, e->right());
  } break;
  case AstType::ConditionalExpression: {
    AstConditionalExpression *e = DC(AstConditionalExpression, node);
    check(smanager, e->condition());
    check(smanager, e->thens());
    check(smanager, e->elses());
  } break;
  case AstType::AssignmentExpression: {
    AstAssignmentExpression *e = DC(AstAssignmentExpression, node);
    check(smanager, e->variable());
    check(smanager, e->value());
  } break;
  case AstType::SequelExpression: {
    AstSequelExpression *e = DC(AstSequelExpression, node);
    if (e->expressionList()) {
      for (int i = 0; i < e->expressionList()->size(); i++) {
        check(smanager, e->expressionList()->get(i));
      }
    }
  } break;
  case AstType::ExpressionStatement: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    check(smanager, e->expression());
  } break;
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        check(smanager, e->statementList()->get(i));
      }
    }
  } break;
  case AstType::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    check(smanager, e->condition());
    check(smanager, e->thens());
    check(smanager, e->elses());
  } break;
  case AstType::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    check(smanager, e->condition());
    check(smanager, e->statement());
  } break;
  case AstType::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    check(smanager, e->start());
    check(smanager, e->step());
    check(smanager, e->end());
    check(smanager, e->statement());
  } break;
  case AstType::ReturnStatement: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    check(smanager, e->expression());
  } break;
  case AstType::VariableDefinition:
  case AstType::VariableInitialDefinition:
  case AstType::FunctionDefinition:
  case AstType::FunctionSignatureDefinition:
  case AstType::FunctionArgumentDefinition:
    break;
  default:
    EX_ASSERT(false, "invalid node: {}", node->toString());
    /* LOG_INFO("do nothing for node:{}", node->toString()); */
    break;
  }
}
