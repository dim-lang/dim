// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Log.h"
#include "container/LinkedHashMap.hpp"
#include <cctype>
#include <numeric>
#include <sstream>
#include <utility>

Symtab::Symtab(Symtab *enclosingScope) : enclosingScope_(enclosingScope) {}

void Symtab::define(const SymNode &snode) {
  LOG_ASSERT(snode.symbol && snode.type && snode.ast,
             "symbol or type or ast is null");
  LOG_ASSERT(hashtab_.find(snode.symbol->name()) == hashtab_.end(),
             "symbol {} already exist", snode.symbol->name());
  hashtab_.insert(std::make_pair(snode.symbol->name(), snode));
}

SymNode Symtab::resolve(const std::string &name) {
  LOG_ASSERT(name.length() > 0, "name#length {} > 0", name.length());
  if (hashtab_.find(name) != hashtab_.end())
    return hashtab_[name];
  if (enclosingScope_)
    return enclosingScope_->resolve(name);
  return SymNode();
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

std::string ClassType::stringify() const { return "ClassType"; }

FunctionType::FunctionType(const std::vector<Type *> &argTypeList,
                           Type *result) {
  LOG_ASSERT(result, "result is null");
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

std::string FunctionType::stringify() const { return "FunctionType"; }

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

// symbol end

GlobalScope::GlobalScope() : Symtab(nullptr) {}

std::string GlobalScope::name() const {
  static std::string globalSymtabName = "GlobalScope";
  return globalSymtabName;
}

SymType GlobalScope::type() const { return SymType::Global; }

std::string GlobalScope::stringify() const { return "GlobalScope"; }

LocalScope::LocalScope(const std::string &localScopeName,
                       Symtab *enclosingScope)
    : Symtab(enclosingScope), localScopeName_(localScopeName) {}

std::string LocalScope::name() const { return localScopeName_; }

SymType LocalScope::type() const { return SymType::Local; }

std::string LocalScope::stringify() const { return "LocalScope"; }
