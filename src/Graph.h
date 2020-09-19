// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "container/Cowstr.h"
#include <memory>
class Ast;
class Scope;

class AstGraph {
public:
  AstGraph(Ast *ast);
  virtual ~AstGraph() = default;
  virtual int draw(const Cowstr &output);

private:
  Ast *ast_;
};

class SymbolGraph {
public:
  SymbolGraph(std::shared_ptr<Scope> scope);
  virtual ~SymbolGraph() = default;
  virtual int draw(const Cowstr &output);

private:
  std::shared_ptr<Scope> scope_;
};
