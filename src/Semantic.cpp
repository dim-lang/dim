// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Log.h"
#include "Semant.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Type.h"
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
void Semantic::build(SymbolTable *symtable, const Ast *node) {
  CASSERT(symtable, "symtable is null");
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_TRANSLATE_UNIT: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      build(e->get(i));
    }
  } break;
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *e = DC(AstVariableDeclaration, node);
    if (e->declarationList()) {
      for (int i = 0; i < e->declarationList()->size(); i++) {
        build(e->declarationList()->get(i));
      }
    }
  } break;
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *e =
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
    AstFunctionDeclaration *e = DC(AstFunctionDeclaration, node);
    FunctionSymbol *fs = new FunctionSymbol(e->identifier(), symtable->css());
    std::vector<std::pair<Symbol *, Type *>> argumentTypeList;
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        AstFunctionArgumentDeclaration *fa =
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
        build(e->argumentList()->get(i));
      }
    }
    CASSERT(e->statement(), "e#statement is null");
    build(e->statement());
    SymbolTable::popSymbol();
    SymbolTable::popType();
  } break;
  case A_FUNCTION_ARGUMENT_DECLARATION: {
    AstFunctionArgumentDeclaration *e =
        DC(AstFunctionArgumentDeclaration, node);
    FunctionArgumentSymbol *fa = new FunctionArgumentSymbol(e->value());
    symtable->css()->define(fa);
    symtable->cts()->define(fa, BuiltinType::ty_void());
  } break;
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
    symtable->pushSymbol(ls);
    symtable->pushType(lt);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        build(e->statementList()->get(i));
      }
    }
    SymbolTable::popSymbol();
    SymbolTable::popType();
  } break;
  case A_IF_STATEMENT: {
    AstIfStatement *e = DC(AstIfStatement, node);
    if (e->condition()) {
      build(e->condition());
    }
    if (e->hit()) {
      build(e->hit());
    }
    if (e->miss()) {
      build(e->miss());
    }
  } break;
  case A_WHILE_STATEMENT: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    CASSERT(e->statement(), "e->statement is null");
    build(e->statement());
  } break;
  case A_FOR_STATEMENT: {
    AstForStatement *e = DC(AstForStatement, node);
    LocalSymtab *ls = new LocalSymtab(e->name(), symtable->css());
    LocalTytab *lt = new LocalTytab(e->name(), symtable->cts());
    symtable->pushSymbol(ls);
    symtable->pushType(lt);
    build(e->initial());
    build(e->statement());
    symtable->popSymbol();
    symtable->popType();
  } break;
  case A_RETURN_STATEMENT: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    if (e->expression()) {
      build(e->expression());
    }
  } break;
  case A_EXPRESSION_STATEMENT: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    if (e->expression()) {
      build(e->expression());
    }
  } break;
  default:
    CINFO("do nothing for node:{}", node->toString());
    break;
  }
}

void Semant::checkImpl(Ast *node) {
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_IDENTIFIER_CONSTANT: {
    AstIdentifierConstant *e = dynamic_cast<AstIdentifierConstant *>(node);
    Symbol *sym = csym_->resolve(e->value());
    Type *ty = cty_->resolve(sym);
    CASSERT(sym, "sym is null");
    CASSERT(ty, "ty is null");
  } break;
  case A_UNARY_EXPRESSION: {
    AstUnaryExpression *e = dynamic_cast<AstUnaryExpression *>(node);
    checkImpl(e->expression());
  } break;
  case A_BINARY_EXPRESSION: {
    AstBinaryExpression *e = dynamic_cast<AstBinaryExpression *>(node);
    checkImpl(e->left());
    checkImpl(e->right());
  } break;
  case A_CONDITIONAL_EXPRESSION: {
    AstBinaryExpression *e = dynamic_cast<AstBinaryExpression *>(node);
    checkImpl(e->left());
    checkImpl(e->right());
  } break;
  case A_ASSIGNMENT_EXPRESSION: {
    AstAssignmentExpression *e = dynamic_cast<AstAssignmentExpression *>(node);
    checkImpl(e->variable());
  } break;
  case A_EXPRESSION_STATEMENT: {
    AstExpressionStatement *e = dynamic_cast<AstExpressionStatement *>(node);
    checkImpl(e->expression());
  } break;
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *e = dynamic_cast<AstCompoundStatement *>(node);
    Symbol *sym = csym_->resolve(e->name());
    Type *ty = cty_->resolve(sym);
    Symbol::push(gsym_, csym_, dynamic_cast<Symtab *>(sym));
    Type::push(gty_, cty_, dynamic_cast<Tytab *>(ty));
    for (int i = 0; i < e->statementList()->size(); i++) {
      checkImpl(e->statementList()->get(i));
    }
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  case A_IF_STATEMENT: {
    AstIfStatement *e = dynamic_cast<AstIfStatement *>(node);
    checkImpl(e->hit());
    checkImpl(e->miss());
  } break;
  case A_WHILE_STATEMENT: {
    AstWhileStatement *e = dynamic_cast<AstWhileStatement *>(node);
    checkImpl(e->condition());
    checkImpl(e->statement());
  } break;
  case A_FOR_STATEMENT: {
    AstForStatement *e = dynamic_cast<AstForStatement *>(node);
    Symbol *sym = csym_->resolve(e->name());
    Type *ty = cty_->resolve(sym);
    Symbol::push(gsym_, csym_, dynamic_cast<Symtab *>(sym));
    Type::push(gty_, cty_, dynamic_cast<Tytab *>(ty));
    checkImpl(e->initial());
    checkImpl(e->condition());
    checkImpl(e->post());
    checkImpl(e->statement());
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  default: {
    CINFO("do nothing for node:{}", node->toString());
  } break;
  }
}

