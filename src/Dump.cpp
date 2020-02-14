// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Ast.h"
#include "Log.h"
#include <algorithm>
#include <functional>
#include <string>
#include <utility>

static int dumpAstHeight(Ast *e, int c) {
  if (!e) {
    return c;
  }
  switch (e->type()) {
  case A_IDENTIFIER_CONSTANT:
  case A_I8_CONSTANT:
  case A_U8_CONSTANT:
  case A_I16_CONSTANT:
  case A_U16_CONSTANT:
  case A_I32_CONSTANT:
  case A_U32_CONSTANT:
  case A_I64_CONSTANT:
  case A_U64_CONSTANT:
  case A_BOOLEAN_CONSTANT:
  case A_STRING_CONSTANT:
  case A_F32_CONSTANT:
  case A_F64_CONSTANT:
    return c + 1;
  case A_CALL_EXPRESSION: {
    AstCallExpression *t = dynamic_cast<AstCallExpression *>(e);
    return dumpAstHeight(t->argumentList(), c + 1);
  }
  case A_UNARY_EXPRESSION: {
    AstUnaryExpression *t = dynamic_cast<AstUnaryExpression *>(e);
    return dumpAstHeight(t->expression(), c + 1);
  }
  case A_BINARY_EXPRESSION: {
    AstBinaryExpression *t = dynamic_cast<AstBinaryExpression *>(e);
    int a = dumpAstHeight(t->left(), c + 1);
    int b = dumpAstHeight(t->right(), c + 1);
    return std::max(a, b);
  }
  case A_CONDITIONAL_EXPRESSION: {
    AstConditionalExpression *t = dynamic_cast<AstConditionalExpression *>(e);
    int p = dumpAstHeight(t->condition(), c + 1);
    int q = dumpAstHeight(t->left(), c + 1);
    int u = dumpAstHeight(t->right(), c + 1);
    return std::max(p, std::max(q, u));
  }
  case A_ASSIGNMENT_EXPRESSION: {
    AstAssignmentExpression *t = dynamic_cast<AstAssignmentExpression *>(e);
    int p = dumpAstHeight(t->left(), c + 1);
    int q = dumpAstHeight(t->right(), c + 1);
    return std::max(p, q);
  }
  case A_EXPRESSION_STATEMENT: {
    AstExpressionStatement *t = dynamic_cast<AstExpressionStatement *>(e);
    return dumpAstHeight(t->expression(), c + 1);
  }
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *t = dynamic_cast<AstCompoundStatement *>(e);
    return dumpAstHeight(t->statementList(), c + 1);
  }
  case A_IF_STATEMENT: {
    AstIfStatement *t = dynamic_cast<AstIfStatement *>(e);
    int p = dumpAstHeight(t->condition(), c + 1);
    int q = dumpAstHeight(t->left(), c + 1);
    int u = dumpAstHeight(t->right(), c + 1);
    return std::max(p, std::max(q, u));
  }
  case A_WHILE_STATEMENT: {
    AstWhileStatement *t = dynamic_cast<AstWhileStatement *>(e);
    int p = dumpAstHeight(t->condition(), c + 1);
    int q = dumpAstHeight(t->statement(), c + 1);
    return std::max(p, q);
  }
  case A_FOR_STATEMENT: {
    AstForStatement *t = dynamic_cast<AstForStatement *>(e);
    int p = dumpAstHeight(t->initialize(), c + 1);
    int q = dumpAstHeight(t->condition(), c + 1);
    int u = dumpAstHeight(t->post(), c + 1);
    int v = dumpAstHeight(t->statement(), c + 1);
    return std::max(std::max(p, q), std::max(u, v));
  }
  case A_CONTINUE_STATEMENT: {
    return c + 1;
  }
  case A_BREAK_STATEMENT: {
    return c + 1;
  }
  case A_RETURN_STATEMENT: {
    AstReturnStatement *t = dynamic_cast<AstReturnStatement *>(e);
    return dumpAstHeight(t->expression(), c + 1);
  }
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *t = dynamic_cast<AstVariableDeclaration *>(e);
    return dumpAstHeight(t->declarationList(), c + 1);
  }
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *t =
        dynamic_cast<AstVariableAssignmentDeclaration *>(e);
    return dumpAstHeight(t->expression(), c + 1);
  }
  case A_FUNCTION_DECLARATION: {
    AstFunctionDeclaration *t = dynamic_cast<AstFunctionDeclaration *>(e);
    int p = dumpAstHeight(t->argumentList(), c + 1);
    int q = dumpAstHeight(t->result(), c + 1);
    int u = dumpAstHeight(t->statement(), c + 1);
    return std::max(std::max(p, q), u);
  }
  case A_FUNCTION_ARGUMENT_DECLARATION: {
    return c + 1;
  }
  case A_EXPRESSION_LIST: {
    AstExpressionList *t = dynamic_cast<AstExpressionList *>(e);
    std::vector<int> v;
    for (int i = 0; i < t->size(); i++) {
      v.push_back(dumpAstHeight(t->get(i), c + 1));
    }
    return std::max_element(v.begin(), v.end());
  }
  case A_STATEMENT_LIST: {
    AstStatementList *t = dynamic_cast<AstStatementList *>(e);
    std::vector<int> v;
    for (int i = 0; i < t->size(); i++) {
      v.push_back(dumpAstHeight(t->get(i), c + 1));
    }
    return std::max_element(v.begin(), v.end());
  }
  case A_DECLARATION_LIST: {
    AstDeclarationList *t = dynamic_cast<AstDeclarationList *>(e);
    std::vector<int> v;
    for (int i = 0; i < t->size(); i++) {
      v.push_back(dumpAstHeight(t->get(i), c + 1));
    }
    return std::max_element(v.begin(), v.end());
  }
  default:
    break;
  }
  CASSERT(false, "must not reach here");
}

