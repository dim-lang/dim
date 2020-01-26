// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include <memory>

class Interpreter {
public:
  Interpreter(Ast *program);
  virtual ~Interpreter() = default;

private:
  std::shared_ptr<Ast> program_;
};
