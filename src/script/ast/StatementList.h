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

class StatementList : public Ast {
public:
  StatementList(const std::vector<Ast *> &children);
  virtual ~StatementList() = default;
  virtual int size() const;
  virtual Ast *get(int i) const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::vector<Ast *> children_;
};

} // namespace fastype
