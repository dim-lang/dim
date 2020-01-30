// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>
#include <unordered_map>

class Symbol;

class Scope : public Stringify {
public:
  Scope(const std::string &scopeName, Scope *enclosingScope = nullptr);
  virtual ~Scope();
  virtual const std::string &name() const;
  virtual std::string toString() const;

  virtual void define(Symbol *symbol);
  virtual Symbol *resolve(const std::string &name);
  virtual Scope *enclosingScope() const;

  /* global scope instance */
  static void initialize();
  static Scope *currentScope();
  static void push(Scope *scope);
  static void pop();

protected:
  static Scope *currentScope_;

  std::string scopeName_;
  Scope *enclosingScope_;
  std::unordered_map<std::string, Symbol *> symbolTable_;
};
