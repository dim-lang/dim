// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Scope.h"
#include "Log.h"
#include <algorithm>
#include <utility>

// Scope::Scope(std::shared_ptr<Scope> father) : father_(father) {}

// Scope::~Scope() { symtab_.clear(); }

void Scope::define(const std::string &name, std::shared_ptr<Ast> node) {
  FCHECK(symtab_.find(name) == symtab_.end(), "symbol name: {} already exist",
         name);
  symtab_.insert(std::make_pair(name, node));
}

std::shared_ptr<Ast> Scope::lookup(const std::string &name) {
  FCHECK(symtab_.find(name) != symtab_.end(), "symbol name: {} not found",
         name);
  return symtab_[name];
}
