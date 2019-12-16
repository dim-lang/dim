// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/UnaryOp.h"

namespace fastype {

UnaryOp::UnaryOp(std::shared_ptr<Token> op, std::shared_ptr<Ast> expr)
    : op_(op), expr_(expr) {}

std::shared_ptr<Token> UnaryOp::op() const { return op_; }

std::shared_ptr<Ast> UnaryOp::expr() const { return expr_; }

std::string UnaryOp::toString() const {
  std::string _1 = op_ ? op_->toString() : "null";
  std::string _2 = expr_ ? expr_->toString() : "null";
  return fmt::format("[ @UnaryOp op_:{}, expr_:{} ]", _1, _2);
}

Type UnaryOp::type() const { return Type::T_UNARY_OP; }

} // namespace fastype
