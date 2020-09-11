// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"
#include "boost/core/noncopyable.hpp"

class SymbolTable : private boost::noncopyable {
public:
  SymbolTable();
  virtual ~SymbolTable();

  // scope api
  virtual void s_define(const Symbol *s, const TypeSymbol *ts);
  virtual const SymbolData &s_resolve(const Name &name) const;
  virtual void ts_define(const TypeSymbol *symbol);
  virtual const TypeSymbolData &ts_resolve(const Name &name) const;

  virtual bool contains(const Name &name) const;
  virtual bool empty() const;
  virtual int size() const;

  virtual Scope::s_iterator s_begin();
  virtual Scope::s_const_iterator s_begin() const;
  virtual Scope::s_iterator s_end();
  virtual Scope::s_const_iterator s_end() const;

  virtual Scope::ts_iterator ts_begin();
  virtual Scope::ts_const_iterator ts_begin() const;
  virtual Scope::ts_iterator ts_end();
  virtual Scope::ts_const_iterator ts_end() const;

  void push(Symbol *s);
  void pop();
  int depth() const;

private:
  Symbol *scope_; // scope
  int depth_;     // symbol stack depth
};
