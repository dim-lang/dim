// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/BinaryOp.h"
#include <fmt/format.h>

namespace fastype {

BinaryOp::BinaryOp(Sptr<Ast> left, Sptr<Token> op, Sptr<Ast> right)
    : left_(left), op_(op), right_(right) {}

Sptr<Ast> BinaryOp::left() const { return left_; }

Sptr<Token> BinaryOp::op() const { return op_; }

Sptr<Ast> BinaryOp::right() const { return right_; }

std::string BinaryOp::toString() const {
  return fmt::format("[ @BinaryOp left_:{}, op_:{}, right_:{} ]",
                     left_->toString(), op_->toString(), right_->toString());
}

Ast::AstType BinaryOp::type() const { return Ast::AstType::BINARY_OP; }

} // namespace fastype
