// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/CommentToken.h"
#include <fmt/format.h>

namespace fastype {

CommentToken::CommentToken(int lineNumber, Cowstr literal)
    : Token(lineNumber, TokenType::TT_COMMENT), literal_(literal) {}

Cowstr CommentToken::literal() const { return literal_; }

virtual std::string toString() const {
  return fmt::format("[ @CommentToken lineNumber_:{}, literal_:{}, type_:{} ]",
                     lineNumber_, literal_.stdstr(), type_.name());
}

} // namespace fastype
