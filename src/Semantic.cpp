// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Exception.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"

#define DC(x, y) dynamic_cast<x *>(y)
#define DCC(x, y) dynamic_cast<const x *>(y)

void Semantic::build(SymbolTable *symtab, Ast *node) {
  EX_ASSERT(symtab, "symtab is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      build(symtab, e->get(i));
    }
  } break;
  case AstType::VariableDefinition: {
    AstVariableDefinition *e = DC(AstVariableDefinition, node);
    if (e->definitionList()) {
      for (int i = 0; i < e->definitionList()->size(); i++) {
        build(symtab, e->definitionList()->get(i));
      }
    }
  } break;
  case AstType::VariableInitialDefinition: {
    AstVariableInitialDefinition *e = DC(AstVariableInitialDefinition, node);
    VariableSymbol *varsym =
        new VariableSymbol(e->identifier(), symtab->current);
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
    symtab->current->define(Scope::make_snode(varsym, varty, e));
  } break;
  case AstType::FunctionDefinition: {
    AstFunctionDefinition *e = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *signe = e->signature();
    FunctionSymbol *funcsym =
        new FunctionSymbol(signe->identifier(), symtab->current);
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
    symtab->current->define(Scope::make_snode(funcsym, functy, e));
    symtab->push(funcsym);
    if (signe->argumentList()) {
      for (int i = 0; i < signe->argumentList()->size(); i++) {
        build(symtab, signe->argumentList()->get(i));
      }
    }
    EX_ASSERT(e->statement(), "e#statement is null");
    build(symtab, e->statement());
    symtab->pop();
  } break;
  case AstType::FunctionArgumentDefinition: {
    AstFunctionArgumentDefinition *e = DC(AstFunctionArgumentDefinition, node);
    FunctionArgumentSymbol *fargsym =
        new FunctionArgumentSymbol(e->identifier(), symtab->current);
    symtab->current->define(
        Scope::make_snode(fargsym, BuiltinType::ty_void(), e));
  } break;
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    LocalScope *loc = new LocalScope(e->name(), symtab->current);
    symtab->current->define(Scope::make_snode(loc, ScopeType::ty_local(), e));
    symtab->push(loc);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        build(symtab, e->statementList()->get(i));
      }
    }
    symtab->pop();
  } break;
  case AstType::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    build(symtab, e->condition());
    build(symtab, e->thens());
    build(symtab, e->elses());
  } break;
  case AstType::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    EX_ASSERT(e->statement(), "e->statement is null");
    build(symtab, e->statement());
  } break;
  case AstType::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    LocalScope *loc = new LocalScope(e->name(), symtab->current);
    symtab->current->define(Scope::make_snode(loc, ScopeType::ty_local(), e));
    symtab->push(loc);
    build(symtab, e->start());
    build(symtab, e->step());
    build(symtab, e->end());
    build(symtab, e->statement());
    symtab->pop();
  } break;
  case AstType::ReturnStatement: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    build(symtab, e->expression());
  } break;
  case AstType::ExpressionStatement: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    build(symtab, e->expression());
  } break;
  default:
    /* EX_ASSERT(false, "invalid node:{}", node->toString()); */
    LOG_INFO("do nothing for node:{}", node->toString());
    break;
  }
}

void Semantic::check(const SymbolTable *symtab, const Ast *node) {
  EX_ASSERT(symtab, "symtab is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    const AstTranslateUnit *e = DCC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(symtab, e->get(i));
    }
  } break;
  case AstType::IdentifierConstant: {
    const AstIdentifierConstant *e = DCC(AstIdentifierConstant, node);
    Scope::SNode snode = symtab->current->resolve(e->value());
    EX_ASSERT(Scope::s(snode) && Scope::t(snode) && Scope::a(snode),
              "sematic check failure: identifier symbol {} not found",
              e->value());
  } break;
  case AstType::CallExpression: {
    const AstCallExpression *e = DCC(AstCallExpression, node);
    Scope::SNode snode = symtab->current->resolve(e->identifier());
    EX_ASSERT(Scope::s(snode) && Scope::t(snode) && Scope::a(snode),
              "sematic check failure: function symbol {} not found",
              e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        const AstFunctionArgumentDefinition *farge =
            DCC(AstFunctionArgumentDefinition, e->argumentList()->get(i));
        Scope::SNode argnode = symtab->current->resolve(farge->identifier());
        EX_ASSERT(
            Scope::s(argnode) && Scope::t(argnode) && Scope::a(argnode),
            "sematic check failure: function argument symbol {} not found",
            farge->identifier());
      }
    }
  } break;
  case AstType::UnaryExpression: {
    const AstUnaryExpression *e = DCC(AstUnaryExpression, node);
    check(symtab, e->expression());
  } break;
  case AstType::BinaryExpression: {
    const AstBinaryExpression *e = DCC(AstBinaryExpression, node);
    check(symtab, e->left());
    check(symtab, e->right());
  } break;
  case AstType::ConditionalExpression: {
    const AstConditionalExpression *e = DCC(AstConditionalExpression, node);
    check(symtab, e->condition());
    check(symtab, e->thens());
    check(symtab, e->elses());
  } break;
  case AstType::AssignmentExpression: {
    const AstAssignmentExpression *e = DCC(AstAssignmentExpression, node);
    check(symtab, e->variable());
    check(symtab, e->value());
  } break;
  case AstType::SequelExpression: {
    const AstSequelExpression *e = DCC(AstSequelExpression, node);
    if (e->expressionList()) {
      for (int i = 0; i < e->expressionList()->size(); i++) {
        check(symtab, e->expressionList()->get(i));
      }
    }
  } break;
  case AstType::ExpressionStatement: {
    const AstExpressionStatement *e = DCC(AstExpressionStatement, node);
    check(symtab, e->expression());
  } break;
  case AstType::CompoundStatement: {
    const AstCompoundStatement *e = DCC(AstCompoundStatement, node);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        check(symtab, e->statementList()->get(i));
      }
    }
  } break;
  case AstType::IfStatement: {
    const AstIfStatement *e = DCC(AstIfStatement, node);
    check(symtab, e->condition());
    check(symtab, e->thens());
    check(symtab, e->elses());
  } break;
  case AstType::WhileStatement: {
    const AstWhileStatement *e = DCC(AstWhileStatement, node);
    check(symtab, e->condition());
    check(symtab, e->statement());
  } break;
  case AstType::ForStatement: {
    const AstForStatement *e = DCC(AstForStatement, node);
    check(symtab, e->start());
    check(symtab, e->step());
    check(symtab, e->end());
    check(symtab, e->statement());
  } break;
  case AstType::ReturnStatement: {
    const AstReturnStatement *e = DCC(AstReturnStatement, node);
    check(symtab, e->expression());
  } break;
  case AstType::VariableDefinition:
  case AstType::VariableInitialDefinition:
  case AstType::FunctionDefinition:
  case AstType::FunctionSignatureDefinition:
  case AstType::FunctionArgumentDefinition:
    break;
  default:
    EX_ASSERT(false, "invalid node: {}", node->toString());
    break;
  }
}
