// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Parameter.h"

namespace fastype {

Parameter::Parameter(std::shared_ptr<Token> token) : token_(token) {}

icu::UnicodeString Parameter::value() const { return token_->literal(); }

std::string Parameter::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @Parameter token_:{} ]", _1);
}

int Parameter::type() const { return F_TYPE_INTEGER_CONSTANT; }

} // namespace fastype
