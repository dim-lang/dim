// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstLeaf.h"
#include "script/Token.h"

namespace fastype {

class IntegerLiteral : public AstLeaf {
public:
  IntegerLiteral(Sptr<Token> token);
  virtual ~IntegerLiteral() = default;
  virtual long long value() const;
};

} // namespace fastype
