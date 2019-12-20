// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <vector>

namespace fastype {

class FunctionDeclaration : public Ast {
public:
  FunctionDeclaration(std::shared_ptr<Ast> functionId,
                      const std::vector<std::shared_ptr<Ast>> &varList,
                      std::shared_ptr<Ast> compoundStatement);
  virtual ~FunctionDeclaration() = default;
  virtual std::shared_ptr<Ast> functionId() const;
  virtual const std::vector<std::shared_ptr<Ast>> &varList() const;
  virtual std::shared_ptr<Ast> compoundStatement() const;
  virtual std::string toString() const;
  virtual int type() const;

private:
  std::shared_ptr<Ast> functionId_;           // function identifier
  std::vector<std::shared_ptr<Ast>> varList_; // variable list
  std::shared_ptr<Ast> compoundStatement_;    // compound statement
};

} // namespace fastype
