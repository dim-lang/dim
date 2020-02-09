// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"

class Semant {
public:
  Semant(Ast *program);
  virtual ~Semant();
  virtual void build();
  virtual void check();

private:
  virtual void buildImpl(Ast *node);
  virtual void checkImpl(Ast *node);

  Ast *program_;
  Symtab *gsym_;
  Symtab *csym_;
  Tytab *gty_;
  Tytab *cty_;
};
