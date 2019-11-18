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

class BinaryOp : public AstTree {
public:
  BinaryOp(Sptr<AstTree> left, Sptr<AstTree> op, Sptr<AstTree> right);
  virtual ~BinaryOp() = default;
  virtual Sptr<AstTree> left() const;
  virtual Sptr<AstTree> op() const;
  virtual Sptr<AstTree> right() const;
  virtual std::string toString() const;
  virtual std::string name() const;

private:
  Sptr<AstTree> left_;
  Sptr<AstTree> op_;
  Sptr<AstTree> right_;
};

} // namespace fastype
