// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Log.h"

// symbol {

Symbol::Symbol(const Cowstr &name, const Location &location,
               std::shared_ptr<Symbol> owner)
    : Nameable(name), Locationable(location), owner_(owner) {}

std::shared_ptr<Symbol> Symbol::owner() const { return owner_; }

// symbol }

// type symbol {

TypeSymbol::TypeSymbol(const Cowstr &name, const Location &location,
                       std::shared_ptr<Symbol> owner)
    : Nameable(name), Locationable(location), owner_(owner) {}

std::shared_ptr<Symbol> TypeSymbol::owner() const { return owner_; }

// type symbol }

// scope {

Scope::Scope(const Cowstr &name, const Location &location,
             std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

void Scope::define(std::shared_ptr<Symbol> s, std::shared_ptr<TypeSymbol> ts) {
  LOG_ASSERT(s, "s must not null");
  LOG_ASSERT(ts, "ts must not null");
  smap_.insert(s->name(), SymbolData(s, ts));
  tsmap_.insert(ts->name(), TypeSymbolData(ts));
}

SymbolData Scope::s_resolve(const Cowstr &name) const {
  if (smap_.contains(name)) {
    return smap_.find(name)->second;
  }
  return owner() ? std::static_pointer_cast<Scope>(owner())->s_resolve(name)
                 : SymbolData(nullptr, nullptr);
}

TypeSymbolData Scope::ts_resolve(const Cowstr &name) const {
  if (tsmap_.contains(name)) {
    return tsmap_.find(name)->second;
  }
  return owner() ? std::static_pointer_cast<Scope>(owner())->ts_resolve(name)
                 : TypeSymbolData(nullptr);
}

bool Scope::s_resolveContains(const Cowstr &name) const {
  if (smap_.contains(name)) {
    return true;
  }
  return owner()
             ? std::static_pointer_cast<Scope>(owner())->s_resolveContains(name)
             : false;
}

bool Scope::ts_resolveContains(const Cowstr &name) const {
  if (tsmap_.contains(name)) {
    return true;
  }
  return owner()
             ? std::static_pointer_cast<Scope>(owner())->s_resolveContains(name)
             : false;
}

bool Scope::s_contains(const Cowstr &name) const {
  return smap_.contains(name);
}

bool Scope::ts_contains(const Cowstr &name) const {
  return tsmap_.contains(name);
}

bool Scope::s_empty() const { return smap_.empty(); }

bool Scope::ts_empty() const { return tsmap_.empty(); }

int Scope::s_size() const { return (int)smap_.size(); }

int Scope::ts_size() const { return (int)tsmap_.size(); }

Scope::s_iterator Scope::s_begin() { return smap_.begin(); }

Scope::s_const_iterator Scope::s_begin() const { return smap_.begin(); }

Scope::s_const_iterator Scope::s_cbegin() const { return smap_.cbegin(); }

Scope::s_iterator Scope::s_end() { return smap_.end(); }

Scope::s_const_iterator Scope::s_end() const { return smap_.end(); }

Scope::s_const_iterator Scope::s_cend() const { return smap_.cend(); }

Scope::ts_iterator Scope::ts_begin() { return tsmap_.begin(); }

Scope::ts_const_iterator Scope::ts_begin() const { return tsmap_.begin(); }

Scope::ts_const_iterator Scope::ts_cbegin() const { return tsmap_.cbegin(); }

Scope::ts_iterator Scope::ts_end() { return tsmap_.end(); }

Scope::ts_const_iterator Scope::ts_end() const { return tsmap_.end(); }

Scope::ts_const_iterator Scope::ts_cend() const { return tsmap_.cend(); }

// scope }

S_Var::S_Var(const Cowstr &name, const Location &location,
             std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

SymbolKind S_Var::kind() const { return SymbolKind::Var; }

S_Func::S_Func(const Cowstr &name, const Location &location,
               std::shared_ptr<Symbol> owner,
               const std::vector<std::shared_ptr<Symbol>> &a_params)
    : Scope(name, location, owner), params(a_params) {}

SymbolKind S_Func::kind() const { return SymbolKind::Func; }

S_Param::S_Param(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

SymbolKind S_Param::kind() const { return SymbolKind::Param; }

S_Class::S_Class(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner,
                 const std::vector<std::shared_ptr<Symbol>> &a_fields,
                 const std::vector<std::shared_ptr<Symbol>> &a_methods)
    : Scope(name, location, owner), fields(a_fields), methods(a_methods) {}

SymbolKind S_Class::kind() const { return SymbolKind::Class; }

S_Field::S_Field(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

SymbolKind S_Field::kind() const { return SymbolKind::Field; }

S_Method::S_Method(const Cowstr &name, const Location &location,
                   std::shared_ptr<Symbol> owner,
                   const std::vector<std::shared_ptr<Symbol>> &a_params)
    : Scope(name, location, owner), params(a_params) {}

SymbolKind S_Method::kind() const { return SymbolKind::Method; }

S_Local::S_Local(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : Scope(name, location, owner) {}

SymbolKind S_Local::kind() const { return SymbolKind::Local; }

SymbolKind S_Global::kind() const { return SymbolKind::Global; }

std::shared_ptr<S_Global> S_Global::global(new S_Global());

S_Global::S_Global() : Scope("global", Location(), nullptr) {}

Ts_Builtin::Ts_Builtin(const Cowstr &name, const Location &location,
                       std::shared_ptr<Symbol> owner)
    : TypeSymbol(name, location, owner) {}

TypeSymbolKind Ts_Builtin::kind() const { return TypeSymbolKind::Builtin; }

Ts_Class::Ts_Class(const Cowstr &name, const Location &location,
                   std::shared_ptr<Symbol> owner,
                   const std::vector<std::shared_ptr<Symbol>> &a_fields,
                   const std::vector<std::shared_ptr<Symbol>> &a_methods)
    : TypeSymbol(name, location, owner), fields(a_fields), methods(a_methods) {}

TypeSymbolKind Ts_Class::kind() const { return TypeSymbolKind::Class; }

Ts_Func::Ts_Func(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner,
                 const std::vector<std::shared_ptr<TypeSymbol>> &a_params,
                 std::shared_ptr<TypeSymbol> a_result)
    : TypeSymbol(name, location, owner), params(a_params), result(a_result) {}

TypeSymbolKind Ts_Func::kind() const { return TypeSymbolKind::Func; }
