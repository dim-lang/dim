// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <memory>
#include <string>
#include <unordered_map>

class Scope {
public:
  // Scope(std::shared_ptr<Scope> father = std::shared_ptr<Scope>(nullptr));
  // virtual ~Scope();

  static void define(const std::string &name, std::shared_ptr<Ast> node);
  static std::shared_ptr<Ast> lookup(const std::string &name);

protected:
  static std::unordered_map<std::string, std::shared_ptr<Ast>> symtab_;
  // std::shared_ptr<Scope> father_;
};
