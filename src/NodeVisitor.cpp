// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "NodeVisitor.h"
#include "Log.h"

FunctionCallExpressionVisitor::FunctionCallExpressionVisitor(
    std::shared_ptr<Ast> node)
    : node_(std::static_pointer_cast<AstFunctionCallExpression, Ast>(node)) {}

std::shared_ptr<Ast> FunctionCallExpressionVisitor::visit() {
  FINFO("node_:{}", node_ ? node_->toString() : "null");
  return node_;
}

UnaryExpressionVisitor::UnaryExpressionVisitor(std::shared_ptr<Ast> node)
    : node_(std::static_pointer_cast<AstUnaryExpression, Ast>(node)) {}

std::shared_ptr<Ast> UnaryExpressionVisitor::visit() {
  std::shared_ptr<AstExpression> expr =
      std::static_pointer_cast<AstExpression, Ast>(node_->expression());
  switch (node_->token()) {
  case T_ADD: {
    switch (expr->type()) {
    case A_I8_CONSTANT: {
      break;
    }
    default: {
      goto unary_expression_error;
    }
    }
    break;
  }
  case T_SUB: {
    break;
  }
  case T_BIT_NOT: {
    break;
  }
  case T_LOGIC_NOT: {
    break;
  }
  default: {
    goto unary_expression_error;
  }
  }

unary_expression_error:
  FTHROW("invalid node_:{}", node_ ? node_->toString() : "null");
}
