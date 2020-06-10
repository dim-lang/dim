// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"
#include <cctype>
#include <numeric>
#include <sstream>
#include <utility>

Scope::Scope(Scope *enclosingScope) : enclosingScope_(enclosingScope) {}

void Scope::define(const Scope::SNode &snode) {
  const Symbol *s = Scope::sym(snode);
  const Type *t = Scope::ty(snode);
  const Ast *a = Scope::ast(snode);
  EX_ASSERT(s && t && a, "symbol {} or type {} or ast {} is null", (void *)s,
            (void *)t, (void *)a);
  EX_ASSERT(map_.find(Scope::sym(snode)->name()) == map_.end(),
            "symbol {} already exist", Scope::sym(snode)->name());
  map_.insert(std::make_pair(Scope::sym(snode)->name(), snode));
}

Scope::SNode Scope::resolve(const std::string &name) {
  EX_ASSERT(name.length() > 0, "name#length {} > 0", name.length());
  if (map_.find(name) != map_.end())
    return map_[name];
  if (enclosingScope_)
    return enclosingScope_->resolve(name);
  return std::make_tuple<Symbol *, Type *, Ast *>(nullptr, nullptr, nullptr);
}

Scope *Scope::enclosingScope() { return enclosingScope_; }

std::string Scope::toString() const {
  return fmt::format("[ @{} name_:{}, enclosingScope_:{}, map_#size:{} ]",
                     stringify(), name(), enclosingScope_->name(), map_.size());
}

Scope::Iterator Scope::begin() { return map_.begin(); }

Scope::CIterator Scope::begin() const { return map_.begin(); }

Scope::Iterator Scope::end() { return map_.end(); }

Scope::CIterator Scope::end() const { return map_.end(); }

int Scope::size() const { return (int)map_.size(); }

bool Scope::empty() const { return map_.empty(); }

const Symbol *Scope::sym(const SNode &snode) { return std::get<0>(snode); }

Symbol *&Scope::sym(SNode &snode) { return std::get<0>(snode); }

const Type *Scope::ty(const SNode &snode) { return std::get<1>(snode); }

Type *&Scope::ty(SNode &snode) { return std::get<1>(snode); }

const Ast *Scope::ast(const SNode &snode) { return std::get<2>(snode); }

Ast *&Scope::ast(SNode &snode) { return std::get<2>(snode); }

Scope::SNode Scope::make_snode(Symbol *s, Type *t, Ast *a) {
  EX_ASSERT(s && t && a, "s {} or t {} or a {} is null", (void *)s, (void *)t,
            (void *)a);
  return std::make_tuple(s, t, a);
}

const Scope::SNode &Scope::invalid_snode() {
  static Scope::SNode inv = std::make_tuple(nullptr, nullptr, nullptr);
  return inv;
}

// type start

BuiltinType::BuiltinType(const std::string &name) : builtinTypeName_(name) {}

std::string BuiltinType::name() const { return builtinTypeName_; }

TyType BuiltinType::type() const { return TyType::Builtin; }

BuiltinType *BuiltinType::ty_int8() {
  static BuiltinType *type_int8 = new BuiltinType("int8");
  return type_int8;
}

BuiltinType *BuiltinType::ty_uint8() {
  static BuiltinType *type_uint8 = new BuiltinType("uint8");
  return type_uint8;
}

BuiltinType *BuiltinType::ty_int16() {
  static BuiltinType *type_int16 = new BuiltinType("int16");
  return type_int16;
}

BuiltinType *BuiltinType::ty_uint16() {
  static BuiltinType *type_uint16 = new BuiltinType("uint16");
  return type_uint16;
}

BuiltinType *BuiltinType::ty_int32() {
  static BuiltinType *type_int32 = new BuiltinType("int32");
  return type_int32;
}

BuiltinType *BuiltinType::ty_uint32() {
  static BuiltinType *type_uint32 = new BuiltinType("uint32");
  return type_uint32;
}