void Semant::check() {
  CASSERT(gsym_, "gsym_ is null");
  CASSERT(gty_, "gty_ is null");
  AstTranslateUnit *e = dynamic_cast<AstTranslateUnit *>(translateUnit_);
  CASSERT(e, "translateUnit_ is null: {}", e ? e->toString() : "null");
  Symbol::push(gsym_, csym_, gsym_);
  Type::push(gty_, cty_, gty_);
  for (int i = 0; i < e->size(); i++) {
    checkImpl(e->get(i));
  }
  Symbol::pop(gsym_, csym_);
  Type::pop(gty_, cty_);
}

/**
 * A_TRANSLATE_UNIT
 *
 * A_IDENTIFIER_CONSTANT
 * A_I8_CONSTANT
 * A_U8_CONSTANT
 * A_I16_CONSTANT
 * A_U16_CONSTANT
 * A_I32_CONSTANT
 * A_U32_CONSTANT
 * A_I64_CONSTANT
 * A_U64_CONSTANT
 * A_F32_CONSTANT
 * A_F64_CONSTANT
 * A_BOOLEAN_CONSTANT
 * A_STRING_CONSTANT
 *
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
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    for (int i = 0; i < e->size(); i++) {
      check(e->get(i));
    }
  } break;
  case A_IDENTIFIER_CONSTANT: {
    AstIdentifierConstant *e = DC(AstIdentifierConstant, node);
    Symbol *s = symtable->css()->resolve(e->value());
    CASSERT(s, "sematic check failure: symbol {} not found", e->value());
  } break;
  case A_CALL_EXPRESSION: {
    AstCallExpression *e = DC(AstCallExpression, node);
    Symbol *fs = symtable->css()->resolve(e->identifier());
    CASSERT(fs, "sematic check failure: function symbol {} not found",
            e->identifier());
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        AstFunctionArgumentDeclaration *fad = e->argumentList()->get(i);
        Symbol *fas = symtable->css()->resolve(fad->value());
        CASSERT(fas,
                "sematic check failure: function argument symbol {} not found",
                fad->value());
      }
    }
  } break;
  case A_UNARY_EXPRESSION: {
    AstUnaryExpression *e = DC(AstUnaryExpression, node);
    check(e->expression());
  } break;
  case A_BINARY_EXPRESSION: {
    AstBinaryExpression *e = DC(AstBinaryExpression, node);
    check(e->left());
    check(e->right());
  } break;
  case A_CONDITIONAL_EXPRESSION: {
    AstConditionalExpresion *e = DC(AstConditionalExpresion, node);
    check(e->condition());
    check(e->hit());
    check(e->miss());
  } break;
  case A_ASSIGNMENT_EXPRESSION: {
    AstAssignmentExpression *e = DC(AstAssignmentExpression, node);
    check(e->variable());
    check(e->value());
  } break;
  case A_SEQUEL_EXPERSSION: {
    AstSequelExpression *e = DC(AstSequelExpression, node);
    if (e->expressionList()) {
      for (int i = 0; i < e->expressionList()->size(); i++) {
        check(e->expressionList()->get(i));
      }
    }
  } break;
  case A_EXPRESSION_STATEMENT: {
    AstExpressionStatement *e = DC(AstExpressionStatement, node);
    check(e->expresion());
  } break;
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    if (e->statementList()) {
      for (int i = 0; i < e->statementList()->size(); i++) {
        check(e->statementList()->get(i));
      }
    }
  } break;
  case A_IF_STATEMENT: {
    AstIfStatement *e = DC(AstIfStatement, node);
    check(e->condition());
    check(e->hit());
    if (e->miss()) {
      check(e->miss());
    }
  } break;
  case A_WHILE_STATEMENT: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    check(e->condition());
    check(e->statement());
  } break;
  case A_FOR_STATEMENT: {
    AstForStatement *e = DC(AstForStatement, node);
    check(e->initial());
    check(e->condition());
    check(e->post());
    check(e->statement());
  } break;
  case A_RETURN_STATEMENT: {
    AstReturnStatement *e = DC(AstReturnStatement, node);
    check(e->expression());
  } break;
  default:
    CINFO("do nothing for node: {}", node->toString());
    break;
  }
}

#undef DC
