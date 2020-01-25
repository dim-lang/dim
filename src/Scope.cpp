// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Scope.h"
#include "Log.h"
#include <algorithm>
#include <utility>

void Scope::define(std::shared_ptr<Symbol> symbol) {
  FCHECK(symbol, "symbol {} already exist", symbol->toString());
  symbolTable_.insert(std::make_pair(symbol->name(), symbol));
}

std::shared_ptr<Symbol> Scope::resolve(const std::string &name) {
  FCHECK(symbolTable_.find(name) != symbolTable_.end(), "symbol {} not exist",
         name);
  return symbolTable_[name];
}

void Scope::setEnclosingScope(std::shared_ptr<Scope> scope) {
  enclosingScope_ = scope;
}

std::shared_ptr<Scope> Scope::enclosingScope() const { return enclosingScope_; }

void Scope::initialize() { currentScope_ = GlobalScope::instance(); }

std::shared_ptr<Scope> Scope::currentScope() { return currentScope_; }

void Scope::pushScope(std::shared_ptr<Scope> scope) {
  scope->setEnclosingScope(currentScope_);
  currentScope_ = scope;
}

void Scope::popScope(std::shared_ptr<Scope> scope) {
  currentScope_ = scope->enclosingScope();
}
