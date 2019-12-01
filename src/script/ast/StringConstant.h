// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class StringConstant : public Ast {
public:
  StringConstant(std::shared_ptr<Token> token);
  virtual ~StringConstant();
  virtual icu::UnicodeString value() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::shared_ptr<Token> token_;
};

} // namespace fastype
