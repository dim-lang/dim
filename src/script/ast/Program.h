// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/Ast.h"
#include "script/Token.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>
#include <vector>

namespace fastype {

class Program : public Ast {
public:
  Program(Sptr<Ast> node);
  virtual ~Program() = default;
  virtual Sptr<Ast> statementList() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Sptr<Ast> statementList_;
};

} // namespace fastype
