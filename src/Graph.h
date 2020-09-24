// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include "container/Cowstr.h"
#include <memory>
class Ast;
class Scope;

class Graph : private boost::noncopyable {
public:
  virtual ~Graph() = default;
  virtual int draw(const Cowstr &output) const = 0;
};

class AstGraph : public Graph {
public:
  AstGraph(Ast *ast);
  virtual ~AstGraph() = default;
  virtual int draw(const Cowstr &output) const;

private:
  Ast *ast_;
};

class SymbolGraph : public Graph {
public:
  SymbolGraph(std::shared_ptr<Scope> scope);
  virtual ~SymbolGraph() = default;
  virtual int draw(const Cowstr &output) const;

private:
  std::shared_ptr<Scope> scope_;
};
