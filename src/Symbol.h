// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Location.h"
#include "Name.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include <memory>

/*================ symbol kind enum start from 2000 ================*/
BETTER_ENUM(SymbolKind, int,
            // symbol
            // variable
            Var = 2000,
            // function
            Func, Param,
            // class
            Field, Method,
            // scope
            Local, Global,
            // type symbol
            PlainType, FuncType, ClassType)

/*================ type symbol kind enum start from 3000 ================*/
BETTER_ENUM(TypeSymbolKind, int,
            // plain type
            Plain = 3000,
            // function type
            Func,
            // class type
            Class)

class Ast;
class Ownable;
class Symbol;
class TypeSymbol;
class Scope;

/**
 * relationship between Symbol, TypeSymbol and Scope:
 *
 * Symbol = Var, Param
 * Symbol+Scope = Func, Method, Local, Global
 * TypeSymbol =
 *    Plain: int, long, boolean, void, etc
 *    Func: (int, int)=>int, etc
 * TypeSymbol+Scope = Class
 */

class Ownable {
public:
  virtual ~Ownable() = default;
  virtual std::shared_ptr<Scope> owner() const = 0;
};

class OwnableImpl : public Ownable {
public:
  Ownable(std::shared_ptr<Scope> owner);
  virtual ~Ownable() = default;
  virtual std::shared_ptr<Scope> owner() const;

protected:
  std::shared_ptr<Scope> owner_;
};

class Typeable {
public:
  virtual ~Typeable() = default;
  virtual std::shared_ptr<TypeSymbol> type() const = 0;
};

class TypeableImpl : public Typeable {
public:
  TypeableImpl(std::shared_ptr<TypeSymbol> a_typeableImpl);
  virtual ~TypeableImpl() = default;
  virtual std::shared_ptr<TypeSymbol> type() const;

protected:
  std::shared_ptr<TypeSymbol> typeableImpl_;
};

#define SYMBOL_INTERFACE                                                       \
  virtual std::shared_ptr<Scope> owner() const = 0;                            \
  virtual Cowstr &name() = 0;                                                  \
  virtual const Cowstr &name() const = 0;                                      \
  virtual Location &location() = 0;                                            \
  virtual const Location &location() const = 0;                                \
  virtual SymbolKind kind() const = 0;                                         \
  virtual std::shared_ptr<TypeSymbol> type() const = 0;

class Symbol : public Nameable,
               public Locationable,
               public Ownable,
               public Typeable,
               private boost::noncopyable {
public:
  virtual ~Symbol() = default;
  SYMBOL_INTERFACE

  static bool isSymbol(std::shared_ptr<Symbol> symbol);
  static bool isTypeSymbol(std::shared_ptr<Symbol> symbol);
};

class TypeSymbol : public Symbol {
public:
  virtual ~TypeSymbol() = default;
  SYMBOL_INTERFACE
};

struct SymbolData {
  std::shared_ptr<Symbol> symbol;

  SymbolData(std::shared_ptr<Symbol> a_symbol) : symbol(a_symbol) {}
  virtual ~SymbolData() = default;
};

struct TypeSymbolData {
  std::shared_ptr<TypeSymbol> typeSymbol;

  TypeSymbolData(std::shared_ptr<TypeSymbol> a_typeSymbol)
      : typeSymbol(a_typeSymbol) {}
  virtual ~TypeSymbolData() = default;
};

class Scope : public Symbol {
public:
  using scope_map = LinkedHashMap<Cowstr, std::shared_ptr<Symbol>>;
  using scope_iterator = scope_map::iterator;
  using scope_const_iterator = scope_map::const_iterator;

  virtual ~Scope() = default;

  // static std::shared_ptr<Scope> from(Ast *compileUnit);
  // static void check(Ast *compileUnit, std::shared_ptr<Symbol> symbol);

  SYMBOL_INTERFACE

