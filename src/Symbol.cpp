// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Ast.h"
#include "Exception.h"
#include "container/LinkedHashMap.hpp"
#include "fmt/format.h"
#include <cctype>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>

// Symbol {

Symbol::Symbol(const Name &name, const Symbol *owner)
    : name_(name), owner_(owner) {}

const Name &Symbol::name() const { return name_; }

const Symbol *Symbol::owner() const { return owner_; }

// Symbol }

// TypeSymbol {

TypeSymbol::TypeSymbol(const Name &name, const Symbol *owner)
    : name_(name), owner_(owner) {}

const Name &TypeSymbol::name() const { return name_; }

const Symbol *TypeSymbol::owner() const { return owner_; }

// TypeSymbol }

// Scope {

Scope::Scope(const Name &name, const Symbol *owner) : Symbol(name, owner) {}

void Scope::define(const Symbol *s, const TypeSymbol *ts) {
  EX_ASSERT(s, "symbol s must not null");
  EX_ASSERT(ts, "type symbol ts must not null");
  EX_ASSERT(!s_scope_.contains(s->name()),
            "symbol {} already exist in scope {}", s->name().raw(),
            (void *)&s_scope_);
  EX_ASSERT(!ts_scope_.contains(ts->name()),
            "type symbol {} already exist in scope {}", ts->name().raw(),
            (void *)&ts_scope_);
  s_scope_.insert(s->name(), SymbolData(s, ts));
  ts_scope_.insert(ts->name(), TypeSymbolData(ts));
}

const SymbolData &Scope::s_resolve(const Name &name) const {
  EX_ASSERT(name.raw().length() > 0, "name {} length {} > 0", name.raw(),
            name.raw().length());
  if (s_scope_.contains(name)) {
    return s_scope_[name];
  }
  return static_cast<const Scope *>(owner_)->s_resolve(name);
}

const TypeSymbolData &Scope::ts_resolve(const Name &name) const {
  EX_ASSERT(name.raw().length() > 0, "name {} length {} > 0", name.raw(),
            name.raw().length());
  if (ts_scope_.contains(name)) {
    return ts_scope_[name];
  }
  return static_cast<const Scope *>(owner_)->ts_resolve(name);
}

int Scope::resolveContains(const Name &name) const {
  EX_ASSERT(name.raw().length() > 0, "name {} length {} > 0", name.raw(),
            name.raw().length());
  return s_resolveContains(name) + ts_resolveContains(name);
}

int Scope::s_resolveContains(const Name &name) const {
  EX_ASSERT(name.raw().length() > 0, "name {} length {} > 0", name.raw(),
            name.raw().length());
  if (s_scope_.contains(name)) {
    return 1;
  }
  if (owner_) {
    return static_cast<const Scope *>(owner_)->s_resolveContains(name) + 1;
  }
  return 0;
}

int Scope::ts_resolveContains(const Name &name) const {
  EX_ASSERT(name.raw().length() > 0, "name {} length {} > 0", name.raw(),
            name.raw().length());
  if (ts_scope_.contains(name)) {
    return 1;
  }
  if (owner_) {
    return static_cast<const Scope *>(owner_)->ts_resolveContains(name) + 1;
  }
  return 0;
}

bool Scope::contains(const Name &name) const {
  EX_ASSERT(name.raw().length() > 0, "name {} length {} > 0", name.raw(),
            name.raw().length());
  return s_scope_.contains(name) || ts_scope_.contains(name);
}

bool Scope::empty() const { return s_scope_.empty() && ts_scope_.empty(); }

int Scope::size() const { return s_scope_.size() + ts_scope_.size(); }

Scope::s_iterator Scope::s_begin() { return s_scope_.begin(); }

Scope::s_const_iterator s_begin() const { return s_scope_.begin(); }

Scope::s_iterator s_end() { return s_scope_.end(); }

Scope::s_const_iterator s_end() const { return s_scope_.end(); }

Scope::ts_iterator ts_begin() { return ts_scope_.begin(); }

