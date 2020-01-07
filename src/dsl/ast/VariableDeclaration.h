// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class VariableDeclaration : public Ast {
public:
  VariableDeclaration(std::shared_ptr<Ast> var, std::shared_ptr<Ast> expr);
  virtual ~VariableDeclaration() = default;
  virtual std::shared_ptr<Ast> var() const;
  virtual std::shared_ptr<Ast> expr() const;
  virtual std::string toString() const;
  virtual int type() const;

private:
  std::shared_ptr<Ast> var_;
  std::shared_ptr<Ast> expr_;
};

} // namespace fastype