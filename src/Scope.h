// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <memory>
#include <string>
#include <unordered_map>

class Symbol;

class Scope : public Stringify {
public:
  virtual std::string name() const = 0;
  virtual std::string toString() const = 0;

  virtual void define(std::shared_ptr<Symbol> symbol);
  virtual std::shared_ptr<Symbol> resolve(const std::string &name);
  virtual void setEnclosingScope(std::shared_ptr<Scope> scope);
  virtual std::shared_ptr<Scope> enclosingScope() const;

  static void initialize();
  static std::shared_ptr<Scope> currentScope();
  static void pushScope(std::shared_ptr<Scope> scope);
  static void popScope(std::shared_ptr<Scope> scope);

private:
  static std::shared_ptr<Scope> currentScope_;

  std::shared_ptr<Scope> enclosingScope_;
  std::unordered_map<std::string, std::shared_ptr<Symbol>> symbolTable_;
};