static int dumpAstWidth(Ast *e, int c) {
  if (!e) {
    return c;
  }
  switch (e->type()) {
  case A_IDENTIFIER_CONSTANT: {
    AstIdentifierConstant *t = dynamic_cast<AstIdentifierConstant *>(e);
    return t->value().length();
  }
  case A_I8_CONSTANT: {
    AstI8Constant *t = dynamic_cast<AstI8Constant *>(e);
    return std::to_string(t->value()).length();
  }
  case A_U8_CONSTANT: {
    AstU8Constant *t = dynamic_cast<AstU8Constant *>(e);
    return std::to_string(t->value()).length();
  }
  case A_I16_CONSTANT: {
    AstI16Constant *t = dynamic_cast<AstI16Constant *>(e);
    return std::to_string(t->value()).length();
  }
  case A_U16_CONSTANT: {
    AstU16Constant *t = dynamic_cast<AstU16Constant *>(e);
    return std::to_string(t->value()).length();
  }
  case A_I32_CONSTANT: {
    AstI32Constant *t = dynamic_cast<AstI32Constant *>(e);
    return std::to_string(t->value()).length();
  }
  case A_U32_CONSTANT: {
    AstU32Constant *t = dynamic_cast<AstU32Constant *>(e);
    return std::to_string(t->value()).length();
  }
  case A_I64_CONSTANT:
  case A_U64_CONSTANT:
  case A_BOOLEAN_CONSTANT:
  case A_STRING_CONSTANT:
  case A_F32_CONSTANT:
  case A_F64_CONSTANT:
    return c + 1;
  case A_CALL_EXPRESSION: {
    AstCallExpression *t = dynamic_cast<AstCallExpression *>(e);
    return dumpAstWidth(t->argumentList(), c + 1);
  }
  case A_UNARY_EXPRESSION: {
    AstUnaryExpression *t = dynamic_cast<AstUnaryExpression *>(e);
    return dumpAstWidth(t->expression(), c + 1);
  }
  case A_BINARY_EXPRESSION: {
    AstBinaryExpression *t = dynamic_cast<AstBinaryExpression *>(e);
    int a = dumpAstWidth(t->left(), c + 1);
    int b = dumpAstWidth(t->right(), c + 1);
    return std::max(a, b);
  }
  case A_CONDITIONAL_EXPRESSION: {
    AstConditionalExpression *t = dynamic_cast<AstConditionalExpression *>(e);
    int p = dumpAstWidth(t->condition(), c + 1);
    int q = dumpAstWidth(t->left(), c + 1);
    int u = dumpAstWidth(t->right(), c + 1);
    return std::max(p, std::max(q, u));
  }
  case A_ASSIGNMENT_EXPRESSION: {
    AstAssignmentExpression *t = dynamic_cast<AstAssignmentExpression *>(e);
    int p = dumpAstWidth(t->left(), c + 1);
    int q = dumpAstWidth(t->right(), c + 1);
    return std::max(p, q);
  }
  case A_EXPRESSION_STATEMENT: {
    AstExpressionStatement *t = dynamic_cast<AstExpressionStatement *>(e);
    return dumpAstWidth(t->expression(), c + 1);
  }
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *t = dynamic_cast<AstCompoundStatement *>(e);
    return dumpAstWidth(t->statementList(), c + 1);
  }
  case A_IF_STATEMENT: {
    AstIfStatement *t = dynamic_cast<AstIfStatement *>(e);
    int p = dumpAstWidth(t->condition(), c + 1);
    int q = dumpAstWidth(t->left(), c + 1);
    int u = dumpAstWidth(t->right(), c + 1);
    return std::max(p, std::max(q, u));
  }
  case A_WHILE_STATEMENT: {
    AstWhileStatement *t = dynamic_cast<AstWhileStatement *>(e);
    int p = dumpAstWidth(t->condition(), c + 1);
    int q = dumpAstWidth(t->statement(), c + 1);
    return std::max(p, q);
  }
  case A_FOR_STATEMENT: {
    AstForStatement *t = dynamic_cast<AstForStatement *>(e);
    int p = dumpAstWidth(t->initialize(), c + 1);
    int q = dumpAstWidth(t->condition(), c + 1);
    int u = dumpAstWidth(t->post(), c + 1);
    int v = dumpAstWidth(t->statement(), c + 1);
    return std::max(std::max(p, q), std::max(u, v));
  }
  case A_CONTINUE_STATEMENT: {
    return c + 1;
  }
  case A_BREAK_STATEMENT: {
    return c + 1;
  }
  case A_RETURN_STATEMENT: {
    AstReturnStatement *t = dynamic_cast<AstReturnStatement *>(e);
    return dumpAstWidth(t->expression(), c + 1);
  }
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *t = dynamic_cast<AstVariableDeclaration *>(e);
    return dumpAstWidth(t->declarationList(), c + 1);
  }
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *t =
        dynamic_cast<AstVariableAssignmentDeclaration *>(e);
    return dumpAstWidth(t->expression(), c + 1);
  }
  case A_FUNCTION_DECLARATION: {
    AstFunctionDeclaration *t = dynamic_cast<AstFunctionDeclaration *>(e);
    int p = dumpAstWidth(t->argumentList(), c + 1);
    int q = dumpAstWidth(t->result(), c + 1);
    int u = dumpAstWidth(t->statement(), c + 1);
    return std::max(std::max(p, q), u);
  }
  case A_FUNCTION_ARGUMENT_DECLARATION: {
    return c + 1;
  }
  case A_EXPRESSION_LIST: {
    AstExpressionList *t = dynamic_cast<AstExpressionList *>(e);
    std::vector<int> v;
    for (int i = 0; i < t->size(); i++) {
      v.push_back(dumpAstWidth(t->get(i), c + 1));
    }
    return std::max_element(v.begin(), v.end());
  }
  case A_STATEMENT_LIST: {
    AstStatementList *t = dynamic_cast<AstStatementList *>(e);
    std::vector<int> v;
    for (int i = 0; i < t->size(); i++) {
      v.push_back(dumpAstWidth(t->get(i), c + 1));
    }
    return std::max_element(v.begin(), v.end());
  }
  case A_DECLARATION_LIST: {
    AstDeclarationList *t = dynamic_cast<AstDeclarationList *>(e);
    std::vector<int> v;
    for (int i = 0; i < t->size(); i++) {
      v.push_back(dumpAstWidth(t->get(i), c + 1));
    }
    return std::max_element(v.begin(), v.end());
  }
  default:
    break;
  }
  CASSERT(false, "must not reach here");
}

void Dump::print(Ast *e) {
  int height = dumpAstHeight(e, 0);
  int width = dumpAstWidth(e, 0);
}

void Dump::print(Symtab *e) {}

void Dump::print(Tytab *e) {}
