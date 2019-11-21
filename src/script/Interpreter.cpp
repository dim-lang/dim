// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Logging.h"
#include "Token.h"
#include "exception/ParseException.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/FloatingConstant.h"
#include "script/ast/IntegerConstant.h"
#include "script/ast/UnaryOp.h"

namespace fastype {

Interpreter::Interpreter(Sptr<Parser> parser)
    : tree_(nullptr), parser_(parser) {}

Interpreter::~Interpreter() { release(); }

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
  case Ast::AstType::FLOATING_CONSTANT:
    return (long long)visitFloatingConstant(node);
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ParseException, "invalid node type: {}", node->toString());
  }
  return -1LL;
}

long long Interpreter::visitBinaryOp(Ast *node) {
  BinaryOp *e = (BinaryOp *)node;
  if (e->op() == Token::T_ADD) {
    return visit(e->left()) + visit(e->right());
  } else if (e->op() == Token::T_SUB) {
    return visit(e->left()) - visit(e->right());
  } else if (e->op() == Token::T_MUL) {
    return visit(e->left()) * visit(e->right());
  } else if (e->op() == Token::T_DIV) {
    return visit(e->left()) / visit(e->right());
  } else if (e->op() == Token::T_MOD) {
    return visit(e->left()) % visit(e->right());
  }
  F_CHECK(false, "must not reach here, node:{}", node->toString());
  F_THROW(ParseException, "invalid node type: {}", node->toString());
  return -1LL;
}

long long Interpreter::visitUnaryOp(Ast *node) {
  UnaryOp *e = (UnaryOp *)node;
  if (e->op() == Token::T_ADD) {
    return visit(e->expr());
  } else if (e->op() == Token::T_SUB) {
    return -visit(e->expr());
  }
  F_CHECK(false, "must not reach here, node:{}", node->toString());
  F_THROW(ParseException, "invalid node type: {}", node->toString());
  return -1LL;
}

long long Interpreter::visitIntergerConstant(Ast *node) {
  IntegerConstant *e = (IntegerConstant *)node;
  return e->value();
}

double Interpreter::visitFloatingConstant(Ast *node) {
  FloatingConstant *e = (FloatingConstant *)node;
  return e->value();
}

long long Interpreter::interpret() {
  release();
  tree_ = parser_->parse();
  return visit(tree_);
}

static void releaseImpl(Ast *node) {
  if (!node) {
    return;
  }
  switch (node->type()) {
  case Ast::AstType::BINARY_OP: {
    releaseImpl(((BinaryOp *)node)->left());
    releaseImpl(((BinaryOp *)node)->right());
  } break;
  case Ast::AstType::UNARY_OP: {
    releaseImpl(((UnaryOp *)node)->expr());
  } break;
  case Ast::AstType::INTEGER_CONSTANT:
  case Ast::AstType::FLOATING_CONSTANT:
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ParseException, "invalid node type: {}", node->toString());
  }
  delete node;
}

void Interpreter::release() {
  if (!tree_) {
    return;
  }
  releaseImpl(tree_);
}

} // namespace fastype
