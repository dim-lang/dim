// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <string>

#define FS_VARIABLE 101
#define FS_FUNCTION 102

class Symbol {
public:
  Symbol(const std::string &identifier,
         std::shared_ptr<AstVariableDeclaration> variable,
         std::shared_ptr<AstFunctionDeclaration> func);
  virtual ~Symbol() = default;
  virtual int type() const;

private:
  std::string identifier_;
  std::shared_ptr<AstVariableDeclaration> variable_;
  std::shared_ptr<AstFunctionDeclaration> function_;
};
