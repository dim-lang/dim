// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/UnaryOp.h"

namespace fastype {

UnaryOp::UnaryOp(std::shared_ptr<Token> op, std::shared_ptr<Ast> expr)
    : op_(op), expr_(expr) {}

std::shared_ptr<Token> UnaryOp::op() const { return op_; }

std::shared_ptr<Ast> UnaryOp::expr() const { return expr_; }

std::string UnaryOp::toString() const {
  return fmt::format("[ @UnaryOp op_:{}, expr_:{} ]", op_->toString(),
                     expr_->toString());
}

Ast::AstType UnaryOp::type() const { return Ast::AstType::UNARY_OP; }

} // namespace fastype
