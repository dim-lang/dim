// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IntegerConstant.h"

namespace fastype {

IntegerConstant::IntegerConstant(std::shared_ptr<Token> token)
    : token_(token) {}

long long IntegerConstant::value() const { return token_->integer(); }

std::string IntegerConstant::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @IntegerConstant token_:{} ]", _1);
}

int IntegerConstant::type() const { return F_TYPE_INTEGER_CONSTANT; }

} // namespace fastype
