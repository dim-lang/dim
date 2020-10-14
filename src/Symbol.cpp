// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Ast.h"
#include "Counter.h"
#include "LinkedHashMap.hpp"
#include "Location.h"
#include "Log.h"
#include "Name.h"
#include "Token.h"
#include "Util.h"
#include <algorithm>
#include <cctype>

#define SYMBOL_CONSTRUCTOR                                                     \
  NameableImpl(name), LocationableImpl(location), detail::TypeableImpl(type),  \
      detail::OwnableImpl(owner)

#define TYPE_SYMBOL_CONSTRUCTOR                                                \
  NameableImpl(name), LocationableImpl(location), detail::OwnableImpl(owner)

namespace detail {

// OwnableImpl {

OwnableImpl::OwnableImpl(Scope *ownableImpl) : ownableImpl_(ownableImpl) {}

Scope *&OwnableImpl::owner() { return ownableImpl_; }

Scope *OwnableImpl::owner() const { return ownableImpl_; }

// OwnableImpl }

// TypeableImpl {

TypeableImpl ::TypeableImpl(TypeSymbol *typeableImpl)
    : typeableImpl_(typeableImpl) {}

TypeSymbol *&TypeableImpl::type() { return typeableImpl_; }

TypeSymbol *TypeableImpl::type() const { return typeableImpl_; }

// TypeableImpl }

// ScopeImpl {

// symbol api {

ScopeImpl::~ScopeImpl() {
  del(s_data_);
  del(ts_data_);
  del(sc_data_);
  // for (auto i = s_data_.begin(); i != s_data_.end(); i++) {
  //   delete i->second;
  // }
  // for (auto i = ts_data_.begin(); i != ts_data_.end(); i++) {
  //   delete i->second;
  // }
  // for (auto i = sc_data_.begin(); i != sc_data_.end(); i++) {
  //   delete i->second;
  // }
}

void ScopeImpl::s_define(Symbol *symbol) {
  LOG_ASSERT(symbol, "symbol must not null");
  LOG_ASSERT(s_data_.find(symbol->name()) == s_data_.end(),
             "symbol {} already exist", symbol->name());
  s_data_.insert(symbol->name(), symbol);
}

Symbol *ScopeImpl::s_resolve(const Cowstr &name) const {
  s_const_iterator it = s_data_.find(name);
  if (it != s_data_.end()) {
    return it->second;
  }
  return owner() ? owner()->s_resolve(name) : nullptr;
}

bool ScopeImpl::s_contains(const Cowstr &name) const {
  return s_data_.find(name) != s_data_.end();
}

bool ScopeImpl::s_empty() const { return s_data_.empty(); }

int ScopeImpl::s_size() const { return s_data_.size(); }

Scope::s_iterator ScopeImpl::s_begin() { return s_data_.begin(); }

Scope::s_const_iterator ScopeImpl::s_begin() const { return s_data_.begin(); }

Scope::s_const_iterator ScopeImpl::s_cbegin() const { return s_data_.cbegin(); }

Scope::s_iterator ScopeImpl::s_end() { return s_data_.end(); }

Scope::s_const_iterator ScopeImpl::s_end() const { return s_data_.end(); }

Scope::s_const_iterator ScopeImpl::s_cend() const { return s_data_.end(); }

// symbol api }

// type symbol api {

void ScopeImpl::ts_define(TypeSymbol *symbol) {
  LOG_ASSERT(symbol, "symbol must not null");
  LOG_ASSERT(ts_data_.find(symbol->name()) == ts_data_.end(),
             "symbol {} already exist", symbol->name());
  ts_data_.insert(symbol->name(), symbol);
}

TypeSymbol *ScopeImpl::ts_resolve(const Cowstr &name) const {
  ts_const_iterator it = ts_data_.find(name);
  if (it != ts_data_.end()) {
    return it->second;
  }
  return owner() ? owner()->ts_resolve(name) : nullptr;
}

bool ScopeImpl::ts_contains(const Cowstr &name) const {
  return ts_data_.find(name) != ts_data_.end();
}

bool ScopeImpl::ts_empty() const { return ts_data_.empty(); }

int ScopeImpl::ts_size() const { return ts_data_.size(); }

Scope::ts_iterator ScopeImpl::ts_begin() { return ts_data_.begin(); }

Scope::ts_const_iterator ScopeImpl::ts_begin() const {
  return ts_data_.begin();
}

Scope::ts_const_iterator ScopeImpl::ts_cbegin() const {
  return ts_data_.cbegin();
}

Scope::ts_iterator ScopeImpl::ts_end() { return ts_data_.end(); }

Scope::ts_const_iterator ScopeImpl::ts_end() const { return ts_data_.end(); }

Scope::ts_const_iterator ScopeImpl::ts_cend() const { return ts_data_.cend(); }

// type symbol api }

// sub scope api {

void ScopeImpl::subscope_define(Scope *scope) {
  LOG_ASSERT(scope, "scope must not null");
  LOG_ASSERT(!sc_data_.contains(scope->name()), "scope already exist: {}",
             scope->name());
  sc_data_.insert(scope->name(), scope);
}

Scope *ScopeImpl::subscope_resolve(const Cowstr &name) const {
  auto it = sc_data_.find(name);
  return it == sc_data_.end() ? nullptr : it->second;
}

bool ScopeImpl::subscope_contains(const Cowstr &name) const {
  return sc_data_.find(name) != sc_data_.end();
}

bool ScopeImpl::subscope_empty() const { return sc_data_.empty(); }

int ScopeImpl::subscope_size() const { return sc_data_.size(); }

Scope::sc_iterator ScopeImpl::subscope_begin() { return sc_data_.begin(); }

Scope::sc_const_iterator ScopeImpl::subscope_begin() const {
  return sc_data_.begin();
}

Scope::sc_const_iterator ScopeImpl::subscope_cbegin() const {
  return sc_data_.cbegin();
}

Scope::sc_iterator ScopeImpl::subscope_end() { return sc_data_.end(); }

Scope::sc_const_iterator ScopeImpl::subscope_end() const {
  return sc_data_.end();
}

Scope::sc_const_iterator ScopeImpl::subscope_cend() const {
  return sc_data_.cend();
}

// sub scope api }

// ScopeImpl }

} // namespace detail

