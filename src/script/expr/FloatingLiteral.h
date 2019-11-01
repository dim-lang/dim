// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstLeaf.h"
#include "script/Token.h"

namespace fastype {

class FloatingLiteral : public AstLeaf {
public:
  FloatingLiteral(Sptr<Token> token);
  virtual ~FloatingLiteral() = default;
  virtual double value() const;
};

} // namespace fastype
