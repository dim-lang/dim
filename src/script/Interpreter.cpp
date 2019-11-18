// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Logging.h"
#include "Token.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/IntegerConstant.h"
#include "script/ast/UnaryOp.h"

namespace fastype {

Interpreter::Interpreter(Sptr<Parser> parser) : parser_(parser) {}

long long Interpreter::visit(Ast *node) {
  switch (node->type()) {
  case Ast::AstType::BINARY_OP:
    return visitBinaryOp(node);
    break;
  case Ast::AstType::UNARY_OP:
    return visitUnaryOp(node);
    break;
  case Ast::AstType::INTEGER_CONSTANT:
    return visitIntergerConstant(node);
    break;
  }
  F_CHECK(false, "Interpreter#visit must not be here, node:{}",
          node->toString());
}

long long Interpreter::visitBinaryOp(Ast *node) {
  BinaryOp *t = (BinaryOp *)node;
  if (t->op() == Token::T_ADD) {
    return visit(t->left()) + visit(t->right());
  } else if (t->op() == Token::T_SUB) {
    return visit(t->left()) - visit(t->right());
  } else if (t->op() == Token::T_MUL) {
    return visit(t->left()) * visit(t->right());
  } else if (t->op() == Token::T_DIV) {
    return visit(t->left()) / visit(t->right());
  } else if (t->op() == Token::T_MOD) {
    return visit(t->left()) % visit(t->right());
  }
  F_CHECK(false, "Interpreter#visitBinaryOp must not be here, node:{}",
          node->toString());
}

long long Interpreter::visitIntergerConstant(Ast *node) {
  IntegerConstant *t = (IntegerConstant *)node;
  return t->value();
}

long long Interpreter::visitUnaryOp(Ast *node) { return 0L; }

long long Interpreter::interpret() {
  Ast *tree = parser_->parse();
  return visit(tree);
}

} // namespace fastype
