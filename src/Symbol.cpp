// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"
#include <cctype>
#include <numeric>
#include <sstream>
#include <utility>

Symbol::Symbol(Scope *enclosingScope) : enclosingScope_(enclosingScope) {}

Scope *Symbol::enclosingScope() const { return enclosingScope_; }

Scope::Scope(Scope *enclosingScope) : Symbol(enclosingScope) {}

void Scope::define(const Scope::SNode &snode) {
  const Symbol *s = Scope::s(snode);
  const Type *t = Scope::t(snode);
  const Ast *a = Scope::a(snode);
  const llvm::Value *v = Scope::v(snode);
  EX_ASSERT(snode != invalid_snode(), "snode s:{} t:{} a:{} v:{} is null",
            (void *)s, (void *)t, (void *)a, (void *)v);
  EX_ASSERT(map_.find(Scope::s(snode)->name()) == map_.end(),
            "symbol {} already exist", Scope::s(snode)->name());
  map_.insert(std::make_pair(Scope::s(snode)->name(), snode));
}

Scope::SNode Scope::resolve(const std::string &name) const {
  EX_ASSERT(name.length() > 0, "name#length {} > 0", name.length());
  if (map_.find(name) != map_.end())
    return map_[name];
  if (enclosingScope_)
    return enclosingScope_->resolve(name);
  return invalid_snode();
}

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

const Symbol *Scope::s(const SNode &snode) { return std::get<0>(snode); }

Symbol *&Scope::s(SNode &snode) { return std::get<0>(snode); }

const Type *Scope::t(const SNode &snode) { return std::get<1>(snode); }

Type *&Scope::t(SNode &snode) { return std::get<1>(snode); }

const Ast *Scope::a(const SNode &snode) { return std::get<2>(snode); }

Ast *&Scope::a(SNode &snode) { return std::get<2>(snode); }

const llvm::Value *Scope::v(const SNode &snode) { return std::get<3>(snode); }

llvm::Value *&Scope::v(SNode &snode) { return std::get<3>(snode); }

Scope::SNode Scope::make_snode(Symbol *s, Type *t, Ast *a, llvm::Value *v) {
  EX_ASSERT(s && t && a, "s {} or t {} or a {} or v {} is null", (void *)s,
            (void *)t, (void *)a);
  return std::make_tuple(s, t, a, v);
}

const Scope::SNode &Scope::invalid_snode() {
  static Scope::SNode invalid =
      std::make_tuple(nullptr, nullptr, nullptr, nullptr);
  return invalid;
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

VariableSymbol::VariableSymbol(const std::string &variableName,
                               Scope *enclosingScope)
    : Symbol(enclosingScope), variableName_(variableName) {}

std::string VariableSymbol::name() const { return variableName_; }

SymType VariableSymbol::type() const { return SymType::Variable; }

FunctionArgumentSymbol::FunctionArgumentSymbol(
    const std::string &functionArgumentName, Scope *enclosingScope)
    : Symbol(enclosingScope), functionArgumentName_(functionArgumentName) {}

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
