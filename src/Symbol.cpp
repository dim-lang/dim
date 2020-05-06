// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"
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
  LOG_ASSERT(sym, "symbol is null");
  LOG_ASSERT(hashtab_.find(sym->name()) == hashtab_.end(),
             "symbol {} already exist", sym->name());
  hashtab_.insert(std::make_pair(sym->name(), sym));
}

Symbol *Symtab::resolve(const std::string &name) {
  LOG_ASSERT(name.length() > 0, "name#length {} > 0", name.length());
  if (hashtab_.find(name) != hashtab_.end())
    return hashtab_[name];
  if (enclosingScope_)
    return enclosingScope_->resolve(name);
  return nullptr;
}

Symtab *Symtab::enclosingScope() { return enclosingScope_; }

std::string Symtab::toString() const {
  return fmt::format("[ @{} name_:{}, enclosingScope_:{}, hashtab_#size:{} ]",
                     stringify(), name(), enclosingScope_->name(),
                     hashtab_.size());
}

Symtab::Iterator Symtab::begin() { return hashtab_.begin(); }

Symtab::CIterator Symtab::begin() const { return hashtab_.begin(); }

Symtab::Iterator Symtab::end() { return hashtab_.end(); }

Symtab::CIterator Symtab::end() const { return hashtab_.end(); }

int Symtab::size() const { return (int)hashtab_.size(); }

bool Symtab::empty() const { return hashtab_.empty(); }

VariableSymbol::VariableSymbol(const std::string &variableName)
    : variableName_(variableName) {}

std::string VariableSymbol::name() const { return variableName_; }

SymType VariableSymbol::type() const { return SymType::Variable; }

FunctionArgumentSymbol::FunctionArgumentSymbol(
    const std::string &functionArgumentName)
    : functionArgumentName_(functionArgumentName) {}

std::string FunctionArgumentSymbol::name() const {
  return functionArgumentName_;
}

SymType FunctionArgumentSymbol::type() const {
  return SymType::FunctionArgument;
}

FunctionSymbol::FunctionSymbol(const std::string &functionName,
                               Symtab *enclosingScope)
    : Symtab(enclosingScope), functionName_(functionName) {}

std::string FunctionSymbol::name() const { return functionName_; }

SymType FunctionSymbol::type() const { return SymType::Function; }

std::string FunctionSymbol::stringify() const { return "FunctionSymbol"; }

ClassSymbol::ClassSymbol(const std::string &className, Symtab *enclosingScope)
    : Symtab(enclosingScope), className_(className) {}

std::string ClassSymbol::name() const { return className_; }

SymType ClassSymbol::type() const { return SymType::Class; }

std::string ClassSymbol::stringify() const { return "ClassSymbol"; }

GlobalSymtab::GlobalSymtab() : Symtab(nullptr) {}

std::string GlobalSymtab::name() const {
  static std::string globalSymtabName = "GlobalSymtab";
  return globalSymtabName;
}

SymType GlobalSymtab::type() const { return SymType::Global; }

std::string GlobalSymtab::stringify() const { return "GlobalSymtab"; }

LocalSymtab::LocalSymtab(const std::string &localSymtabName,
                         Symtab *enclosingScope)
    : Symtab(enclosingScope), localSymtabName_(localSymtabName) {}

std::string LocalSymtab::name() const { return localSymtabName_; }

SymType LocalSymtab::type() const { return SymType::Local; }

std::string LocalSymtab::stringify() const { return "LocalSymtab"; }
