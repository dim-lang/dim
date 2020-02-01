// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Scope.h"
#include "Log.h"
#include "Symbol.h"
#include "config/Header.h"
#include <algorithm>
#include <sstream>
#include <utility>

Scope::Scope(const std::string &name, Scope *enclosingScope)
    : scopeName_(name), enclosingScope_(enclosingScope) {
  define(BuiltinTypeSymbol::i64Instance());
  define(BuiltinTypeSymbol::f64Instance());
  define(BuiltinTypeSymbol::stringInstance());
  define(BuiltinTypeSymbol::booleanInstance());
  define(BuiltinTypeSymbol::funcInstance());
  define(BuiltinTypeSymbol::classInstance());
}

Scope::~Scope() {
  enclosingScope_ = nullptr;
  symbolTable_.clear();
}

const std::string &Scope::name() const { return scopeName_; }

std::string Scope::toString() const {
  std::stringstream ss;
  ss << fmt::format(
      "[ @Scope scopeName_:{}, enclosingScope_#name:{}, size:{}, ", scopeName_,
      enclosingScope_ ? enclosingScope_->name() : "null", symbolTable_.size());
  int c = 0;
  for (auto i = symbolTable_.begin(); i != symbolTable_.end(); i++) {
    ss << fmt::format("{}:{}", c++, i->first);
    if (c < (int)symbolTable_.size()) {
      ss << ", ";
    }
  }
  ss << " ]";
  return ss.str();
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

Scope *Scope::currentScope() { return currentScope_; }

void Scope::push(Scope *scope) {
  currentScope_ = scope;
  if (!globalScope_) {
    globalScope_ = scope;
  }
}

void Scope::pop() {
  LOG_CHECK(currentScope_, "currentScope_ is null: {}", (void *)currentScope_);
  if (currentScope_) {
    Scope *save = currentScope_;
    currentScope_ = currentScope_->enclosingScope();
    delete save;
  }
  if (!currentScope_) {
    // delete globalScope_;
    globalScope_ = nullptr;
  }
}

//========================== static functions

GlobalScope::GlobalScope() : Scope("GlobalScope") {}

GlobalScope::~GlobalScope() {}

LocalScope::LocalScope(const std::string &scopeName, Scope *enclosingScope)
    : Scope(scopeName, enclosingScope) {}

LocalScope::~LocalScope() {}