  // scope api
  virtual void define(std::shared_ptr<Symbol> symbol);
  virtual std::shared_ptr<Symbol> resolve(const Cowstr &name) const;

  // work only in current map, not recursively
  virtual bool contains(const Cowstr &name) const;
  virtual bool empty() const;
  virtual int size() const;
  virtual scope_iterator begin();
  virtual scope_const_iterator begin() const;
  virtual scope_const_iterator cbegin() const;
  virtual scope_iterator end();
  virtual scope_const_iterator end() const;
  virtual scope_const_iterator cend() const;

protected:
  scope_map smap_;
};

#undef SYMBOL_INTERFACE

// symbol {

#define SYMBOL_DECLARATOR                                                      \
  virtual std::shared_ptr<Scope> owner() const;                                \
  virtual Cowstr &name();                                                      \
  virtual const Cowstr &name() const;                                          \
  virtual Location &location();                                                \
  virtual const Location &location() const;                                    \
  virtual std::shared_ptr<TypeSymbol> type() const;

class S_Var : public Symbol,
              public NameableImpl,
              public LocationableImpl,
              public OwnableImpl,
              public TypeableImpl {
public:
  S_Var(const Cowstr &name, const Location &location,
        std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Var() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Func : public Scope,
               public NameableImpl,
               public LocationableImpl,
               public OwnableImpl,
               public TypeableImpl {
public:
  S_Func(const Cowstr &name, const Location &location,
         std::shared_ptr<Scope> owner);
  virtual ~S_Func() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Param : public Symbol,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl,
                public TypeableImpl {
public:
  S_Param(const Cowstr &name, const Location &location,
          std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Param() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Field : public Symbol,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl,
                public TypeableImpl {
public:
  S_Field(const Cowstr &name, const Location &location,
          std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Field() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Method : public Scope,
                 public NameableImpl,
                 public LocationableImpl,
                 public OwnableImpl,
                 public TypeableImpl {
public:
  S_Method(const Cowstr &name, const Location &location,
           std::shared_ptr<Scope> owner);
  virtual ~S_Method() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Local : public Scope,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl,
                public TypeableImpl {
public:
  S_Local(const Cowstr &name, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~S_Local() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Global : public Scope,
                 public NameableImpl,
                 public LocationableImpl,
                 public OwnableImpl,
                 public TypeableImpl {
public:
  S_Global(const Cowstr &name, const Location &location);
  virtual ~S_Global() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

// symbol }

// type symbol {

// plain type
// integer: byte, ubyte, short, ushort, int, uint, long, ulong
// float: float, double
// boolean
// void
class Ts_Plain : public TypeSymbol,
                 public NameableImpl,
                 public LocationableImpl,
                 public OwnableImpl {
public:
  Ts_Plain(const Cowstr &name, std::shared_ptr<Scope> owner);
  virtual ~Ts_Plain() = default;
  virtual TypeSymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class Ts_Class : public TypeSymbol,
                 public Scope,
                 public NameableImpl,
                 public LocationableImpl,
                 public OwnableImpl {
public:
  Ts_Class(const Cowstr &name, const Location &location,
           std::shared_ptr<Scope> owner);
  virtual ~Ts_Class() = default;
  virtual TypeSymbolKind kind() const;
  virtual bool isSymbol() const;
  virtual bool isTypeSymbol() const;
  SYMBOL_DECLARATOR

  std::vector<std::shared_ptr<Symbol>> fields;
  std::vector<std::shared_ptr<Symbol>> methods;
};

class Ts_Func : public TypeSymbol,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl {
public:
  Ts_Func(const Cowstr &name, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~Ts_Func() = default;
  virtual TypeSymbolKind kind() const;
  SYMBOL_DECLARATOR

  std::vector<std::shared_ptr<TypeSymbol>> params;
  std::shared_ptr<TypeSymbol> result;
};

#undef SYMBOL_DECLARATOR

// type symbol }
