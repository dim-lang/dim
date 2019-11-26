// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/Ast.h"
#include "script/Token.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class BooleanConstant : public Ast {
public:
  BooleanConstant(Sptr<Token> token);
  virtual ~BooleanConstant() = default;
  virtual bool value() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Token> token_;
};

} // namespace fastype