Scope::ts_const_iterator ts_begin() const { return ts_scope_.begin(); }

Scope::ts_iterator ts_end() { return ts_scope_.end(); }

Scope::ts_const_iterator ts_end() const { return ts_scope_.end(); }

// Scope }

// Ts_Builtin {

Ts_Builtin::Ts_Builtin(const Name &name, const Symbol *owner)
    : TypeSymbol(name, owner) {}

TypeSymbolCategory Ts_Builtin::category() const {
  return TypeSymbolCategory::Builtin;
}

std::string Ts_Builtin::toString() const {
  return fmt::format("[{} position:{}]", name().raw(), position.toString());
}

// Ts_Builtin }

// Ts_Class {

Ts_Class::Ts_Class(const Name &name, const Symbol *owner,
                   const std::vector<Symbol *> &fields,
                   const std::vector<Symbol *> &methods)
    : TypeSymbol(name, owner), fields_(fields), methods_(methods) {
  for (int i = 0; i < (int)fields_.size(); i++) {
    EX_ASSERT(fields_[i], "fields_[{}] must not null", i);
  }
  for (int i = 0; i < (int)methods_.size(); i++) {
    EX_ASSERT(methods_[i], "methods_[{}] must not null", i);
  }
}

TypeSymbolCategory Ts_Class::category() const {
  return TypeSymbolCategory::Class;
}

std::string Ts_Class::toString() const {
  std::stringstream ss;
  ss << fmt::format("[{} position:{}, fields:", name().raw(),
                    position.toString());
  if (fields_.size() == 0) {
    ss << "empty, ";
  } else {
    for (int i = 0; i < (int)fields_.size(); i++) {
      ss << fmt::format("{}, ", fields_[i]->name().raw());
    }
  }
  ss << "methods:";
  if (methods_.size() == 0) {
    ss << "empty";
  } else {
    for (int i = 0; i < (int)methods_.size(); i++) {
      ss << fmt::format("{}", methods_[i]->name().raw());
      if (i < (int)methods_.size() - 1) {
        ss << ", ";
      }
    }
  }
  ss << "]";
  return ss.str();
}

std::vector<Symbol *> &Ts_Class::fields() { return fields_; }

const std::vector<Symbol *> &Ts_Class::fields() const { return fields_; }

std::vector<Symbol *> &Ts_Class::methods() { return methods_; }

const std::vector<Symbol *> &Ts_Class::methods() const { return methods_; }

// Ts_Class }

// Ts_Func {

static std::string buildFunctionType(const std::vector<TypeSymbol *> &params,
                                     const TypeSymbol *result) {
  std::stringstream ss;
  ss << "(";
  for (int i = 0; i < (int)params_.size(); i++) {
    ss << params[i]->name().raw();
    if (i < (int)params_.size() - 1) {
      ss << ",";
    }
  }
  ss << fmt::format(")->{}", result_->name().raw());
  return ss.str();
}

Ts_Func::Ts_Func(const Name &name, const Symbol *owner,
                 const std::vector<TypeSymbol *> &params,
                 const TypeSymbol *result)
    : TypeSymbol(buildFunctionType(params, result), owner), params_(params),
      result_(result) {}

TypeSymbolCategory Ts_Func::category() const {
  return TypeSymbolCategory::Func;
}

std::string Ts_Func::toString() const {
  std::stringstream ss;
  ss << fmt::format("[{} position:{}, params:", name().raw(),
                    position.toString());
  if (params_.empty()) {
    ss << "empty, ";
  } else {
    for (int i = 0; i < params_.size(); i++) {
      ss << fmt::format("{}, ", params_[i]->toString());
    }
  }
  ss << fmt::format("result:{}]", result_->toString());
  return ss.str();
}

std::vector<TypeSymbol *> &Ts_Func::params() { return params_; }

const std::vector<TypeSymbol *> &Ts_Func::params() const { return params_; }

TypeSymbol *&Ts_Func::result() { return result_; }

const TypeSymbol *Ts_Func::result() const { return result_; }

// Ts_Func }

// S_Var {

// S_Var }
