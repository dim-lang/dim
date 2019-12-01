// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class BinaryOp : public Ast {
public:
  BinaryOp(std::shared_ptr<Ast> left, std::shared_ptr<Token> op,
           std::shared_ptr<Ast> right);
  virtual ~BinaryOp();
  virtual std::shared_ptr<Ast> left() const;
  virtual std::shared_ptr<Token> op() const;
  virtual std::shared_ptr<Ast> right() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::shared_ptr<Ast> left_;
  std::shared_ptr<Token> op_;
  std::shared_ptr<Ast> right_;
};

} // namespace fastype
