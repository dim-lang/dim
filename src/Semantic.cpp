// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"

#define DC(x, y) dynamic_cast<x *>(y)

/**
 * A_TRANSLATE_UNIT
 *
 * A_VARIABLE_DECLARATION
 * A_VARIABLE_ASSIGNMENT_DECLARATION
 * A_FUNCTION_DECLARATION
 * A_FUNCTION_ARGUMENT_DECLARATION
 *
 * A_COMPOUND_STATEMENT
 * A_IF_STATEMENT
 * A_WHILE_STATEMENT
 * A_FOR_STATEMENT
 * A_RETURN_STATEMENT
 * A_EXPRESSION_STATEMENT
 */
void Semantic::build(SymbolManager *smanager, Ast *node) {
  LOG_ASSERT(smanager, "smanager is null");
  LOG_ASSERT(node, "node is null");
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
    smanager->current->define(SymNode(varsym, varty, e));
  } break;
  case AstType::FunctionDefinition: {
    AstFunctionDefinition *e = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *signe = e->signature();
    FunctionSymbol *funcsym =
        new FunctionSymbol(signe->identifier(), smanager->current);
    std::vector<Type *> fargTypeList;
    LOG_ASSERT(signe->argumentList(), "signe->argumentList is null");
    for (int i = 0; i < signe->argumentList()->size(); i++) {
      AstFunctionArgumentDefinition *arge =
          DC(AstFunctionArgumentDefinition, signe->argumentList()->get(i));
      LOG_ASSERT(arge, "arge is null");
      fargTypeList.push_back(BuiltinType.ty_void());
    }
    FunctionType *ft = new FunctionType(fargTypeList, BuiltinType::ty_void(),
                                        smanager->current);
    smanager->css()->define(fs);
    smanager->cts()->define(fs, ft);
    smanager->pushSymbol(fs);
    smanager->pushType(ft);
    if (e1->argumentList()) {
      for (int i = 0; i < e1->argumentList()->size(); i++) {
        build(smanager, e1->argumentList()->get(i));
      }
    }
    LOG_ASSERT(e->statement(), "e#statement is null");
    build(smanager, e->statement());
    smanager->popSymbol();
    smanager->popType();
  } break;
  case AstType::FunctionArgumentDefinition: {
    AstFunctionArgumentDefinition *e = DC(AstFunctionArgumentDefinition, node);
    FunctionArgumentSymbol *fa = new FunctionArgumentSymbol(e->identifier());
    smanager->css()->define(fa);
    smanager->cts()->define(fa, BuiltinType::ty_void());
  } break;
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), smanager->css());
    LocalTytab *lt = new LocalTytab(e->name(), smanager->cts());
    smanager->css()->define(ls);
    smanager->cts()->define(ls, lt);
    smanager->pushSymbol(ls);
    smanager->pushType(lt);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        build(smanager, e->statementList()->get(i));
      }
    }
    smanager->popSymbol();
    smanager->popType();
  } break;
  case AstType::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    if (e->condition()) {
      build(smanager, e->condition());
    }
    if (e->thens()) {
      build(smanager, e->thens());
    }
    if (e->elses()) {
      build(smanager, e->elses());
    }
  } break;
  case AstType::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    LOG_ASSERT(e->statement(), "e->statement is null");
    build(smanager, e->statement());
  } break;
  case AstType::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), smanager->css());
    LocalTytab *lt = new LocalTytab(e->name(), smanager->cts());
    smanager->css()->define(ls);
    smanager->cts()->define(ls, lt);
    smanager->pushSymbol(ls);
    smanager->pushType(lt);
    build(smanager, e->start());
    build(smanager, e->statement());
    smanager->popSymbol();
    smanager->popType();
  } break;
  case AstType::ReturnStatement: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    if (e->expression()) {
      build(smanager, e->expression());
    }
  } break;
  case AstType::ExpressionStatement: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    if (e->expression()) {
      build(smanager, e->expression());
    }
  } break;
  default:
    LOG_INFO("do nothing for node:{}", node->toString());
    break;
  }
}

/**
 * A_TRANSLATE_UNIT
 *
 * A_IDENTIFIER_CONSTANT
 * A_CALL_EXPRESSION
 * A_UNARY_EXPRESSION
 * A_BINARY_EXPRESSION
 * A_CONDITIONAL_EXPRESSION
 * A_ASSIGNMENT_EXPRESSION
 * A_SEQUEL_EXPERSSION
 *
 * A_EXPRESSION_STATEMENT
 * A_COMPOUND_STATEMENT
 * A_IF_STATEMENT
 * A_WHILE_STATEMENT
 * A_FOR_STATEMENT
 * A_RETURN_STATEMENT
 */
void Semantic::check(SymbolManager *smanager, Ast *node) {
  LOG_ASSERT(smanager, "smanager is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(smanager, e->get(i));
    }
  } break;
  case AstType::IdentifierConstant: {
    AstIdentifierConstant *e = DC(AstIdentifierConstant, node);
    Symbol *s = smanager->css()->resolve(e->value());
    LOG_ASSERT(s, "sematic check failure: symbol {} not found", e->value());
  } break;
  case AstType::CallExpression: {
    AstCallExpression *e = DC(AstCallExpression, node);
    Symbol *fs = smanager->css()->resolve(e->identifier());
    LOG_ASSERT(fs, "sematic check failure: function symbol {} not found",
               e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        AstFunctionArgumentDefinition *fad =
            DC(AstFunctionArgumentDefinition, e->argumentList()->get(i));
        Symbol *fas = smanager->css()->resolve(fad->identifier());
        LOG_ASSERT(
            fas, "sematic check failure: function argument symbol {} not found",
            fad->identifier());
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
    if (e->elses()) {
      check(smanager, e->elses());
    }
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
  default:
    LOG_INFO("do nothing for node: {}", node->toString());
    break;
  }
}

#undef DC
