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
  AssignmentStatement(Sptr<Ast> var, Sptr<Ast> expr);
  virtual ~AssignmentStatement() = default;
  virtual Sptr<Ast> var() const;
  virtual Sptr<Ast> expr() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Ast> var_;
  Sptr<Ast> expr_;
};

} // namespace fastype
