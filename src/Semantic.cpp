// Copyright 2019- <coli-lang>
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
  CASSERT(symtable, "symtable is null");
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_TRANSLATE_UNIT: {
    const AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      build(symtable, e->get(i));
    }
  } break;
  case A_VARIABLE_DECLARATION: {
    const AstVariableDeclaration *e = DC(AstVariableDeclaration, node);
    if (e->declarationList()) {
      for (int i = 0; i < e->declarationList()->size(); i++) {
        build(symtable, e->declarationList()->get(i));
      }
    }
  } break;
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    const AstVariableAssignmentDeclaration *e =
        DC(AstVariableAssignmentDeclaration, node);
    VariableSymbol *vs = new VariableSymbol(e->identifier());
    symtable->css()->define(vs);
    switch (e->expression()->type()) {
    case A_I64_CONSTANT:
      symtable->cts()->define(vs, BuiltinType::ty_i64());
      break;
    case A_F64_CONSTANT:
      symtable->cts()->define(vs, BuiltinType::ty_f64());
      break;
    case A_STRING_CONSTANT:
      symtable->cts()->define(vs, BuiltinType::ty_string());
      break;
    case A_BOOLEAN_CONSTANT:
      symtable->cts()->define(vs, BuiltinType::ty_boolean());
      break;
    default:
      symtable->cts()->define(vs, BuiltinType::ty_void());
      break;
    }
  } break;
  case A_FUNCTION_DECLARATION: {
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
    CASSERT(e->statement(), "e#statement is null");
    build(symtable, e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case A_FUNCTION_ARGUMENT_DECLARATION: {
    const AstFunctionArgumentDeclaration *e =
        DC(AstFunctionArgumentDeclaration, node);
    FunctionArgumentSymbol *fa = new FunctionArgumentSymbol(e->value());
    symtable->css()->define(fa);
    symtable->cts()->define(fa, BuiltinType::ty_void());
  } break;
  case A_COMPOUND_STATEMENT: {
    const AstCompoundStatement *e = DC(AstCompoundStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
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
  case A_IF_STATEMENT: {
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
  case A_WHILE_STATEMENT: {
    const AstWhileStatement *e = DC(AstWhileStatement, node);
    CASSERT(e->statement(), "e->statement is null");
    build(symtable, e->statement());
  } break;
  case A_FOR_STATEMENT: {
    const AstForStatement *e = DC(AstForStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
    symtable->pushSymbol(ls);
    symtable->pushType(lt);
    build(symtable, e->initial());
    build(symtable, e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case A_RETURN_STATEMENT: {
    const AstReturnStatement *e = DC(AstReturnStatement, node);
    if (e->expression()) {
      build(symtable, e->expression());
    }
  } break;
  case A_EXPRESSION_STATEMENT: {
    const AstExpressionStatement *e = DC(AstExpressionStatement, node);
    if (e->expression()) {
      build(symtable, e->expression());
    }
  } break;
  default:
    CINFO("do nothing for node:{}", node->toString());
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
  CASSERT(symtable, "symtable is null");
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_TRANSLATE_UNIT: {
    const AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(symtable, e->get(i));
    }
  } break;
  case A_IDENTIFIER_CONSTANT: {
    const AstIdentifierConstant *e = DC(AstIdentifierConstant, node);
    Symbol *s = symtable->css()->resolve(e->value());
    CASSERT(s, "sematic check failure: symbol {} not found", e->value());
  } break;
  case A_CALL_EXPRESSION: {
    const AstCallExpression *e = DC(AstCallExpression, node);
    Symbol *fs = symtable->css()->resolve(e->identifier());
    CASSERT(fs, "sematic check failure: function symbol {} not found",
            e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        const AstFunctionArgumentDeclaration *fad = e->argumentList()->get(i);
        Symbol *fas = symtable->css()->resolve(fad->value());
        CASSERT(fas,
                "sematic check failure: function argument symbol {} not found",
                fad->value());
      }
    }
  } break;
  case A_UNARY_EXPRESSION: {
    const AstUnaryExpression *e = DC(AstUnaryExpression, node);
    check(symtable, e->expression());
  } break;
  case A_BINARY_EXPRESSION: {
    const AstBinaryExpression *e = DC(AstBinaryExpression, node);
    check(symtable, e->left());
    check(symtable, e->right());
  } break;
  case A_CONDITIONAL_EXPRESSION: {
    const AstConditionalExpression *e = DC(AstConditionalExpression, node);
    check(symtable, e->condition());
    check(symtable, e->hit());
    check(symtable, e->miss());
  } break;
  case A_ASSIGNMENT_EXPRESSION: {
    const AstAssignmentExpression *e = DC(AstAssignmentExpression, node);
    check(symtable, e->variable());
    check(symtable, e->value());
  } break;
  case A_SEQUEL_EXPERSSION: {
    const AstSequelExpression *e = DC(AstSequelExpression, node);
    if (e->expressionList()) {
      for (int i = 0; i < e->expressionList()->size(); i++) {
        check(symtable, e->expressionList()->get(i));
      }
    }
  } break;
  case A_EXPRESSION_STATEMENT: {
    const AstExpressionStatement *e = DC(AstExpressionStatement, node);
    check(symtable, e->expresion());
  } break;
  case A_COMPOUND_STATEMENT: {
    const AstCompoundStatement *e = DC(AstCompoundStatement, node);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        check(symtable, e->statementList()->get(i));
      }
    }
  } break;
  case A_IF_STATEMENT: {
    const AstIfStatement *e = DC(AstIfStatement, node);
    check(symtable, e->condition());
    check(symtable, e->hit());
    if (e->miss()) {
      check(symtable, e->miss());
    }
  } break;
  case A_WHILE_STATEMENT: {
    const AstWhileStatement *e = DC(AstWhileStatement, node);
    check(symtable, e->condition());
    check(symtable, e->statement());
  } break;
  case A_FOR_STATEMENT: {
    const AstForStatement *e = DC(AstForStatement, node);
    check(symtable, e->initial());
    check(symtable, e->condition());
    check(symtable, e->post());
    check(symtable, e->statement());
  } break;
  case A_RETURN_STATEMENT: {
    const AstReturnStatement *e = DC(AstReturnStatement, node);
    check(symtable, e->expression());
  } break;
  default:
    CINFO("do nothing for node: {}", node->toString());
    break;
  }
}

#undef DC
