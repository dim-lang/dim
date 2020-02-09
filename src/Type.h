// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "Symbol.h"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Symbol;
class Tytab;

class Type {
public:
  virtual ~Type() = default;
  virtual const std::string &name() const = 0;

  static void push(Tytab *&global, Tytab *&current, Tytab *scope);
  static void pop(Tytab *&global, Tytab *&current);
};

class Tytab : public Stringify {
public:
  virtual ~Tytab() = default;
  virtual const std::string &name() const = 0;
  virtual void define(Symbol *sym, Type *ty) = 0;
  virtual Type *resolve(Symbol *sym) = 0;
  virtual Tytab *enclosingScope() = 0;
  virtual std::string toString() const = 0;

protected:
};

class BuiltinType : public Type {
public:
  virtual ~BuiltinType() = default;
  virtual const std::string &name() const;

  static const BuiltinType *ty_i8();
  static const BuiltinType *ty_ui8();
  static const BuiltinType *ty_i16();
  static const BuiltinType *ty_ui16();
  static const BuiltinType *ty_i32();
  static const BuiltinType *ty_ui32();
  static const BuiltinType *ty_i64();
  static const BuiltinType *ty_ui64();
  static const BuiltinType *ty_f32();
  static const BuiltinType *ty_f64();
  static const BuiltinType *ty_boolean();
  static const BuiltinType *ty_string();
  static const BuiltinType *ty_nil();
  static const BuiltinType *ty_void();

private:
  BuiltinType(const std::string &name);
  std::string builtinTypeName_;
};

class ClassType : public Type, public Tytab {
public:
  ClassType(const std::vector<std::pair<Symbol *, Type *>> &memberList,
            const std::vector<std::pair<Symbol *, Type *>> &meothodList,
            Tytab *enclosingScope);
  virtual ~ClassType() = default;
  virtual const std::string &name() const;
  virtual void define(Type *ty);
  virtual Type *resolve(const std::string &name);
  virtual Tytab *enclosingScope();
  virtual std::string toString() const;

private:
  std::vector<std::pair<Symbol *, Type *>> memberList_;
  std::vector<std::pair<Symbol *, Type *>> methodList_;
  Tytab *enclosingScope_;
  std::unordered_map<std::string, Type *> hashtab_;
};

class FunctionType : public Type, public Tytab {
public:
  FunctionType(const std::vector<std::pair<Symbol *, Type *>> &argumentList,
               const std::pair<Symbol *, Type *> &result,
               Tytab *enclosingScope);
  virtual ~FunctionType() = default;
  virtual const std::string &name() const;
  virtual void define(Type *ty);
  virtual Type *resolve(const std::string &name);
  virtual Tytab *enclosingScope();
  virtual std::string toString() const;

private:
  std::vector<std::pair<Symbol *, Type *>> argumentList_;
  std::pair<Symbol *, Type *> result_;
  Tytab *enclosingScope_;
  std::unordered_map<std::string, Type *> hashtab_;
};

class GlobalTytab : public Tytab {
public:
  GlobalTytab();
  virtual ~GlobalTytab() = default;
  virtual const std::string &name() const;
  virtual void define(Type *ty);
  virtual Type *resolve(const std::string &name);
  virtual Tytab *enclosingScope();
  virtual std::string toString() const;
};

class LocalTytab : public Tytab {
public:
  LocalTytab(const std::string &name, Tytab *enclosingScope);
  virtual ~LocalTytab() = default;
  virtual const std::string &name() const;
  virtual void define(Type *ty);
  virtual Type *resolve(const std::string &name);
  virtual Tytab *enclosingScope();
  virtual std::string toString() const;

private:
  std::string localName_;
  Tytab *enclosingScope_;
};

class TypeManager {
public:
  static Tytab *globalScope();
  static Tytab *currentScope();
  static void pushScope(Tytab *scope);
  static void popScope();

private:
  static Tytab *globalScope_;
  static Tytab *currentScope_;
};
