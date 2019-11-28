// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/UnaryOp.h"
#include <fmt/format.h>

namespace fastype {

UnaryOp::UnaryOp(Sptr<Token> op, Sptr<Ast> expr) : op_(op), expr_(expr) {}

Sptr<Token> UnaryOp::op() const { return op_; }

Sptr<Ast> UnaryOp::expr() const { return expr_; }

std::string UnaryOp::toString() const {
  return fmt::format("[ @UnaryOp op_:{}, expr_:{} ]", op_->toString(),
                     expr_->toString());
}

Ast::AstType UnaryOp::type() const { return Ast::AstType::UNARY_OP; }

} // namespace fastype
