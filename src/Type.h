// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "container/LinkedHashMap.hpp"
#include "interface/Namely.h"
#include "interface/Stringify.h"
#include <string>
#include <utility>
#include <vector>

/*================ type start from 3000 ================*/
BETTER_ENUM(TyType, int,
            // type
            Builtin = 3000, Class, Function,
            // type scope
            Global, Local)

class Tytab;

class Type : public Namely, private boost::noncopyable {
public:
  virtual ~Type() = default;
  virtual std::string name() const = 0;
  virtual TyType type() const = 0;

  static void push(Tytab *&global, Tytab *&current, Tytab *s);
  static void pop(Tytab *&global, Tytab *&current);
};

class Tytab : public Type, public Stringify {
public:
  using Iterator = LinkedHashMap<Symbol *, Type *>::Iterator;
  using CIterator = LinkedHashMap<Symbol *, Type *>::CIterator;

  Tytab(Tytab *enclosingScope);
  virtual ~Tytab() = default;
  virtual std::string name() const = 0;
  virtual TyType type() const = 0;
  virtual void define(Symbol *sym, Type *ty);
  virtual Type *resolve(Symbol *sym);
  virtual Tytab *enclosingScope();
  virtual std::string toString() const;
  virtual Iterator begin();
  virtual CIterator begin() const;
  virtual Iterator end();
  virtual CIterator end() const;
  virtual int size() const;
  virtual bool empty() const;

protected:
  virtual std::string stringify() const = 0;

  Tytab *enclosingScope_;
  LinkedHashMap<Symbol *, Type *> hashtab_;
};

class BuiltinType : public Type {
public:
  virtual ~BuiltinType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

  static BuiltinType *ty_i8();
  static BuiltinType *ty_u8();
  static BuiltinType *ty_i16();
  static BuiltinType *ty_u16();
  static BuiltinType *ty_i32();
  static BuiltinType *ty_u32();
  static BuiltinType *ty_i64();
  static BuiltinType *ty_u64();
  static BuiltinType *ty_f32();
  static BuiltinType *ty_f64();
  static BuiltinType *ty_boolean();
  static BuiltinType *ty_string();
  static BuiltinType *ty_nil();
  static BuiltinType *ty_void();

private:
  BuiltinType(const std::string &name);
  std::string builtinTypeName_;
};

class ClassType : public Tytab {
public:
  ClassType(const std::string &classType,
            const std::vector<std::pair<Symbol *, Type *>> &memberList,
            const std::vector<std::pair<Symbol *, Type *>> &methodList,
            Tytab *enclosingScope);
  virtual ~ClassType() = default;
  virtual std::string name() const;
  virtual TyType type() const;
  // virtual void define(Symbol *sym, Type *ty);
  // virtual Type *resolve(Symbol *sym);
  // virtual Tytab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
  std::string classType_;
};

class FunctionType : public Tytab {
public:
  FunctionType(const std::vector<std::pair<Symbol *, Type *>> &argumentList,
               Type *result, Tytab *enclosingScope);
  virtual ~FunctionType() = default;
  virtual std::string name() const;
  virtual TyType type() const;
  // virtual void define(Symbol *sym, Type *ty);
  // virtual Type *resolve(Symbol *sym);
  // virtual Tytab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
  std::string functionType_;
};

class GlobalTytab : public Tytab {
public:
  GlobalTytab();
  virtual ~GlobalTytab() = default;
  virtual std::string name() const;
  virtual TyType type() const;
  // virtual void define(Symbol *sym, Type *ty);
  // virtual Type *resolve(Symbol *sym);
  // virtual Tytab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
};

class LocalTytab : public Tytab {
public:
  LocalTytab(const std::string &localTytabName, Tytab *enclosingScope);
  virtual ~LocalTytab() = default;
  virtual std::string name() const;
  virtual TyType type() const;
  // virtual void define(Symbol *sym, Type *ty);
  // virtual Type *resolve(Symbol *sym);
  // virtual Tytab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
  std::string localTytabName_;
};
