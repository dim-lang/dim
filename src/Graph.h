// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <memory>
#include <string>
class Ast;

class Graph {
public:
  static int drawAst(std::shared_ptr<Ast> ast, const std::string &output);
};
