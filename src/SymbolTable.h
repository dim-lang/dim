// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"
#include "Type.h"
#include "boost/core/noncopyable.hpp"

class SymbolTable : private boost::noncopyable {
public:
  SymbolTable();
  virtual ~SymbolTable();

  Symtab *&gss();
  const Symtab *gss() const;
  Symtab *&css();
  const Symtab *css() const;
  int symstk() const;

  Tytab *&gts();
  const Tytab *gts() const;
  Tytab *&cts();
  const Tytab *cts() const;
  int tystk() const;

  void pushSymbol(Symtab *st);
  void popSymbol();
  void pushType(Tytab *tt);
  void popType();
  void resetSymbolStack();
  void resetTypeStack();

private:
  Symtab *gss_; // global symbol scope
  Symtab *css_; // current symbol scope
  int symstk_;  // symbol stack count
  Tytab *gts_;  // global type scope
  Tytab *cts_;  // current type scope
  int tystk_;   // type stack count
};
