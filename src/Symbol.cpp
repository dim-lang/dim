// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Log.h"
#include "config/Header.h"
#include <cctype>
#include <numeric>
#include <utility>

void Symbol::push(Symtab *&global, Symtab *&current, Symtab *s) {
  current = s;
  if (!global) {
    global = s;
  }
}

void Symbol::pop(Symtab *&global, Symtab *&current) {
  current = current->enclosingScope();
}

Symtab::Symtab(Symtab *enclosingScope) : enclosingScope_(enclosingScope) {}

void Symtab::define(Symbol *sym) {
  CASSERT(sym, "symbol is null");
  CASSERT(hashtab_.find(sym->name()) == hashtab_.end(),
          "symbol {} already exist", sym->name());
  hashtab_.insert(std::make_pair(sym->name(), sym));
}

Symbol *Symtab::resolve(const std::string &name) {
  CASSERT(name.length() > 0, "name#length <= 0");
  CASSERT(hashtab_.find(name) != hashtab_.end(), "symbol {} not exist", name);
  return hashtab_[name];
}

Symtab *Symtab::enclosingScope() { return enclosingScope_; }

std::string Symtab::toString() const {
  return fmt::format("[ @{} name_:{}, enclosingScope_:{}, hashtab_#size:{} ]",
                     stringify(), name(), enclosingScope_->name(),
                     hashtab_.size());
}

VariableSymbol::VariableSymbol(const std::string &variableName)
    : variableName_(variableName) {}

const std::string &VariableSymbol::name() const { return variableName_; }

FunctionSymbol::FunctionSymbol(const std::string &functionName,
                               Symtab *enclosingScope)
    : Symtab(enclosingScope), functionName_(functionName) {}

const std::string &FunctionSymbol::name() const { return functionName_; }

std::string FunctionSymbol::stringify() const { return "FunctionSymbol"; }

ClassSymbol::ClassSymbol(const std::string &className, Symtab *enclosingScope)
    : Symtab(enclosingScope), className_(className) {}

const std::string &ClassSymbol::name() const { return className_; }

std::string ClassSymbol::stringify() const { return "ClassSymbol"; }

const std::string &GlobalSymtab::name() const {
  static std::string globalSymtabName = "GlobalSymtab";
  return globalSymtabName;
}

LocalSymtab::LocalSymtab(const std::string &localSymtabName,
                         Symtab *enclosingScope)
    : Symtab(enclosingScope), localSymtabName_(localSymtabName) {}

const std::string &LocalSymtab::name() const { return localSymtabName_; }
