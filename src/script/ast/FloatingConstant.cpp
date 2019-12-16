// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/FloatingConstant.h"

namespace fastype {

FloatingConstant::FloatingConstant(std::shared_ptr<Token> token)
    : token_(token) {}

double FloatingConstant::value() const { return token_->floating(); }

std::string FloatingConstant::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @FloatingConstant token_:{} ]", _1);
}

Type FloatingConstant::type() const { return Type::T_FLOATING_CONSTANT; }

} // namespace fastype
