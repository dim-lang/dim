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

class Empty : public Ast {
public:
  Empty();
  virtual ~Empty() = default;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;
};

} // namespace fastype
