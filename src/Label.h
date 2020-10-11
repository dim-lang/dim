// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Cowstr.h"

class Label {
public:
  Label(const Cowstr &prefix, const Cowstr &delimiter);
  virtual ~Label() = default;

  virtual Cowstr encode(Ast *ast);
  virtual Cowstr decode(const Cowstr &value);

private:
  Cowstr prefix_;
  Cowstr delimiter_;
};
