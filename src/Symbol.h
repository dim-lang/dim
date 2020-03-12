// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "container/LinkedHashMap.hpp"
#include "interface/Namely.h"
#include "interface/Stringify.h"
#include "interface/Typely.h"
#include <string>

#define SYM_VAR 101
#define SYM_FUNC_ARG 102
#define SYM_FUNC 103
#define SYM_CLASS 104

#define SYM_GLOBAL 201
#define SYM_LOCAL 202

class Symtab;

class Symbol : public Namely, public Typely, private boost::noncopyable {
public:
  virtual ~Symbol() = default;
  virtual std::string name() const = 0;
  virtual int type() const = 0;

  static void push(Symtab *&global, Symtab *&current, Symtab *scope);
  static void pop(Symtab *&global, Symtab *&current);
};

class Symtab : public Symbol, public Stringify {
public:
  using Iterator = LinkedHashMap<std::string, Symbol *>::Iterator;
  using CIterator = LinkedHashMap<std::string, Symbol *>::CIterator;

  Symtab(Symtab *enclosingScope);
  virtual ~Symtab() = default;
  virtual std::string name() const = 0;
  virtual int type() const = 0;
  virtual void define(Symbol *sym);
  virtual Symbol *resolve(const std::string &name);
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
  LinkedHashMap<std::string, Symbol *> hashtab_;
};

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const std::string &variableName);
  virtual ~VariableSymbol() = default;
  virtual std::string name() const;
  virtual int type() const;

private:
  std::string variableName_;
};

class FunctionArgumentSymbol : public Symbol {
public:
  FunctionArgumentSymbol(const std::string &functionArgumentName);
  virtual ~FunctionArgumentSymbol() = default;
  virtual std::string name() const;
  virtual int type() const;

private:
  std::string functionArgumentName_;
};

class FunctionSymbol : public Symtab {
public:
  FunctionSymbol(const std::string &functionName, Symtab *enclosingScope);
  virtual ~FunctionSymbol() = default;
  virtual std::string name() const;
  virtual int type() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
  std::string functionName_;
};

class ClassSymbol : public Symtab {
public:
  ClassSymbol(const std::string &className, Symtab *enclosingScope);
  virtual ~ClassSymbol() = default;
  virtual std::string name() const;
  virtual int type() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
  std::string className_;
};

class GlobalSymtab : public Symtab {
public:
  static GlobalSymtab *instance();
  virtual ~GlobalSymtab() = default;
  virtual std::string name() const;
  virtual int type() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

protected:
  GlobalSymtab();
  virtual std::string stringify() const;
};

class LocalSymtab : public Symtab {
public:
  LocalSymtab(const std::string &localSymtabName, Symtab *enclosingScope);
  virtual ~LocalSymtab() = default;
  virtual std::string name() const;
  virtual int type() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

protected:
  virtual std::string stringify() const;
  std::string localSymtabName_;
};
