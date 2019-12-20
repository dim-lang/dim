// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <vector>

namespace fastype {

class ParameterList : public Ast {
public:
  ParameterList(const std::vector<std::shared_ptr<Token>> tokenList);
  virtual ~ParameterList() = default;
  virtual int size() const;
  virtual std::shared_ptr<Ast> get(int index) const;
  virtual std::string toString() const;
  virtual int type() const;

private:
  std::vector<std::shared_ptr<Token>> tokenList_;
};

} // namespace fastype
