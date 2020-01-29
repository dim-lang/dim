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
  Scope(const std::string &scopeName, std::shared_ptr<Scope> enclosingScope =
                                          std::shared_ptr<Scope>(nullptr));
  virtual const std::string &name() const;
  virtual std::string toString() const;

  virtual void define(std::shared_ptr<Symbol> symbol);
  virtual std::shared_ptr<Symbol> resolve(const std::string &name);
  virtual std::shared_ptr<Scope> enclosingScope() const;

  /* global scope instance */
  static void initialize();
  static std::shared_ptr<Scope> currentScope();
  static void push(std::shared_ptr<Scope> scope);
  static void pop(std::shared_ptr<Scope> scope);

protected:
  static std::shared_ptr<Scope> currentScope_;

  std::string scopeName_;
  std::shared_ptr<Scope> enclosingScope_;
  std::unordered_map<std::string, std::shared_ptr<Symbol>> symbolTable_;
};
