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
  std::string _1 = left_ ? left_->toString() : "null";
  std::string _2 = op_ ? op_->toString() : "null";
  std::string _3 = right_ ? right_->toString() : "null";
  return fmt::format("[ @BinaryOp left_:{}, op_:{}, right_:{} ]", _1, _2, _3);
}

Type BinaryOp::type() const { return Type::TP_BINARY_OP; }

} // namespace fastype
