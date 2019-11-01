// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/expr/IntegerLiteral.h"

namespace fastype {

IntegerLiteral::IntegerLiteral(Sptr<Token> token) : AstLeaf(token) {}

long long IntegerLiteral::value() const { return token_->integer(); }

} // namespace fastype
