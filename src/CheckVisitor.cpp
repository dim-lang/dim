// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "CheckVisitor.h"
#include "Log.h"
#include "symbol/VariableSymbol.h"
#include <algorithm>

std::shared_ptr<NodeVisitor> ExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> expressionVisitor(
      new ExpressionVisitor());
  return expressionVisitor;
}

std::shared_ptr<Ast> ExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  FTHROW("not implement for node:{}", node ? node->toString() : "null");
}

std::shared_ptr<Ast>
PrimaryExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  switch (node->type()) {
  case A_I64_CONSTANT:
  case A_F64_CONSTANT:
  case A_STRING_CONSTANT:
  case A_BOOLEAN_CONSTANT: {
    return node;
    break;
  }
  case A_IDENTIFIER_CONSTANT: {
    /* get reference value and return */
    break;
  }
  default: {
    goto primary_expression_error;
  }
  }

primary_expression_error:
  FTHROW("invalid node:{}", node ? node->toString() : "null");
}

std::shared_ptr<NodeVisitor> PrimaryExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> primaryExpressionVisitor(
      new PrimaryExpressionVisitor());
  return primaryExpressionVisitor;
}

std::shared_ptr<Ast>
FunctionCallExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  FTHROW("not implement for node:{}", node ? node->toString() : "null");
  // FINFO("node:{}", node ? node->toString() : "null");
  // return node;
}

std::shared_ptr<NodeVisitor> FunctionCallExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> functionCallExpressionVisitor(
      new FunctionCallExpressionVisitor());
  return functionCallExpressionVisitor;
}

#define UNARY_OP(x, y, op)                                                     \
  return std::shared_ptr<Ast>(new x(op std::static_pointer_cast<x>(y)->value()))

std::shared_ptr<Ast> UnaryExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  std::shared_ptr<AstExpression> expr = std::static_pointer_cast<AstExpression>(
      ExpressionVisitor::instance()->visit(node->expression()));
  switch (node->token()) {
  case T_ADD: {
    switch (expr->type()) {
    case A_I64_CONSTANT:
      return expr;
    case A_F64_CONSTANT:
      return expr;
    default:
      goto unary_expression_error;
    }
    break;
  }
  case T_SUB: {
    switch (expr->type()) {
    case A_I64_CONSTANT:
      UNARY_OP(AstI64Constant, expr, -);
    case A_F64_CONSTANT:
      UNARY_OP(AstF64Constant, expr, -);
    default:
      goto unary_expression_error;
    }
    break;
  }
  case T_BIT_NOT: {
    switch (expr->type()) {
    case A_I64_CONSTANT:
      UNARY_OP(AstI64Constant, expr, ~);
    default:
      goto unary_expression_error;
    }
    break;
  }
  case T_LOGIC_NOT: {
    switch (expr->type()) {
    case A_BOOLEAN_CONSTANT:
      UNARY_OP(AstBooleanConstant, expr, !);
    default:
      goto unary_expression_error;
    }
    break;
  }
  default:
    goto unary_expression_error;
  }

unary_expression_error:
  FTHROW("invalid node:{}", node ? node->toString() : "null");
}

#undef UNARY_OP

std::shared_ptr<NodeVisitor> UnaryExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> unaryExpressionVisitor(
      new UnaryExpressionVisitor());
  return unaryExpressionVisitor;
}

#define BINARY_OP(x, u, a, v, b, op)                                           \
  return std::shared_ptr<Ast>(new x(std::static_pointer_cast<u>(a)             \
                                        ->value()                              \
                                            op std::static_pointer_cast<v>(b)  \
                                        ->value()))

