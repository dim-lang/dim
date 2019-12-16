// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class EmptyStatement : public Ast {
public:
  EmptyStatement();
  virtual ~EmptyStatement() = default;
  virtual std::string toString() const;
  virtual Type type() const;
};

} // namespace fastype
