// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include "interface/Namely.h"
#include "interface/Stringify.h"
#include "llvm/IR/Value.h"
#include <string>
#include <tuple>
#include <vector>

/*================ type start from 2000 ================*/
BETTER_ENUM(SymType, int,
            // symbol
            Variable = 2000, FunctionArgument, Function, Class,
            // symbol scope
            Global, Local)

/*================ type start from 3000 ================*/
BETTER_ENUM(TyType, int,
            // type
            Builtin = 3000, Class, Function,
            // scope type
            Scope)

class Ast;
class Scope;

// symbol
class VariableSymbol;
class FunctionArgumentSymbol;
class FunctionSymbol;
class ClassSymbol;
// scope symbol
class GlobalScope;
class LocalScope;

// type
class BuiltinType;
class ClassType;
class FunctionType;
class ScopeType;

class Scope;

class Symbol : public Namely, private boost::noncopyable {
public:
  Symbol(Scope *enclosingScope);
  virtual ~Symbol() = default;
  virtual std::string name() const = 0;
  virtual SymType type() const = 0;
  virtual Scope *enclosingScope() const;

protected:
  Scope *enclosingScope_;
};

class Type : public Namely, private boost::noncopyable {
public:
  virtual ~Type() = default;
  virtual std::string name() const = 0;
  virtual TyType type() const = 0;
};

class ScopeNode {
public:
  ScopeNode(Symbol *s, Type *t, Ast *a, llvm::Value *v = nullptr);
  virtual ~ScopeNode() = default;
  virtual std::string toString() const;
  static const ScopeNode &invalid();
  bool operator==(const ScopeNode &other) const;
  bool operator!=(const ScopeNode &other) const;

  Symbol *symbol;
  Type *type;
  Ast *ast;
  llvm::Value *value;

private:
  ScopeNode();
};

class Scope : public Symbol, public Stringify {
public:
  using SMap = LinkedHashMap<std::string, ScopeNode *>;
  using Iterator = SMap::Iterator;
  using CIterator = SMap::CIterator;

  Scope(Scope *enclosingScope);
  virtual ~Scope();
  virtual std::string name() const = 0;
  virtual SymType type() const = 0;
  virtual void define(ScopeNode *snode);
  virtual ScopeNode *resolve(const std::string &name) const;
  virtual std::string toString() const;
  virtual Iterator begin();
  virtual CIterator begin() const;
  virtual Iterator end();
  virtual CIterator end() const;
  virtual int size() const;
  virtual bool empty() const;

protected:
  virtual std::string stringify() const = 0;
  SMap map_;
};

class BuiltinType : public Type {
public:
  virtual ~BuiltinType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

  static BuiltinType *ty_int8();
  static BuiltinType *ty_uint8();
  static BuiltinType *ty_int16();
  static BuiltinType *ty_uint16();
  static BuiltinType *ty_int32();
  static BuiltinType *ty_uint32();
  static BuiltinType *ty_int64();
  static BuiltinType *ty_uint64();
  static BuiltinType *ty_float32();
  static BuiltinType *ty_float64();
  static BuiltinType *ty_boolean();
  static BuiltinType *ty_string();
  static BuiltinType *ty_nil();
  static BuiltinType *ty_void();

private:
  BuiltinType(const std::string &name);
  std::string builtinTypeName_;
};

class ClassType : public Type {
public:
  ClassType(const std::string &classType,
            const std::vector<std::pair<Symbol *, Type *>> &memberList,
            const std::vector<std::pair<Symbol *, Type *>> &methodList);
  virtual ~ClassType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

protected:
  std::string classType_;
};

class FunctionType : public Type {
public:
  FunctionType(const std::vector<Type *> &argTypeList, Type *result);
  virtual ~FunctionType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

protected:
  std::string functionType_;
};

class ScopeType : public Type {
public:
  virtual ~ScopeType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

  static ScopeType *ty_local();
  static ScopeType *ty_global();

protected:
  ScopeType(const std::string &scopeTypeName);
  std::string scopeTypeName_;
};

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const std::string &variableName, Scope *enclosingScope);
  virtual ~VariableSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

private:
  std::string variableName_;
};

class FunctionArgumentSymbol : public Symbol {
public:
  FunctionArgumentSymbol(const std::string &functionArgumentName,
                         Scope *enclosingScope);
  virtual ~FunctionArgumentSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

private:
  std::string functionArgumentName_;
};

class FunctionSymbol : public Scope {
public:
  FunctionSymbol(const std::string &functionName, Scope *enclosingScope);
  virtual ~FunctionSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
  std::string functionName_;
};

class ClassSymbol : public Scope {
public:
  ClassSymbol(const std::string &className, Scope *enclosingScope);
  virtual ~ClassSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
  std::string className_;
};

class GlobalScope : public Scope {
public:
  GlobalScope();
  virtual ~GlobalScope() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
};

class LocalScope : public Scope {
public:
  LocalScope(const std::string &localScopeName, Scope *enclosingScope);
  virtual ~LocalScope() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
  std::string localScopeName_;
};
