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

class BinaryOp : public Ast {
public:
  BinaryOp(Sptr<Ast> left, Sptr<Token> op, Sptr<Ast> right);
  virtual ~BinaryOp() = default;
  virtual Sptr<Ast> left() const;
  virtual Sptr<Token> op() const;
  virtual Sptr<Ast> right() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Ast> left_;
  Sptr<Token> op_;
  Sptr<Ast> right_;
};

} // namespace fastype
