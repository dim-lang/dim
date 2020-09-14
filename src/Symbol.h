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
            // variable
            Var = 2000,
            // function
            Func, Param,
            // class
            Class, Field, Method,
            // scope
            Local, Global)

BETTER_ENUM(TypeSymbolKind, int,
            // builtin
            Builtin = 3000,
            // function
            Func,
            // class
            Class)

class Symbol : public Nameable,
               public Locationable,
               private boost::noncopyable {
public:
  Symbol(const Cowstr &name, const Location &location,
         std::shared_ptr<Symbol> owner);
  virtual ~Symbol() = default;
  virtual std::shared_ptr<Symbol> owner() const;
  virtual SymbolKind kind() const = 0;

  // virtual Cowstr &name();
  // virtual const Cowstr &name() const;
  // virtual Location &location();
  // virtual const Location &location() const;

private:
  std::shared_ptr<Symbol> owner_;
};

class TypeSymbol : public Nameable,
                   public Locationable,
                   private boost::noncopyable {
public:
  TypeSymbol(const Cowstr &name, const Location &location,
             std::shared_ptr<Symbol> owner);
  virtual ~TypeSymbol() = default;
  virtual std::shared_ptr<Symbol> owner() const;
  virtual TypeSymbolKind kind() const = 0;

  // virtual Cowstr &name();
  // virtual const Cowstr &name() const;
  // virtual Location &location();
  // virtual const Location &location() const;

private:
  std::shared_ptr<Symbol> owner_;
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

class Scope : public Symbol {
public:
  using s_map = LinkedHashMap<Cowstr, SymbolData>;
  using ts_map = LinkedHashMap<Cowstr, TypeSymbolData>;
  using s_iterator = s_map::iterator;
  using s_const_iterator = s_map::const_iterator;
  using ts_iterator = ts_map::iterator;
  using ts_const_iterator = ts_map::const_iterator;

  Scope(const Cowstr &name, const Location &location,
        std::shared_ptr<Symbol> owner);
  virtual ~Scope() = default;

  virtual void define(std::shared_ptr<Symbol> s,
                      std::shared_ptr<TypeSymbol> ts);
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

class S_Var : public Symbol {
public:
  S_Var(const Cowstr &name, const Location &location,
        std::shared_ptr<Symbol> owner);
  virtual ~S_Var() = default;
  virtual SymbolKind kind() const;
};

class S_Func : public Scope {
public:
  S_Func(const Cowstr &name, const Location &location,
         std::shared_ptr<Symbol> owner,
         const std::vector<std::shared_ptr<Symbol>> &a_params);
  virtual ~S_Func() = default;
  virtual SymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Param : public Symbol {
public:
  S_Param(const Cowstr &name, const Location &location,
          std::shared_ptr<Symbol> owner);
  virtual ~S_Param() = default;
  virtual SymbolKind kind() const;
};

class S_Class : public Scope {
public:
  S_Class(const Cowstr &name, const Location &location,
          std::shared_ptr<Symbol> owner,
          const std::vector<std::shared_ptr<Symbol>> &a_fields,
          const std::vector<std::shared_ptr<Symbol>> &a_methods);
  virtual ~S_Class() = default;
  virtual SymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> fields;
  std::vector<std::shared_ptr<Symbol>> methods;
};

class S_Field : public Symbol {
public:
  S_Field(const Cowstr &name, const Location &location,
          std::shared_ptr<Symbol> owner);
  virtual ~S_Field() = default;
  virtual SymbolKind kind() const;
};

class S_Method : public Scope {
public:
  S_Method(const Cowstr &name, const Location &location,
           std::shared_ptr<Symbol> owner,
           const std::vector<std::shared_ptr<Symbol>> &a_params);
  virtual ~S_Method() = default;
  virtual SymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Local : public Scope {
public:
  S_Local(const Cowstr &name, const Location &location,
          std::shared_ptr<Symbol> owner);
  virtual ~S_Local() = default;
  virtual SymbolKind kind() const;
};

class S_Global : public Scope {
public:
  virtual ~S_Global() = default;
  virtual SymbolKind kind() const;

  static std::shared_ptr<S_Global> global;

private:
  S_Global();
};

// symbol }

// type symbol {

class Ts_Builtin : public TypeSymbol {
public:
  Ts_Builtin(const Cowstr &name, const Location &location,
             std::shared_ptr<Symbol> owner);
  virtual ~Ts_Builtin() = default;
  virtual TypeSymbolKind kind() const;
};

class Ts_Class : public TypeSymbol {
public:
  Ts_Class(const Cowstr &name, const Location &location,
           std::shared_ptr<Symbol> owner,
           const std::vector<std::shared_ptr<Symbol>> &a_fields,
           const std::vector<std::shared_ptr<Symbol>> &a_methods);
  virtual ~Ts_Class() = default;
  virtual TypeSymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> fields;
  std::vector<std::shared_ptr<Symbol>> methods;
};

class Ts_Func : public TypeSymbol {
public:
  Ts_Func(const Cowstr &name, const Location &location,
          std::shared_ptr<Symbol> owner,
          const std::vector<std::shared_ptr<TypeSymbol>> &a_params,
          std::shared_ptr<TypeSymbol> a_result);
  virtual ~Ts_Func();
  virtual TypeSymbolKind kind() const;

  std::vector<std::shared_ptr<TypeSymbol>> params;
  std::shared_ptr<TypeSymbol> result;
};

// type symbol }
