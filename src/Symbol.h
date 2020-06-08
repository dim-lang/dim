// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include "interface/Namely.h"
#include "interface/Stringify.h"
#include <string>

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
            // type scope
            Global, Local)

class Ast;
class Symtab;

class Symbol : public Namely, private boost::noncopyable {
public:
  virtual ~Symbol() = default;
  virtual std::string name() const = 0;
  virtual SymType type() const = 0;
};

class Type : public Namely, private boost::noncopyable {
public:
  virtual ~Type() = default;
  virtual std::string name() const = 0;
  virtual TyType type() const = 0;
};

class SymNode {
public:
  SymNode() : symbol(nullptr), type(nullptr), ast(nullptr) {}
  SymNode(Symbol *a_symbol, Type *a_type, Ast *a_ast)
      : symbol(a_symbol), type(a_type), ast(a_ast) {}
  virtual ~SymNode() = default;
  SymNode(const SymNode &) = default;
  SymNode &operator=(const SymNode &) = default;

  bool operator==(const SymNode &other) {
    if (this == &other)
      return true;
    return symbol == other.symbol && type == other.type && ast == other.ast;
  }
  bool operator!=(const SymNode &other) {
    if (this == &other)
      return false;
    return symbol != other.symbol || type != other.type || ast != other.ast;
  }

  Symbol *symbol;
  Type *type;
  Ast *ast;
};

class Symtab : public Symbol, public Stringify {
public:
  using LHM = LinkedHashMap<std::string, SymNode>;
  using Iterator = LHM::Iterator;
  using CIterator = LHM::CIterator;

  Symtab(Symtab *enclosingScope);
  virtual ~Symtab() = default;
  virtual std::string name() const = 0;
  virtual SymType type() const = 0;
  virtual void define(const SymNode &snode);
  virtual SymNode resolve(const std::string &name);
  virtual Symtab *enclosingScope();
  virtual std::string toString() const;
  virtual Iterator begin();
  virtual CIterator begin() const;
  virtual Iterator end();
  virtual CIterator end() const;
  virtual int size() const;
  virtual bool empty() const;

protected:
  virtual std::string stringify() const = 0;

  Symtab *enclosingScope_;
  LHM hashtab_;
};

// type start

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

class ClassType {
public:
  ClassType(const std::string &classType,
            const std::vector<std::pair<Symbol *, Type *>> &memberList,
            const std::vector<std::pair<Symbol *, Type *>> &methodList);
  virtual ~ClassType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

protected:
  virtual std::string stringify() const;
  std::string classType_;
};

class FunctionType {
public:
  FunctionType(const std::vector<std::pair<Symbol *, Type *>> &argumentList,
               Type *result);
  virtual ~FunctionType() = default;
  virtual std::string name() const;
  virtual TyType type() const;

protected:
  virtual std::string stringify() const;
  std::string functionType_;
};

// type end

// symbol start

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const std::string &variableName);
  virtual ~VariableSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

private:
  std::string variableName_;
};

class FunctionArgumentSymbol : public Symbol {
public:
  FunctionArgumentSymbol(const std::string &functionArgumentName);
  virtual ~FunctionArgumentSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

private:
  std::string functionArgumentName_;
};

class FunctionSymbol : public Symtab {
public:
  FunctionSymbol(const std::string &functionName, Symtab *enclosingScope);
  virtual ~FunctionSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
  std::string functionName_;
};

class ClassSymbol : public Symtab {
public:
  ClassSymbol(const std::string &className, Symtab *enclosingScope);
  virtual ~ClassSymbol() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
  std::string className_;
};

// symbol end

class GlobalScope : public Symtab {
public:
  GlobalScope();
  virtual ~GlobalScope() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
};

class LocalScope : public Symtab {
public:
  LocalScope(const std::string &localScopeName, Symtab *enclosingScope);
  virtual ~LocalScope() = default;
  virtual std::string name() const;
  virtual SymType type() const;

protected:
  virtual std::string stringify() const;
  std::string localScopeName_;
};
