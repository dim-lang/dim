// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/AstLeaf.h"
#include "exception/NotImplementException.h"

namespace fastype {

AstLeaf::AstLeaf(Sptr<Token> token) : token_(token) {}

Sptr<AstTree> AstLeaf::child(int i) {
  F_THROW(NotImplementException, "child not implement: {}", toString());
}

virtual int childrenNumber();
virtual std::string toString();
virtual Sptr<Token> token() const;

} // namespace fastype
