// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Type.h"
#include "Log.h"
#include <sstream>

Tytab::Tytab(Tytab *enclosingScope) : enclosingScope_(enclosingScope) {}

void Type::push(Tytab *&global, Tytab *&current, Tytab *s) {
  current = s;
  if (!global) {
    global = s;
  }
}

void Type::pop(Tytab *&global, Tytab *&current) {
  current = current->enclosingScope();
}

void Tytab::define(Symbol *sym, Type *ty) {
  CASSERT(sym, "sym is null");
  CASSERT(ty, "ty is null");
  CASSERT(hashtab_.find(sym) == hashtab_.end(), "sym {}:{} already exist",
          (void *)sym, sym->name());
  hashtab_.insert(std::make_pair(sym, ty));
}

Type *Tytab::resolve(Symbol *sym) {
  CASSERT(sym, "sym is null");
  CASSERT(hashtab_.find(sym) != hashtab_.end(), "sym {}:{} not exist",
          (void *)sym, sym->name());
  return hashtab_[sym];
}

Tytab *Tytab::enclosingScope() { return enclosingScope_; }

std::string Tytab::toString() const {
  return fmt::format("[ @{} name_:{}, enclosingScope_:{}, hashtab_#size:{} ]",
                     stringify(), name(), enclosingScope_->name(),
                     hashtab_.size());
}

BuiltinType::BuiltinType(const std::string &name) : builtinTypeName_(name) {}

const std::string &BuiltinType::name() const { return builtinTypeName_; }

const BuiltinType *BuiltinType::ty_i8() {
  static BuiltinType *type_i8 = new BuiltinType("i8");
  return type_i8;
}

const BuiltinType *BuiltinType::ty_u8() {
  static BuiltinType *type_u8 = new BuiltinType("u8");
  return type_u8;
}

const BuiltinType *BuiltinType::ty_i16() {
  static BuiltinType *type_i16 = new BuiltinType("i16");
  return type_i16;
}

const BuiltinType *BuiltinType::ty_u16() {
  static BuiltinType *type_u16 = new BuiltinType("u16");
  return type_u16;
}

const BuiltinType *BuiltinType::ty_i32() {
  static BuiltinType *type_i32 = new BuiltinType("i32");
  return type_i32;
}

const BuiltinType *BuiltinType::ty_u32() {
  static BuiltinType *type_u32 = new BuiltinType("u32");
  return type_u32;
}

const BuiltinType *BuiltinType::ty_i64() {
  static BuiltinType *type_i64 = new BuiltinType("i64");
  return type_i64;
}

const BuiltinType *BuiltinType::ty_u64() {
  static BuiltinType *type_u64 = new BuiltinType("u64");
  return type_u64;
}

const BuiltinType *BuiltinType::ty_f32() {
  static BuiltinType *type_f32 = new BuiltinType("f32");
  return type_f32;
}

const BuiltinType *BuiltinType::ty_f64() {
  static BuiltinType *type_f64 = new BuiltinType("f64");
  return type_f64;
}

const BuiltinType *BuiltinType::ty_boolean() {
  static BuiltinType *type_boolean = new BuiltinType("boolean");
  return type_boolean;
}

const BuiltinType *BuiltinType::ty_string() {
  static BuiltinType *type_string = new BuiltinType("string");
  return type_string;
}

const BuiltinType *BuiltinType::ty_nil() {
  static BuiltinType *type_nil = new BuiltinType("nil");
  return type_nil;
}

const BuiltinType *BuiltinType::ty_void() {
  static BuiltinType *type_void = new BuiltinType("void");
  return type_void;
}

ClassType::ClassType(const std::string &classType,
                     const std::vector<std::pair<Symbol *, Type *>> &memberList,
                     const std::vector<std::pair<Symbol *, Type *>> &methodList,
                     Tytab *enclosingScope)
    : Tytab(enclosingScope), classType_(classType) {
  for (int i = 0; i < memberList.size(); i++) {
    define(memberList[i].first, memberList[i].second);
  }
  for (int i = 0; i < methodList.size(); i++) {
    define(methodList[i].first, methodList[i].second);
  }
}

const std::string &ClassType::name() const { return classType_; }

std::string ClassType::stringify() const { return "ClassType"; }

FunctionType::FunctionType(
    const std::vector<std::pair<Symbol *, Type *>> &argumentList, Type *result,
    Tytab *enclosingScope)
    : Tytab(enclosingScope) {
  CASSERT(result, "result is null");
  CASSERT(enclosingScope, "enclosingScope is null");
  std::stringstream ss;
  ss << "func(";
  for (int i = 0; i < argumentList.size(); i++) {
    ss << argumentList[i].second->name();
    if (i < (int)argumentList.size() - 1) {
      ss << ",";
    }
  }
  ss << "):" << result->name();
  functionType_ = ss.str();
  for (int i = 0; i < (int)argumentList.size(); i++) {
    define(argumentList[i].first, argumentList[i].second);
  }
}

const std::string &FunctionType::name() const { return functionType_; }

std::string FunctionType::stringify() const { return "FunctionType"; }

GlobalTytab::GlobalTytab() : Tytab(nullptr) {}

const std::string &GlobalTytab::name() const {
  static std::string globalTytabName = "GlobalTytab";
  return globalTytabName;
}

std::string GlobalTytab::stringify() const { return "GlobalTytab"; }

LocalTytab::LocalTytab(const std::string &localTytabName, Tytab *enclosingScope)
    : Tytab(enclosingScope), localTytabName_(localTytabName) {}

const std::string &LocalTytab::name() const { return localTytabName_; }

std::string LocalTytab::stringify() const { return "LocalTytab"; }
