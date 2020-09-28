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
            Local, Global)

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
 *    PlainType: int, long, boolean, void, etc
 *    FuncType: (int, int)=>int, etc
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

class Symbol : public Nameable,
               public Locationable,
               public Ownable,
               private boost::noncopyable {
public:
  virtual ~Symbol() = default;
  virtual SymbolKind kind() const = 0;
  virtual std::shared_ptr<Scope> owner() const = 0;
  virtual Cowstr &name() = 0;
  virtual const Cowstr &name() const = 0;
  virtual Location &location() = 0;
  virtual const Location &location() const = 0;
};

class TypeSymbol : public Nameable,
                   public Locationable,
                   public Ownable,
                   private boost::noncopyable {
public:
  virtual ~TypeSymbol() = default;
  virtual TypeSymbolKind kind() const = 0;
  virtual std::shared_ptr<Scope> owner() const = 0;
  virtual Cowstr &name() = 0;
  virtual const Cowstr &name() const = 0;
  virtual Location &location() = 0;
  virtual const Location &location() const = 0;
};

struct SymbolData {
  std::shared_ptr<Symbol> symbol;
  std::shared_ptr<TypeSymbol> typeSymbol;

  SymbolData(std::shared_ptr<Symbol> a_symbol,
             std::shared_ptr<TypeSymbol> a_typeSymbol)
      : symbol(a_symbol), typeSymbol(a_typeSymbol) {}
  virtual ~SymbolData() = default;
};

struct TypeSymbolData {
  std::shared_ptr<TypeSymbol> typeSymbol;

  TypeSymbolData(std::shared_ptr<TypeSymbol> a_typeSymbol)
      : typeSymbol(a_typeSymbol) {}
  virtual ~TypeSymbolData() = default;
};

class Scope : public Nameable,
              public Locationable,
              public Ownable,
              private boost::noncopyable {
public:
  using s_map = LinkedHashMap<Cowstr, SymbolData>;
  using ts_map = LinkedHashMap<Cowstr, TypeSymbolData>;
  using s_iterator = s_map::iterator;
  using s_const_iterator = s_map::const_iterator;
  using ts_iterator = ts_map::iterator;
  using ts_const_iterator = ts_map::const_iterator;

  virtual ~Scope() = default;

  static std::shared_ptr<Scope> from(Ast *compileUnit);
  // static void check(Ast *compileUnit, std::shared_ptr<Symbol> symbol);

  virtual std::shared_ptr<Scope> owner() const = 0;
  virtual Cowstr &name() = 0;
  virtual const Cowstr &name() const = 0;
  virtual Location &location() = 0;
  virtual const Location &location() const = 0;
  virtual bool isSymbol() const = 0;
  virtual bool isTypeSymbol() const = 0;

  // scope api
  virtual void s_define(std::shared_ptr<Symbol> s,
                        std::shared_ptr<TypeSymbol> ts);
  virtual void ts_define(std::shared_ptr<TypeSymbol> ts);
  virtual SymbolData s_resolve(const Cowstr &name) const;
  virtual TypeSymbolData ts_resolve(const Cowstr &name) const;
  virtual bool s_resolveContains(const Cowstr &name) const;
  virtual bool ts_resolveContains(const Cowstr &name) const;

  // work only in plain level
  virtual bool s_contains(const Cowstr &name) const;
  virtual bool ts_contains(const Cowstr &name) const;
  virtual bool s_empty() const;
  virtual bool ts_empty() const;
  virtual int s_size() const;
  virtual int ts_size() const;

  virtual s_iterator s_begin();
  virtual s_const_iterator s_begin() const;
  virtual s_const_iterator s_cbegin() const;
  virtual s_iterator s_end();
  virtual s_const_iterator s_end() const;
  virtual s_const_iterator s_cend() const;

  virtual ts_iterator ts_begin();
  virtual ts_const_iterator ts_begin() const;
  virtual ts_const_iterator ts_cbegin() const;
  virtual ts_iterator ts_end();
  virtual ts_const_iterator ts_end() const;
  virtual ts_const_iterator ts_cend() const;

protected:
  s_map smap_;
  ts_map tsmap_;
};

// symbol {

#define SYMBOL_DECLARATOR                                                      \
  virtual Cowstr &name();                                                      \
  virtual const Cowstr &name() const;                                          \
  virtual Location &location();                                                \
  virtual const Location &location() const;                                    \
  virtual std::shared_ptr<Scope> owner() const;

class S_Var : public Symbol,
              public NameableImpl,
              public LocationableImpl,
              public OwnableImpl {
public:
  S_Var(const Cowstr &name, const Location &location,
        std::shared_ptr<Scope> owner);
  virtual ~S_Var() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Func : public Symbol,
               public Scope,
               public NameableImpl,
               public LocationableImpl,
               public OwnableImpl {
public:
  S_Func(const Cowstr &name, const Location &location,
         std::shared_ptr<Scope> owner);
  virtual ~S_Func() = default;
  virtual SymbolKind kind() const;
  virtual bool isSymbol() const;
  virtual bool isTypeSymbol() const;
  SYMBOL_DECLARATOR
  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Param : public Symbol,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl {
public:
  S_Param(const Cowstr &name, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~S_Param() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Field : public Symbol,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl {
public:
  S_Field(const Cowstr &name, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~S_Field() = default;
  virtual SymbolKind kind() const;
  SYMBOL_DECLARATOR
};

class S_Method : public Symbol,
                 public Scope,
                 public NameableImpl,
                 public LocationableImpl,
                 public OwnableImpl {
public:
  S_Method(const Cowstr &name, const Location &location,
           std::shared_ptr<Scope> owner);
  virtual ~S_Method() = default;
  virtual SymbolKind kind() const;
  virtual bool isSymbol() const;
  virtual bool isTypeSymbol() const;
  SYMBOL_DECLARATOR

  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Local : public Symbol,
                public Scope,
                public NameableImpl,
                public LocationableImpl,
                public OwnableImpl {
public:
  S_Local(const Cowstr &name, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~S_Local() = default;
  virtual SymbolKind kind() const;
  virtual bool isSymbol() const;
  virtual bool isTypeSymbol() const;
  SYMBOL_DECLARATOR
};

class S_Global : public Symbol,
                 public Scope,
                 public NameableImpl,
                 public LocationableImpl,
                 public OwnableImpl {
public:
  S_Global(const Cowstr &name, const Location &location);
  virtual ~S_Global() = default;
  virtual SymbolKind kind() const;
  virtual bool isSymbol() const;
  virtual bool isTypeSymbol() const;
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
