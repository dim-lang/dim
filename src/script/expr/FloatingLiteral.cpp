// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/expr/FloatingLiteral.h"

namespace fastype {

FloatingLiteral::FloatingLiteral(Sptr<Token> token) : AstLeaf(token) {}

double FloatingLiteral::value() const { return token_->floating(); }

} // namespace fastype
