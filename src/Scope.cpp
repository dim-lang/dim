// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Scope.h"
#include "Log.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

Scope::Scope(const std::string &name, std::shared_ptr<Scope> enclosingScope)
    : scopeName_(name), enclosingScope_(enclosingScope) {}

const std::string &Scope::name() const { return scopeName_; }

std::string Scope::toString() const {
  return fmt::format(
      "[ @Scope scopeName_:{}, symbolTable_#size:{}, enclosingScope_:{} ]",
      scopeName_, symbolTable_.size(),
      enclosingScope_ ? enclosingScope_->toString() : "null");
}

void Scope::define(std::shared_ptr<Symbol> symbol) {
  FCHECK(symbol, "symbol {} already exist", symbol->toString());
  symbolTable_.insert(std::make_pair(symbol->name(), symbol));
}

std::shared_ptr<Symbol> Scope::resolve(const std::string &name) {
  FCHECK(symbolTable_.find(name) != symbolTable_.end(), "symbol {} not exist",
         name);
  return symbolTable_[name];
}

std::shared_ptr<Scope> Scope::enclosingScope() const { return enclosingScope_; }

void Scope::initialize() { currentScope_ = GlobalScope::instance(); }

std::shared_ptr<Scope> Scope::currentScope() { return currentScope_; }

void Scope::push(std::shared_ptr<Scope> scope) { currentScope_ = scope; }

void Scope::pop(std::shared_ptr<Scope> scope) {
  currentScope_ = scope->enclosingScope();
}
