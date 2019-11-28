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

class UnaryOp : public Ast {
public:
  UnaryOp(Sptr<Token> op, Sptr<Ast> expr);
  virtual ~UnaryOp() = default;
  virtual Sptr<Token> op() const;
  virtual Sptr<Ast> expr() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Token> op_;
  Sptr<Ast> expr_;
};

} // namespace fastype
