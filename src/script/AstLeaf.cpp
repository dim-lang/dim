// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/AstLeaf.h"
#include "exception/NotImplementException.h"
#include <fmt/format.h>

namespace fastype {

AstLeaf::AstLeaf(Sptr<Token> token) : token_(token) {}

Sptr<AstTree> AstLeaf::child(int i) const {
  F_THROW(NotImplementException, "child not implement: {}", toString());
}

int AstLeaf::childrenNumber() const { return 0; }

std::string AstLeaf::toString() const {
  return fmt::format("[ @AstLeaf token_:{} ]", token_->toString());
}

Sptr<Token> AstLeaf::token() const { return token_; }

} // namespace fastype
