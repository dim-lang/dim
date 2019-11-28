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

class VariableDeclaration : public Ast {
public:
  VariableDeclaration(const std::vector<Sptr<Ast>> &children);
  virtual ~VariableDeclaration() = default;
  virtual int size() const;
  virtual Sptr<Ast> get(int i) const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::vector<Sptr<Ast>> children_;
};

} // namespace fastype
