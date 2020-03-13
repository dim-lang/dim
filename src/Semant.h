// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "Type.h"

class Semantic {
public:
  void buildSemantic(Symtab *&gss, Symtab *&css, const Ast *node);
  void checkSemantic(Tytab *&gts, Tytab *&cts, const Ast *node);
};

class Semant {
public:
  Semant(Ast *translateUnit);
  virtual ~Semant();
  virtual void build();
  virtual void check();
  virtual Symtab *globalSymbolTable();
  virtual Tytab *globalTypeTable();

private:
  virtual void buildImpl(Ast *node);
  virtual void checkImpl(Ast *node);

  Ast *translateUnit_;
  Symtab *gsym_;
  Symtab *csym_;
  Tytab *gty_;
  Tytab *cty_;
};
