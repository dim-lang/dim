// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>
#include <unordered_map>

class Symtab;

class Symbol {
public:
  virtual ~Symbol() = default;
  virtual const std::string &name() const = 0;

  static void push(Symtab *&global, Symtab *&current, Symtab *scope);
  static void pop(Symtab *&global, Symtab *&current);
};

class Symtab : public Stringify {
public:
  Symtab(Symtab *enclosingScope);
  virtual ~Symtab() = default;
  virtual const std::string &name() const = 0;
  virtual void define(Symbol *sym);
  virtual Symbol *resolve(const std::string &name);
  virtual Symtab *enclosingScope();
  virtual std::string toString() const;

protected:
  virtual std::string stringify() const = 0;

  Symtab *enclosingScope_;
  std::unordered_map<std::string, Symbol *> hashtab_;
};

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const std::string &variableName);
  virtual ~VariableSymbol() = default;
  virtual const std::string &name() const;

private:
  std::string variableName_;
};

class FunctionSymbol : public Symbol, public Symtab {
public:
  FunctionSymbol(const std::string &functionName, Symtab *enclosingScope);
  virtual ~FunctionSymbol() = default;
  virtual const std::string &name() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

private:
  virtual std::string stringify() const;
  std::string functionName_;
};

class ClassSymbol : public Symbol, public Symtab {
public:
  ClassSymbol(const std::string &className, Symtab *enclosingScope);
  virtual ~ClassSymbol() = default;
  virtual const std::string &name() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

private:
  virtual std::string stringify() const;
  std::string className_;
};

class GlobalSymtab : public Symtab {
public:
  GlobalSymtab();
  virtual ~GlobalSymtab() = default;
  virtual const std::string &name() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

private:
  virtual std::string stringify() const;
};

class LocalSymtab : public Symtab {
public:
  LocalSymtab(const std::string &localSymtabName, Symtab *enclosingScope);
  virtual ~LocalSymtab() = default;
  virtual const std::string &name() const;
  // virtual void define(Symbol *sym);
  // virtual Symbol *resolve(const std::string &name);
  // virtual Symtab *enclosingScope();
  // virtual std::string toString() const;

private:
  virtual std::string stringify() const;
  std::string localSymtabName_;
};