BuiltinType *BuiltinType::ty_int64() {
  static BuiltinType *type_int64 = new BuiltinType("int64");
  return type_int64;
}

BuiltinType *BuiltinType::ty_uint64() {
  static BuiltinType *type_uint64 = new BuiltinType("uint64");
  return type_uint64;
}

BuiltinType *BuiltinType::ty_float32() {
  static BuiltinType *type_float32 = new BuiltinType("float32");
  return type_float32;
}

BuiltinType *BuiltinType::ty_float64() {
  static BuiltinType *type_float64 = new BuiltinType("float64");
  return type_float64;
}

BuiltinType *BuiltinType::ty_boolean() {
  static BuiltinType *type_boolean = new BuiltinType("boolean");
  return type_boolean;
}

BuiltinType *BuiltinType::ty_string() {
  static BuiltinType *type_string = new BuiltinType("string");
  return type_string;
}

BuiltinType *BuiltinType::ty_nil() {
  static BuiltinType *type_nil = new BuiltinType("nil");
  return type_nil;
}

BuiltinType *BuiltinType::ty_void() {
  static BuiltinType *type_void = new BuiltinType("void");
  return type_void;
}

ClassType::ClassType(const std::string &classType,
                     const std::vector<std::pair<Symbol *, Type *>> &memberList,
                     const std::vector<std::pair<Symbol *, Type *>> &methodList)
    : classType_(classType) {}

std::string ClassType::name() const { return classType_; }

TyType ClassType::type() const { return TyType::Class; }

FunctionType::FunctionType(const std::vector<Type *> &argTypeList,
                           Type *result) {
  EX_ASSERT(result, "result is null");
  std::stringstream ss;
  ss << "func(";
  for (int i = 0; i < (int)argTypeList.size(); i++) {
    ss << argTypeList[i]->name();
    if (i < (int)argTypeList.size() - 1) {
      ss << ",";
    }
  }
  ss << "):" << result->name();
  functionType_ = ss.str();
}

std::string FunctionType::name() const { return functionType_; }

TyType FunctionType::type() const { return TyType::Function; }

ScopeType::ScopeType(const std::string &scopeTypeName)
    : scopeTypeName_(scopeTypeName) {}

std::string ScopeType::name() const { return scopeTypeName_; }

TyType ScopeType::type() const { return TyType::Scope; }

ScopeType *ScopeType::ty_local() {
  static ScopeType *ty_local = new ScopeType("LocalScopeType");
  return ty_local;
}

ScopeType *ScopeType::ty_global() {
  static ScopeType *ty_global = new ScopeType("GlobalScopeType");
  return ty_global;
}

// type end

// symbol start

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
                               Scope *enclosingScope)
    : Scope(enclosingScope), functionName_(functionName) {}

std::string FunctionSymbol::name() const { return functionName_; }

SymType FunctionSymbol::type() const { return SymType::Function; }

std::string FunctionSymbol::stringify() const { return "FunctionSymbol"; }

ClassSymbol::ClassSymbol(const std::string &className, Scope *enclosingScope)
    : Scope(enclosingScope), className_(className) {}

std::string ClassSymbol::name() const { return className_; }

SymType ClassSymbol::type() const { return SymType::Class; }

std::string ClassSymbol::stringify() const { return "ClassSymbol"; }

// symbol end

GlobalScope::GlobalScope() : Scope(nullptr) {}

std::string GlobalScope::name() const {
  static std::string globalScopeName = "GlobalScope";
  return globalScopeName;
}

SymType GlobalScope::type() const { return SymType::Global; }

std::string GlobalScope::stringify() const { return "GlobalScope"; }

LocalScope::LocalScope(const std::string &localScopeName, Scope *enclosingScope)
    : Scope(enclosingScope), localScopeName_(localScopeName) {}

std::string LocalScope::name() const { return localScopeName_; }

SymType LocalScope::type() const { return SymType::Local; }

std::string LocalScope::stringify() const { return "LocalScope"; }
