// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/expr/NameLiteral.h"

namespace fastype {

NameLiteral::NameLiteral(Sptr<Token> token) : AstLeaf(token) {}

icu::UnicodeString NameLiteral::value() const { return token_->literal(); }

} // namespace fastype
