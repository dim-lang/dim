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

class StringConstant : public Ast {
public:
  StringConstant(Sptr<Token> token);
  virtual ~StringConstant() = default;
  virtual icu::UnicodeString value() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Token> token_;
};

} // namespace fastype
