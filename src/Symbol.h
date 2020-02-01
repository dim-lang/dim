// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Scope.h"
#include "Stringify.h"
#include "Type.h"
#include <string>

class Symbol : public Stringify {
public:
  virtual ~Symbol() = default;
  virtual const std::string &name() const = 0;
  virtual Type *type() const = 0;
  virtual std::string toString() const = 0;
};

class BuiltinTypeSymbol : public Symbol, public Type {
public:
  virtual ~BuiltinTypeSymbol() = default;
  virtual const std::string &name() const;
  virtual Type *type() const;
  virtual std::string toString() const;
  /**
   * instance:
   * <i64> <f64> <string> <boolean> <class> <func>
   */
  static BuiltinTypeSymbol *i64Instance();
  static BuiltinTypeSymbol *f64Instance();
  static BuiltinTypeSymbol *stringInstance();
  static BuiltinTypeSymbol *booleanInstance();
  static BuiltinTypeSymbol *classInstance();
  static BuiltinTypeSymbol *funcInstance();

private:
  BuiltinTypeSymbol(const std::string &name);

  std::string builtinTypeName_;
};

class VariableSymbol : public Symbol {
public:
  /**
   * <x:i64>
   * <y:boolean>
   */
  VariableSymbol(const std::string &name, Type *type);
  virtual ~VariableSymbol() = default;
  virtual const std::string &name() const;
  virtual Type *type() const;
  virtual std::string toString() const;

private:
  std::string variableName_;
  Type *variableType_;
};

class FunctionSymbol : public Symbol, public Scope {
public:
  FunctionSymbol(const std::string &name, Type *type,
                 Scope *enclosingScope = Scope::currentScope());
  virtual ~FunctionSymbol() = default;
  /**
   * <func():void>
   * <func(i64, boolean):void>
   * <func(string):i64>
   */
  virtual const std::string &name() const;
  virtual Type *type() const;
  virtual std::string toString() const;

private:
  std::string functionName_;
  Type *functionType_;
};

class ClassSymbol : public Symbol, public Scope, public Type {
public:
  /**
   * <System>
   */
  ClassSymbol(const std::string &name, Type *type);
  virtual ~ClassSymbol() = default;
  virtual const std::string &name() const;
  virtual Type *type() const;
  virtual std::string toString() const;

private:
  std::string className_;
  Type *classType_;
};
