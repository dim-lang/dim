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
void Semantic::build(SymbolTable *symtable, Ast *node) {
  LOG_ASSERT(symtable, "symtable is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      build(symtable, e->get(i));
    }
  } break;
  case AstType::VariableDefinition: {
    AstVariableDefinition *e = DC(AstVariableDefinition, node);
    if (e->definitionList()) {
      for (int i = 0; i < e->definitionList()->size(); i++) {
        build(symtable, e->definitionList()->get(i));
      }
    }
  } break;
  case AstType::VariableInitialDefinition: {
    AstVariableInitialDefinition *e = DC(AstVariableInitialDefinition, node);
    VariableSymbol *vs = new VariableSymbol(e->identifier());
    symtable->css()->define(vs);
    switch (e->expression()->type()) {
    case AstType::Int64Constant:
      symtable->cts()->define(vs, BuiltinType::ty_i64());
      break;
    case AstType::Float64Constant:
      symtable->cts()->define(vs, BuiltinType::ty_f64());
      break;
    case AstType::StringConstant:
      symtable->cts()->define(vs, BuiltinType::ty_string());
      break;
    case AstType::BooleanConstant:
      symtable->cts()->define(vs, BuiltinType::ty_boolean());
      break;
    default:
      symtable->cts()->define(vs, BuiltinType::ty_void());
      break;
    }
  } break;
  case AstType::FunctionDefinition: {
    AstFunctionDefinition *e = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *e1 = e->signature();
    FunctionSymbol *fs = new FunctionSymbol(e1->identifier(), symtable->css());
    std::vector<std::pair<Symbol *, Type *>> argumentTypeList;
    if (e1->argumentList()) {
      for (int i = 0; i < e1->argumentList()->size(); i++) {
        AstFunctionArgumentDefinition *fa =
            DC(AstFunctionArgumentDefinition, e1->argumentList()->get(i));
        argumentTypeList.push_back(
            std::make_pair(new FunctionArgumentSymbol(fa->identifier()),
                           BuiltinType::ty_void()));
      }
    }
    FunctionType *ft = new FunctionType(
        argumentTypeList, BuiltinType::ty_void(), symtable->cts());
    symtable->css()->define(fs);
    symtable->cts()->define(fs, ft);
    symtable->pushSymbol(fs);
    symtable->pushType(ft);
    if (e1->argumentList()) {
      for (int i = 0; i < e1->argumentList()->size(); i++) {
        build(symtable, e1->argumentList()->get(i));
      }
    }
    LOG_ASSERT(e->statement(), "e#statement is null");
    build(symtable, e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case AstType::FunctionArgumentDefinition: {
    AstFunctionArgumentDefinition *e = DC(AstFunctionArgumentDefinition, node);
    FunctionArgumentSymbol *fa = new FunctionArgumentSymbol(e->identifier());
    symtable->css()->define(fa);
    symtable->cts()->define(fa, BuiltinType::ty_void());
  } break;
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
    symtable->css()->define(ls);
    symtable->cts()->define(ls, lt);
    symtable->pushSymbol(ls);
    symtable->pushType(lt);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        build(symtable, e->statementList()->get(i));
      }
    }
    symtable->popSymbol();
    symtable->popType();
  } break;
  case AstType::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    if (e->condition()) {
      build(symtable, e->condition());
    }
    if (e->thens()) {
      build(symtable, e->thens());
    }
    if (e->elses()) {
      build(symtable, e->elses());
    }
  } break;
  case AstType::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    LOG_ASSERT(e->statement(), "e->statement is null");
    build(symtable, e->statement());
  } break;
  case AstType::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
    symtable->css()->define(ls);
    symtable->cts()->define(ls, lt);
    symtable->pushSymbol(ls);
    symtable->pushType(lt);
    build(symtable, e->start());
    build(symtable, e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case AstType::ReturnStatement: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    if (e->expression()) {
      build(symtable, e->expression());
    }
  } break;
  case AstType::ExpressionStatement: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    if (e->expression()) {
      build(symtable, e->expression());
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
void Semantic::check(SymbolTable *symtable, Ast *node) {
  LOG_ASSERT(symtable, "symtable is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(symtable, e->get(i));
    }
  } break;
  case AstType::IdentifierConstant: {
    AstIdentifierConstant *e = DC(AstIdentifierConstant, node);
    Symbol *s = symtable->css()->resolve(e->value());
    LOG_ASSERT(s, "sematic check failure: symbol {} not found", e->value());
  } break;
  case AstType::CallExpression: {
    AstCallExpression *e = DC(AstCallExpression, node);
    Symbol *fs = symtable->css()->resolve(e->identifier());
    LOG_ASSERT(fs, "sematic check failure: function symbol {} not found",
               e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        AstFunctionArgumentDefinition *fad =
            DC(AstFunctionArgumentDefinition, e->argumentList()->get(i));
        Symbol *fas = symtable->css()->resolve(fad->identifier());
        LOG_ASSERT(
            fas, "sematic check failure: function argument symbol {} not found",
            fad->identifier());
      }
    }
  } break;
  case AstType::UnaryExpression: {
    AstUnaryExpression *e = DC(AstUnaryExpression, node);
    check(symtable, e->expression());
  } break;
  case AstType::BinaryExpression: {
    AstBinaryExpression *e = DC(AstBinaryExpression, node);
    check(symtable, e->left());
    check(symtable, e->right());
  } break;
  case AstType::ConditionalExpression: {
    AstConditionalExpression *e = DC(AstConditionalExpression, node);
    check(symtable, e->condition());
    check(symtable, e->thens());
    check(symtable, e->elses());
  } break;
  case AstType::AssignmentExpression: {
    AstAssignmentExpression *e = DC(AstAssignmentExpression, node);
    check(symtable, e->variable());
    check(symtable, e->value());
  } break;
  case AstType::SequelExpression: {
    AstSequelExpression *e = DC(AstSequelExpression, node);
    if (e->expressionList()) {
      for (int i = 0; i < e->expressionList()->size(); i++) {
        check(symtable, e->expressionList()->get(i));
      }
    }
  } break;
  case AstType::ExpressionStatement: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    check(symtable, e->expression());
  } break;
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        check(symtable, e->statementList()->get(i));
      }
    }
  } break;
  case AstType::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    check(symtable, e->condition());
    check(symtable, e->thens());
    if (e->elses()) {
      check(symtable, e->elses());
    }
  } break;
  case AstType::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    check(symtable, e->condition());
    check(symtable, e->statement());
  } break;
  case AstType::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    check(symtable, e->start());
    check(symtable, e->step());
    check(symtable, e->end());
    check(symtable, e->statement());
  } break;
  case AstType::ReturnStatement: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    check(symtable, e->expression());
  } break;
  default:
    LOG_INFO("do nothing for node: {}", node->toString());
    break;
  }
}

#undef DC
