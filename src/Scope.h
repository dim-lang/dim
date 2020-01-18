// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <string>
#include <unordered_map>

class Scope {
public:
  static void insert(const std::string &name, std::shared_ptr<Ast> node);
  static std::shared_ptr<Ast> lookup(const std::string &name);

private:
  std::unordered_map<std::string, std::shared_ptr<Ast>> symtab_;
};