// TypeSymbol {

TypeSymbol *TypeSymbol::ts_byte() {
  static TypeSymbol *ts(new Ts_Plain("byte"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_ubyte() {
  static TypeSymbol *ts(new Ts_Plain("ubyte"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_short() {
  static TypeSymbol *ts(new Ts_Plain("short"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_ushort() {
  static TypeSymbol *ts(new Ts_Plain("ushort"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_int() {
  static TypeSymbol *ts(new Ts_Plain("int"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_uint() {
  static TypeSymbol *ts(new Ts_Plain("uint"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_long() {
  static TypeSymbol *ts(new Ts_Plain("long"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_ulong() {
  static TypeSymbol *ts(new Ts_Plain("ulong"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_float() {
  static TypeSymbol *ts(new Ts_Plain("float"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_double() {
  static TypeSymbol *ts(new Ts_Plain("double"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_char() {
  static TypeSymbol *ts(new Ts_Plain("char"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_boolean() {
  static TypeSymbol *ts(new Ts_Plain("boolean"));
  return ts;
}

TypeSymbol *TypeSymbol::ts_void() {
  static TypeSymbol *ts(new Ts_Plain("void"));
  return ts;
}

// TypeSymbol {

// scope {

// symbol {

S_Var::S_Var(const Cowstr &name, const Location &location, TypeSymbol *type,
             Scope *owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Var::kind() const { return SymbolKind::Var; }

S_Func::S_Func(const Cowstr &name, const Location &location, TypeSymbol *type,
               Scope *owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Func::kind() const { return SymbolKind::Func; }

S_Param::S_Param(const Cowstr &name, const Location &location, TypeSymbol *type,
                 Scope *owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Param::kind() const { return SymbolKind::Param; }

S_Field::S_Field(const Cowstr &name, const Location &location, TypeSymbol *type,
                 Scope *owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Field::kind() const { return SymbolKind::Field; }

S_Method::S_Method(const Cowstr &name, const Location &location,
                   TypeSymbol *type, Scope *owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Method::kind() const { return SymbolKind::Method; }

// symbol }

// type symbol {

Ts_Plain::Ts_Plain(const Cowstr &name)
    : NameableImpl(name),
      LocationableImpl(Location(Position(-1, -1), Position(-1, -1))),
      detail::OwnableImpl(nullptr) {}

TypeSymbolKind Ts_Plain::kind() const { return TypeSymbolKind::Plain; }

Ts_Class::Ts_Class(const Cowstr &name, const Location &location, Scope *owner)
    : TYPE_SYMBOL_CONSTRUCTOR {}

TypeSymbolKind Ts_Class::kind() const { return TypeSymbolKind::Class; }

Ts_Func::Ts_Func(const std::vector<TypeSymbol *> a_params, TypeSymbol *a_result,
                 const Location &location, Scope *owner)
    : NameableImpl(generateName(a_params, a_result)),
      LocationableImpl(location), detail::OwnableImpl(owner), params(a_params),
      result(a_result) {}

TypeSymbolKind Ts_Func::kind() const { return TypeSymbolKind::Func; }

Cowstr Ts_Func::generateName(const std::vector<TypeSymbol *> &params,
                             TypeSymbol *result) {
  std::stringstream ss;
  ss << "(";
  for (int i = 0; i < (int)params.size(); i++) {
    ss << params[i]->name();
    if (i < (int)params.size() - 1) {
      ss << ",";
    }
  }
  ss << ")=>" << result->name();
  return ss.str();
}

// type symbol }

// scope {

S_Local::S_Local(const Cowstr &name, const Location &location, Scope *owner)
    : TYPE_SYMBOL_CONSTRUCTOR {}

ScopeKind S_Local::kind() const { return ScopeKind::Local; }

S_Global::S_Global(const Cowstr &name, const Location &location)
    : NameableImpl(name),
      LocationableImpl(location), detail::OwnableImpl(nullptr) {}

ScopeKind S_Global::kind() const { return ScopeKind::Global; }

// scope }
