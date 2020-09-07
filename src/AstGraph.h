// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "graphviz/cgraph.h"
#include <memory>
#include <string>
class Ast;

class AstGraph {
public:
  AstGraph(std::shared_ptr<Ast> ast);
  virtual ~AstGraph();
  virtual int write(const std::string &output);

private:
  Agraph_t *g;
};
