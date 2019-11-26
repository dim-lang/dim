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
  AssignmentStatement(Sptr<Token> letToken, Ast *left, Sptr<Token> assignToken,
                      Ast *right);
  virtual ~AssignmentStatement() = default;
  virtual Ast *left() const;
  virtual Ast *right() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Token> letToken_;
  Ast *left_;
  Sptr<Token> assignToken_;
  Ast *right_;
};

} // namespace fastype