std::shared_ptr<Ast> BinaryExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  std::shared_ptr<AstBinaryExpression> e =
      std::static_pointer_cast<AstBinaryExpression>(node);
  std::shared_ptr<AstExpression> left = std::static_pointer_cast<AstExpression>(
      ExpressionVisitor::instance()->visit(e->left()));
  std::shared_ptr<AstExpression> right =
      std::static_pointer_cast<AstExpression>(
          ExpressionVisitor::instance()->visit(e->right()));
  switch (e->token()) {
  case T_ADD: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  +);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstI64Constant, left, AstF64Constant, right,
                  +);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstI64Constant, right,
                  +);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstF64Constant, right,
                  +);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_SUB: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  -);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstI64Constant, left, AstF64Constant, right,
                  -);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstI64Constant, right,
                  -);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstF64Constant, right,
                  -);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_MUL: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant,
                  right, *);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstI64Constant, left, AstF64Constant,
                  right, *);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstI64Constant,
                  right, *);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstF64Constant,
                  right, *);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_DIV: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  /);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstI64Constant, left, AstF64Constant, right,
                  /);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstI64Constant, right,
                  /);
      case A_F64_CONSTANT:
        BINARY_OP(AstF64Constant, AstF64Constant, left, AstF64Constant, right,
                  /);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_MOD: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  %);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_BIT_LSHIFT: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  <<);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_BIT_RSHIFT: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  >>);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_BIT_RZEROSHIFT: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  >>);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_LT: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstI64Constant,
                  right, <);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstF64Constant,
                  right, <);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstI64Constant,
                  right, <);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstF64Constant,
                  right, <);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_LE: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstI64Constant,
                  right, <=);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstF64Constant,
                  right, <=);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstI64Constant,
                  right, <=);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstF64Constant,
                  right, <=);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_GT: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstI64Constant,
                  right, >);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstF64Constant,
                  right, >);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstI64Constant,
                  right, >);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstF64Constant,
                  right, >);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_GE: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstI64Constant,
                  right, >=);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstF64Constant,
                  right, >=);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstI64Constant,
                  right, >=);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstF64Constant,
                  right, >=);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_EQ: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstI64Constant,
                  right, ==);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstF64Constant,
                  right, ==);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstI64Constant,
                  right, ==);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstF64Constant,
                  right, ==);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_NEQ: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstI64Constant,
                  right, !=);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstI64Constant, left, AstF64Constant,
                  right, !=);
      default:
        goto binary_expression_error;
      }
      break;
    }
    case A_F64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstI64Constant,
                  right, !=);
      case A_F64_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstF64Constant, left, AstF64Constant,
                  right, !=);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_BIT_AND: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant,
                  right, &);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_BIT_OR: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  |);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_BIT_XOR: {
    switch (left->type()) {
    case A_I64_CONSTANT: {
      switch (right->type()) {
      case A_I64_CONSTANT:
        BINARY_OP(AstI64Constant, AstI64Constant, left, AstI64Constant, right,
                  ^);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_LOGIC_AND: {
    switch (left->type()) {
    case A_BOOLEAN_CONSTANT: {
      switch (right->type()) {
      case A_BOOLEAN_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstBooleanConstant, left,
                  AstBooleanConstant, right, &&);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  case T_LOGIC_OR: {
    switch (left->type()) {
    case A_BOOLEAN_CONSTANT: {
      switch (right->type()) {
      case A_BOOLEAN_CONSTANT:
        BINARY_OP(AstBooleanConstant, AstBooleanConstant, left,
                  AstBooleanConstant, right, ||);
      default:
        goto binary_expression_error;
      }
      break;
    }
    default:
      goto binary_expression_error;
    }
    break;
  }
  default:
    goto binary_expression_error;
  }

binary_expression_error:
  FTHROW("invalid node:{}", node ? node->toString() : "null");
}

#undef BINARY_OP

std::shared_ptr<NodeVisitor> BinaryExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> binaryExpressionVisitor(
      new BinaryExpressionVisitor());
  return binaryExpressionVisitor;
}

std::shared_ptr<Ast>
ConditionalExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  std::shared_ptr<AstConditionalExpression> e =
      std::static_pointer_cast<AstConditionalExpression>(node);
  std::shared_ptr<AstBooleanConstant> cond =
      std::static_pointer_cast<AstBooleanConstant>(
          ExpressionVisitor::instance()->visit(e->condExpression()));
  std::shared_ptr<Ast> ifExpr =
      ExpressionVisitor::instance()->visit(e->ifExpression());
  std::shared_ptr<Ast> elseExpr =
      ExpressionVisitor::instance()->visit(e->elseExpression());
  return cond->value() ? ifExpr : elseExpr;
}

std::shared_ptr<NodeVisitor> ConditionalExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> conditionalExpressionVisitor(
      new ConditionalExpressionVisitor());
  return conditionalExpressionVisitor;
}

std::shared_ptr<Ast>
AssignmentExpressionVisitor::visit(std::shared_ptr<Ast> node) {
  std::shared_ptr<AstAssignmentExpression> e =
      std::static_pointer_cast<AstAssignmentExpression>(node);
  std::shared_ptr<AstIdentifierConstant> var =
      std::static_pointer_cast<AstIdentifierConstant>(
          ExpressionVisitor::instance()->visit(e->left()));
  std::shared_ptr<Ast> val = ExpressionVisitor::instance()->visit(e->right());
  std::shared_ptr<VariableSymbol> sym =
      std::static_pointer_cast<VariableSymbol>(
          Scope::currentScope()->resolve(var->value()));
  switch (e->token()) {
  case T_ASSIGN:
  case T_ADD_ASSIGN:
  case T_SUB_ASSIGN:
  case T_MUL_ASSIGN:
  case T_DIV_ASSIGN:
  case T_MOD_ASSIGN:
  case T_BIT_AND_ASSIGN:
  case T_BIT_OR_ASSIGN:
  case T_BIT_XOR_ASSIGN:
  case T_BIT_LSHIFT_ASSIGN:
  case T_BIT_RSHIFT_ASSIGN:
  case T_BIT_ZERORSHIFT_ASSIGN:
  default:
    goto assignment_expression_error;
  }

assignment_expression_error:
  FTHROW("invalid node:{}", node ? node->toString() : "null");
}

std::shared_ptr<NodeVisitor> AssignmentExpressionVisitor::instance() {
  static std::shared_ptr<NodeVisitor> assignmentExpressionVisitor(
      new AssignmentExpressionVisitor());
  return assignmentExpressionVisitor;
}
