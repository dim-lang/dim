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

class AssignmentStatement : public Ast {
public:
  AssignmentStatement(Ast *var, Ast *expr);
  virtual ~AssignmentStatement() = default;
  virtual Ast *var() const;
  virtual Ast *expr() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Ast *var_;
  Ast *expr_;
};

} // namespace fastype
