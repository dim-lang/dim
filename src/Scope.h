// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <memory>
#include <string>
#include <unordered_map>

class Scope {
public:
  Scope(std::shared_ptr<Scope> father = std::shared_ptr<Scope>(nullptr));
  virtual ~Scope();

  virtual void define(const std::string &name, std::shared_ptr<Ast> node);
  virtual std::shared_ptr<Ast> lookup(const std::string &name);

protected:
  std::unordered_map<std::string, std::shared_ptr<Ast>> symtab_;
  std::shared_ptr<Scope> father_;
};
