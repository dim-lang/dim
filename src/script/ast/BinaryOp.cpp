// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/BinaryOp.h"
#include <fmt/format.h>

namespace fastype {

BinaryOp::BinaryOp(Sptr<AstTree> left, Sptr<AstTree> op, Sptr<AstTree> right)
    : left_(left), op_(op), right_(right) {}

Sptr<AstTree> BinaryOp::left() const { return left_; }

Sptr<AstTree> BinaryOp::op() const { return op_; }

Sptr<AstTree> BinaryOp::right() const { return right_; }

std::string BinaryOp::toString() const {
  return fmt::format("[ @BinaryOp left_:{}, op_:{}, right_:{} ]",
                     left_->toString(), op_->toString(), right_->toString());
}

std::string BinaryOp::name() const { return "BinaryOp"; }

} // namespace fastype
