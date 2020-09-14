// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include "container/Cowstr.h"

class Ast;
class Symbol;

class Nameable {
public:
  Nameable();
  Nameable(const Cowstr &name);
  virtual ~Nameable() = default;
  virtual Cowstr &name();
  virtual const Cowstr &name() const;

private:
  Cowstr name_;
};

class AstGraphNameGenerator {
public:
  virtual ~AstGraphNameGenerator() = default;
  virtual Cowstr from(const Ast *ast);

protected:
  Counter counter_;
};

class SymbolNameGenerator {
public:
  virtual ~SymbolNameGenerator() = default;
  virtual Cowstr from(const Ast *ast);
};

class IrNameGenerator {
public:
  virtual ~IrNameGenerator() = default;
  virtual Cowstr from(const Ast *ast);
  virtual Cowstr from(const Symbol *sym);
  virtual Cowstr from(const Cowstr &s1, const Cowstr &s2 = "",
                      const Cowstr &s3 = "", const Cowstr &s4 = "",
                      const Cowstr &s5 = "", const Cowstr &s6 = "",
                      const Cowstr &s7 = "", const Cowstr &s8 = "",
                      const Cowstr &s9 = "");
};
