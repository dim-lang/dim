// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Scope.h"
#include "Log.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

Scope::Scope(const std::string &name, Scope *enclosingScope)
    : scopeName_(name), enclosingScope_(enclosingScope) {}

Scope::~Scope() {}

const std::string &Scope::name() const { return scopeName_; }

std::string Scope::toString() const {
  return fmt::format(
      "[ @Scope scopeName_:{}, symbolTable_#size:{}, enclosingScope_:{} ]",
      scopeName_, symbolTable_.size(),
      enclosingScope_ ? enclosingScope_->toString() : "null");
}

void Scope::define(Symbol *symbol) {
  LOG_CHECK(symbol, "symbol {} already exist",
            symbol ? symbol->toString() : "null");
  symbolTable_.insert(std::make_pair(symbol->name(), symbol));
}

Symbol *Scope::resolve(const std::string &name) {
  LOG_CHECK(symbolTable_.find(name) != symbolTable_.end(),
            "symbol {} not exist", name);
  return symbolTable_[name];
}

Scope *Scope::enclosingScope() const { return enclosingScope_; }

//========================== static functions

Scope *Scope::currentScope_ = nullptr;

Scope *Scope::globalScope_ = nullptr;

Scope *Scope::globalScope() { return globalScope_; }

void Scope::resetGlobalScope(Scope *scope) { std::swap(globalScope_, scope); }

Scope *Scope::currentScope() { return currentScope_; }

void Scope::push(Scope *scope) { std::swap(currentScope_, scope); }

void Scope::pop() {
  LOG_CHECK(currentScope_, "currentScope_ is null: {}", (void *)currentScope_);
  std::swap(currentScope_, currentScope_->enclosingScope());
}

//========================== static functions

GlobalScope::GlobalScope(const std::string &scopeName) : Scope(scopeName) {}

GlobalScope::~GlobalScope() {}

LocalScope::LocalScope(const std::string &scopeName, Scope *enclosingScope)
    : Scope(scopeName, enclosingScope) {}

LocalScope::~LocalScope() {}
