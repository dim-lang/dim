// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>
#include <unordered_map>

//==============================

class Scope;
class GlobalScope;
class LocalScope;

//==============================

class Symbol;

class Scope : public Stringify {
public:
  Scope(const std::string &scopeName, Scope *enclosingScope = nullptr);
  virtual ~Scope() = default;
  virtual const std::string &name() const;
  virtual std::string toString() const;

  virtual void define(Symbol *symbol);
  virtual Symbol *resolve(const std::string &name);
  virtual Scope *enclosingScope() const;

  /* scope instance */
  static Scope *globalScope();
  static Scope *currentScope();
  static void push(Scope *scope);
  static void pop();

protected:
  static Scope *currentScope_;
  static Scope *globalScope_;

  std::string scopeName_;
  Scope *enclosingScope_;
  std::unordered_map<std::string, Symbol *> symbolTable_;
};

class GlobalScope : public Scope {
public:
  GlobalScope();
  virtual ~GlobalScope() = default;
};

class LocalScope : public Scope {
public:
  LocalScope(const std::string &scopeName, Scope *enclosingScope);
  virtual ~LocalScope() = default;
};
