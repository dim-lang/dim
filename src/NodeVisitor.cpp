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
    case A_I8_CONSTANT:
    case A_UI8_CONSTANT:
    case A_I16_CONSTANT:
    case A_UI16_CONSTANT:
    case A_I32_CONSTANT:
    case A_UI32_CONSTANT:
    case A_I64_CONSTANT:
    case A_UI64_CONSTANT:
    case A_F32_CONSTANT:
    case A_F64_CONSTANT:
      return expr;
      break;
    default:
      goto unary_expression_error;
    }
    break;
  }

#define UNARY_OP(x, y, op)                                                     \
  std::static_pointer_cast<x, AstExpression>(y)->reset(                        \
      op std::static_pointer_cast<x, AstExpression>(y)->value());              \
  return y;                                                                    \
  break

  case T_SUB: {
    switch (expr->type()) {
    case A_I8_CONSTANT:
      UNARY_OP(AstI8Constant, expr, -);
    case A_UI8_CONSTANT:
      UNARY_OP(AstUI8Constant, expr, -);
    case A_I16_CONSTANT:
      UNARY_OP(AstI16Constant, expr, -);
    case A_UI16_CONSTANT:
      UNARY_OP(AstUI16Constant, expr, -);
    case A_I32_CONSTANT:
      UNARY_OP(AstI32Constant, expr, -);
    case A_UI32_CONSTANT:
      UNARY_OP(AstUI32Constant, expr, -);
    case A_I64_CONSTANT:
      UNARY_OP(AstI64Constant, expr, -);
    case A_UI64_CONSTANT:
      UNARY_OP(AstUI64Constant, expr, -);
    case A_F32_CONSTANT:
      UNARY_OP(AstF32Constant, expr, -);
    case A_F64_CONSTANT:
      UNARY_OP(AstF64Constant, expr, -);
    default:
      goto unary_expression_error;
      break;
    }
    break;
  }
  case T_BIT_NOT: {
    switch (expr->type()) {
    case A_I8_CONSTANT:
      UNARY_OP(AstI8Constant, expr, ~);
    case A_UI8_CONSTANT:
      UNARY_OP(AstUI8Constant, expr, ~);
    case A_I16_CONSTANT:
      UNARY_OP(AstI16Constant, expr, ~);
    case A_UI16_CONSTANT:
      UNARY_OP(AstUI16Constant, expr, ~);
    case A_I32_CONSTANT:
      UNARY_OP(AstI32Constant, expr, ~);
    case A_UI32_CONSTANT:
      UNARY_OP(AstUI32Constant, expr, ~);
    case A_I64_CONSTANT:
      UNARY_OP(AstI64Constant, expr, ~);
    case A_UI64_CONSTANT:
      UNARY_OP(AstUI64Constant, expr, ~);
    default:
      goto unary_expression_error;
      break;
    }
    break;
  }
  case T_LOGIC_NOT: {
    switch (expr->type()) {
    case A_BOOLEAN_CONSTANT:
      UNARY_OP(AstBooleanConstant, expr, !);
    default:
      goto unary_expression_error;
      break;
    }
    break;
  }
  default: {
    goto unary_expression_error;
  }
  }

unary_expression_error:
  FTHROW("invalid node_:{}", node_ ? node_->toString() : "null");
}

#undef UNARY_OP
