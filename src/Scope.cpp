// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Scope.h"
#include "Log.h"
#include "config/Header.h"
#include <algorithm>
#include <utility>

Scope::Scope(const std::string &name, Scope *enclosingScope)
    : scopeName_(name), enclosingScope_(enclosingScope) {}

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
  LOG_CHECK(symbolTable_.find(name) != symbolTable_.end(), "symbol {} not exist",
         name);
  return symbolTable_[name];
}

Scope *Scope::enclosingScope() const { return enclosingScope_; }

void Scope::initialize() { currentScope_ = GlobalScope::instance(); }

Scope *Scope::currentScope() { return currentScope_; }

void Scope::push(Scope *scope) { currentScope_ = scope; }

void Scope::pop() { currentScope_ = currentScope_->enclosingScope(); }
