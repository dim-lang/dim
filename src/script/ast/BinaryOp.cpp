// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/BinaryOp.h"

namespace fastype {

BinaryOp::BinaryOp(std::shared_ptr<Ast> left, std::shared_ptr<Token> op,
                   std::shared_ptr<Ast> right)
    : left_(left), op_(op), right_(right) {}

std::shared_ptr<Ast> BinaryOp::left() const { return left_; }

std::shared_ptr<Token> BinaryOp::op() const { return op_; }

std::shared_ptr<Ast> BinaryOp::right() const { return right_; }

std::string BinaryOp::toString() const {
  return fmt::format("[ @BinaryOp left_:{}, op_:{}, right_:{} ]",
                     left_->toString(), op_->toString(), right_->toString());
}

Ast::AstType BinaryOp::type() const { return Ast::AstType::BINARY_OP; }

} // namespace fastype
