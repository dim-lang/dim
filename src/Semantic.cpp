// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"

#define DC(x, y) dynamic_cast<const x *>(y)

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
void Semantic::build(SymbolTable *symtable, const Ast *node) {
  LOG_ASSERT(symtable, "symtable is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TUNIT: {
    const AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      build(symtable, e->get(i));
    }
  } break;
  case AstType::VAR_DECL: {
    const AstVariableDeclaration *e = DC(AstVariableDeclaration, node);
    if (e->declarationList()) {
      for (int i = 0; i < e->declarationList()->size(); i++) {
        build(symtable, e->declarationList()->get(i));
      }
    }
  } break;
  case AstType::VAR_ASSIGN_DECL: {
    const AstVariableAssignmentDeclaration *e =
        DC(AstVariableAssignmentDeclaration, node);
    VariableSymbol *vs = new VariableSymbol(e->identifier());
    symtable->css()->define(vs);
    switch (e->expression()->type()) {
    case AstType::I64_CONST:
      symtable->cts()->define(vs, BuiltinType::ty_i64());
      break;
    case AstType::F64_CONST:
      symtable->cts()->define(vs, BuiltinType::ty_f64());
      break;
    case AstType::STR_CONST:
      symtable->cts()->define(vs, BuiltinType::ty_string());
      break;
    case AstType::BOOL_CONST:
      symtable->cts()->define(vs, BuiltinType::ty_boolean());
      break;
    default:
      symtable->cts()->define(vs, BuiltinType::ty_void());
      break;
    }
  } break;
  case AstType::FUNC_DECL: {
    const AstFunctionDeclaration *e = DC(AstFunctionDeclaration, node);
    FunctionSymbol *fs = new FunctionSymbol(e->identifier(), symtable->css());
    std::vector<std::pair<Symbol *, Type *>> argumentTypeList;
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        const AstFunctionArgumentDeclaration *fa =
            DC(AstFunctionArgumentDeclaration, e->argumentList()->get(i));
        argumentTypeList.push_back(std::make_pair(
            new FunctionArgumentSymbol(fa->value()), BuiltinType::ty_void()));
      }
    }
    FunctionType *ft = new FunctionType(
        argumentTypeList, BuiltinType::ty_void(), symtable->cts());
    symtable->css()->define(fs);
    symtable->cts()->define(fs, ft);
    symtable->pushSymbol(fs);
    symtable->pushType(ft);
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        build(symtable, e->argumentList()->get(i));
      }
    }
    LOG_ASSERT(e->statement(), "e#statement is null");
    build(symtable, e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case AstType::FUNC_ARG_DECL: {
    const AstFunctionArgumentDeclaration *e =
        DC(AstFunctionArgumentDeclaration, node);
    FunctionArgumentSymbol *fa = new FunctionArgumentSymbol(e->value());
    symtable->css()->define(fa);
    symtable->cts()->define(fa, BuiltinType::ty_void());
  } break;
  case AstType::COMP_STMT: {
    const AstCompoundStatement *e = DC(AstCompoundStatement, node);
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
  case AstType::IF_STMT: {
    const AstIfStatement *e = DC(AstIfStatement, node);
    if (e->condition()) {
      build(symtable, e->condition());
    }
    if (e->hit()) {
      build(symtable, e->hit());
    }
    if (e->miss()) {
      build(symtable, e->miss());
    }
  } break;
  case AstType::WHILE_STMT: {
    const AstWhileStatement *e = DC(AstWhileStatement, node);
    LOG_ASSERT(e->statement(), "e->statement is null");
    build(symtable, e->statement());
  } break;
  case AstType::FOR_STMT: {
    const AstForStatement *e = DC(AstForStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
    symtable->css()->define(ls);
    symtable->cts()->define(ls, lt);
    symtable->pushSymbol(ls);
    symtable->pushType(lt);
    build(symtable, e->initial());
    build(symtable, e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case AstType::RET_STMT: {
    const AstReturnStatement *e = DC(AstReturnStatement, node);
    if (e->expression()) {
      build(symtable, e->expression());
    }
  } break;
  case AstType::EXPR_STMT: {
    const AstExpressionStatement *e = DC(AstExpressionStatement, node);
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
void Semantic::check(SymbolTable *symtable, const Ast *node) {
  LOG_ASSERT(symtable, "symtable is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::TUNIT: {
    const AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(symtable, e->get(i));
    }
  } break;
  case AstType::ID_CONST: {
    const AstIdentifierConstant *e = DC(AstIdentifierConstant, node);
    Symbol *s = symtable->css()->resolve(e->value());
    LOG_ASSERT(s, "sematic check failure: symbol {} not found", e->value());
  } break;
  case AstType::CALL_EXPR: {
    const AstCallExpression *e = DC(AstCallExpression, node);
    Symbol *fs = symtable->css()->resolve(e->identifier());
    LOG_ASSERT(fs, "sematic check failure: function symbol {} not found",
               e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        const AstFunctionArgumentDeclaration *fad =
            DC(AstFunctionArgumentDeclaration, e->argumentList()->get(i));
        Symbol *fas = symtable->css()->resolve(fad->value());
        LOG_ASSERT(
            fas, "sematic check failure: function argument symbol {} not found",
            fad->value());
      }
    }
  } break;
  case AstType::UNA_EXPR: {
    const AstUnaryExpression *e = DC(AstUnaryExpression, node);
    check(symtable, e->expression());
  } break;
  case AstType::BIN_EXPR: {
    const AstBinaryExpression *e = DC(AstBinaryExpression, node);
    check(symtable, e->left());
    check(symtable, e->right());
  } break;
  case AstType::COND_EXPR: {
    const AstConditionalExpression *e = DC(AstConditionalExpression, node);
    check(symtable, e->condition());
    check(symtable, e->hit());
    check(symtable, e->miss());
  } break;
  case AstType::ASSIGN_EXPR: {
    const AstAssignmentExpression *e = DC(AstAssignmentExpression, node);
    check(symtable, e->variable());
    check(symtable, e->value());
  } break;
  case AstType::SEQ_EXPR: {
    const AstSequelExpression *e = DC(AstSequelExpression, node);
    if (e->expressionList()) {
      for (int i = 0; i < e->expressionList()->size(); i++) {
        check(symtable, e->expressionList()->get(i));
      }
    }
  } break;
  case AstType::EXPR_STMT: {
    const AstExpressionStatement *e = DC(AstExpressionStatement, node);
    check(symtable, e->expression());
  } break;
  case AstType::COMP_STMT: {
    const AstCompoundStatement *e = DC(AstCompoundStatement, node);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        check(symtable, e->statementList()->get(i));
      }
    }
  } break;
  case AstType::IF_STMT: {
    const AstIfStatement *e = DC(AstIfStatement, node);
    check(symtable, e->condition());
    check(symtable, e->hit());
    if (e->miss()) {
      check(symtable, e->miss());
    }
  } break;
  case AstType::WHILE_STMT: {
    const AstWhileStatement *e = DC(AstWhileStatement, node);
    check(symtable, e->condition());
    check(symtable, e->statement());
  } break;
  case AstType::FOR_STMT: {
    const AstForStatement *e = DC(AstForStatement, node);
    check(symtable, e->initial());
    check(symtable, e->condition());
    check(symtable, e->post());
    check(symtable, e->statement());
  } break;
  case AstType::RET_STMT: {
    const AstReturnStatement *e = DC(AstReturnStatement, node);
    check(symtable, e->expression());
  } break;
  default:
    LOG_INFO("do nothing for node: {}", node->toString());
    break;
  }
}

#undef DC
