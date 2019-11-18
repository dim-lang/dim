// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstTree.h"
#include "script/Token.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class IntegerConstant : public AstTree {
public:
  IntegerConstant(Sptr<AstTree> op, Sptr<AstTree> expr);
  virtual ~IntegerConstant() = default;
  virtual Sptr<AstTree> op() const;
  virtual Sptr<AstTree> expr() const;
  virtual std::string toString() const;
  virtual std::string name() const;

private:
  Sptr<AstTree> op_;
  Sptr<AstTree> expr_;
};

} // namespace fastype
