// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstList.h"
#include "script/Token.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class BinaryExpr : public AstList {
public:
  BinaryExpr(const std::vector<Sptr<AstTree>> &children);
  virtual ~BinaryExpr() = default;
  virtual icu::UnicodeString op() const;
  virtual Sptr<AstTree> left() const;
  virtual Sptr<AstTree> right() const;
};

} // namespace fastype
