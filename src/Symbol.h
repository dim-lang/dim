// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "LinkedHashMap.h"
#include "Location.h"
#include "Log.h"
#include "Name.h"
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include <memory>

BETTER_ENUM(SymbolKind, int,
            // symbol
            // variable
            Var = 2000,
            // function
            Func, Param,
            // class
            Field, Method)

BETTER_ENUM(TypeSymbolKind, int,
            // symbol
            // variable
            Plain = 3000,
            // function
            Func,
            // class
            Class)

BETTER_ENUM(ScopeKind, int, Global = 4000, Local)

class Ast;
class Symbol;
class TypeSymbol;
class Scope;

/**
 * relationship between Symbol, TypeSymbol and Scope:
 *
 * Symbol = Var, Param
 * Symbol+Scope = Func, Method
 * TypeSymbol =
 *    Plain: int, long, boolean, void, etc
 *    Func: (int, int)=>int, etc
 * TypeSymbol+Scope = Class
 * Scope = Local, Global
 *
 * TypeSymbol is a specilized Symbol
 */

namespace detail {

class Ownable {
public:
  virtual ~Ownable() = default;
  virtual std::shared_ptr<Scope> &owner() = 0;
  virtual std::shared_ptr<Scope> owner() const = 0;
};

class OwnableImpl : public Ownable {
public:
  OwnableImpl(std::shared_ptr<Scope> owner);
  virtual ~OwnableImpl() = default;
  virtual std::shared_ptr<Scope> &owner();
  virtual std::shared_ptr<Scope> owner() const;

protected:
  std::shared_ptr<Scope> ownableImpl_;
};

class Typeable {
public:
  virtual ~Typeable() = default;
  virtual std::shared_ptr<TypeSymbol> &type() = 0;
  virtual std::shared_ptr<TypeSymbol> type() const = 0;
};

class TypeableImpl : public Typeable {
public:
  TypeableImpl(std::shared_ptr<TypeSymbol> typeableImpl);
  virtual ~TypeableImpl() = default;
  virtual std::shared_ptr<TypeSymbol> &type();
  virtual std::shared_ptr<TypeSymbol> type() const;

protected:
  std::shared_ptr<TypeSymbol> typeableImpl_;
};

class Astable {
public:
  virtual ~Astable() = default;
  virtual Ast *&ast() = 0;
  virtual Ast *ast() const = 0;
};

class AstableImpl : public Astable {
public:
  AstableImpl(Ast *astableImpl = nullptr);
  virtual ~AstableImpl() = default;
  virtual Ast *&ast();
  virtual Ast *ast() const;

protected:
  Ast *astableImpl_;
};

} // namespace detail

class Symbol : public Nameable,
               public Locationable,
               public detail::Typeable,
               public detail::Ownable,
               private boost::noncopyable {
public:
  virtual ~Symbol() = default;
  virtual SymbolKind kind() const = 0;

  static bool isSymbol(std::shared_ptr<Symbol> sym);
  static bool isTypeSymbol(std::shared_ptr<Symbol> sym);
};

class TypeSymbol : public Nameable,
                   public Locationable,
                   public detail::Ownable,
                   private boost::noncopyable {
public:
  virtual ~TypeSymbol() = default;
  virtual TypeSymbolKind kind() const = 0;

  static std::shared_ptr<TypeSymbol> ts_byte();
  static std::shared_ptr<TypeSymbol> ts_ubyte();
  static std::shared_ptr<TypeSymbol> ts_short();
  static std::shared_ptr<TypeSymbol> ts_ushort();
  static std::shared_ptr<TypeSymbol> ts_int();
  static std::shared_ptr<TypeSymbol> ts_uint();
  static std::shared_ptr<TypeSymbol> ts_long();
  static std::shared_ptr<TypeSymbol> ts_ulong();
  static std::shared_ptr<TypeSymbol> ts_float();
  static std::shared_ptr<TypeSymbol> ts_double();
  static std::shared_ptr<TypeSymbol> ts_char();
  static std::shared_ptr<TypeSymbol> ts_boolean();
  static std::shared_ptr<TypeSymbol> ts_void();
};

class Scope : public Nameable,
              public Locationable,
              public detail::Ownable,
              private boost::noncopyable {
public:
  using s_map = LinkedHashMap<Cowstr, std::shared_ptr<Symbol>>;
  using s_iterator = s_map::iterator;
  using s_const_iterator = s_map::const_iterator;
  using ts_map = LinkedHashMap<Cowstr, std::shared_ptr<TypeSymbol>>;
  using ts_iterator = ts_map::iterator;
  using ts_const_iterator = ts_map::const_iterator;
  using scope_map = LinkedHashMap<Cowstr, std::shared_ptr<Scope>>;
  using sc_iterator = scope_map::iterator;
  using sc_const_iterator = scope_map::const_iterator;

  virtual ~Scope() = default;

  static void create(Ast *compileUnit);

  /**
   * symbol interface
   */
  virtual void s_define(std::shared_ptr<Symbol> symbol) = 0;
  virtual std::shared_ptr<Symbol> s_resolve(const Cowstr &name) const = 0;
  virtual bool s_contains(const Cowstr &name) const = 0;
  virtual bool s_empty() const = 0;
  virtual int s_size() const = 0;
  virtual s_iterator s_begin() = 0;
  virtual s_const_iterator s_begin() const = 0;
  virtual s_const_iterator s_cbegin() const = 0;
  virtual s_iterator s_end() = 0;
  virtual s_const_iterator s_end() const = 0;
  virtual s_const_iterator s_cend() const = 0;

  /**
   * type symbol interface
   */
  virtual void ts_define(std::shared_ptr<TypeSymbol> symbol) = 0;
  virtual std::shared_ptr<TypeSymbol> ts_resolve(const Cowstr &name) const = 0;
  virtual bool ts_contains(const Cowstr &name) const = 0;
  virtual bool ts_empty() const = 0;
  virtual int ts_size() const = 0;
  virtual ts_iterator ts_begin() = 0;
  virtual ts_const_iterator ts_begin() const = 0;
  virtual ts_const_iterator ts_cbegin() const = 0;
  virtual ts_iterator ts_end() = 0;
  virtual ts_const_iterator ts_end() const = 0;
  virtual ts_const_iterator ts_cend() const = 0;

  /**
   * scope interface
   */
  virtual void subscope_define(std::shared_ptr<Scope> scope) = 0;
  virtual std::shared_ptr<Scope> subscope_resolve(const Cowstr &name) const = 0;
  virtual bool subscope_contains(const Cowstr &name) const = 0;
  virtual bool subscope_empty() const = 0;
  virtual int subscope_size() const = 0;
  virtual sc_iterator subscope_begin() = 0;
  virtual sc_const_iterator subscope_begin() const = 0;
  virtual sc_const_iterator subscope_cbegin() const = 0;
  virtual sc_iterator subscope_end() = 0;
  virtual sc_const_iterator subscope_end() const = 0;
  virtual sc_const_iterator subscope_cend() const = 0;
};

namespace detail {

class ScopeImpl : public Scope {
public:
  virtual ~ScopeImpl() = default;

  /**
   * symbol interface
   */
  virtual void s_define(std::shared_ptr<Symbol> symbol);
  virtual std::shared_ptr<Symbol> s_resolve(const Cowstr &name) const;
  virtual bool s_contains(const Cowstr &name) const;
  virtual bool s_empty() const;
  virtual int s_size() const;
  virtual s_iterator s_begin();
  virtual s_const_iterator s_begin() const;
  virtual s_const_iterator s_cbegin() const;
  virtual s_iterator s_end();
  virtual s_const_iterator s_end() const;
  virtual s_const_iterator s_cend() const;

  /**
   * type symbol interface
   */
  virtual void ts_define(std::shared_ptr<TypeSymbol> symbol);
  virtual std::shared_ptr<TypeSymbol> ts_resolve(const Cowstr &name) const;
  virtual bool ts_contains(const Cowstr &name) const;
  virtual bool ts_empty() const;
  virtual int ts_size() const;
  virtual ts_iterator ts_begin();
  virtual ts_const_iterator ts_begin() const;
  virtual ts_const_iterator ts_cbegin() const;
  virtual ts_iterator ts_end();
  virtual ts_const_iterator ts_end() const;
  virtual ts_const_iterator ts_cend() const;

  /**
   * scope interface
   */
  virtual void subscope_insert(std::shared_ptr<Scope> scope);
  virtual std::shared_ptr<Scope> subscope_find(const Cowstr &name) const;
  virtual bool subscope_contains(const Cowstr &name) const;
  virtual bool subscope_empty() const;
  virtual int subscope_size() const;
  virtual sc_iterator subscope_begin();
  virtual sc_const_iterator subscope_begin() const;
  virtual sc_const_iterator subscope_cbegin() const;
  virtual sc_iterator subscope_end();
  virtual sc_const_iterator subscope_end() const;
  virtual sc_const_iterator subscope_cend() const;

protected:
  s_map s_data_;
  ts_map ts_data_;
  scope_map sc_data_;
};

} // namespace detail

// symbol {

class S_Var : public Symbol,
              public NameableImpl,
              public LocationableImpl,
              public detail::TypeableImpl,
              public detail::OwnableImpl,
              public detail::AstableImpl {
public:
  S_Var(const Cowstr &name, const Location &location,
        std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Var() = default;
  virtual SymbolKind kind() const;
};

class S_Func : public Symbol,
               public NameableImpl,
               public LocationableImpl,
               public detail::TypeableImpl,
               public detail::OwnableImpl,
               public detail::AstableImpl,
               public detail::ScopeImpl {
public:
  S_Func(const Cowstr &name, const Location &location,
         std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Func() = default;
  virtual SymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> params;
};

class S_Param : public Symbol,
                public NameableImpl,
                public LocationableImpl,
                public detail::TypeableImpl,
                public detail::OwnableImpl,
                public detail::AstableImpl {
public:
  S_Param(const Cowstr &name, const Location &location,
          std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Param() = default;
  virtual SymbolKind kind() const;
};

class S_Field : public Symbol,
                public NameableImpl,
                public LocationableImpl,
                public detail::TypeableImpl,
                public detail::OwnableImpl,
                public detail::AstableImpl {
public:
  S_Field(const Cowstr &name, const Location &location,
          std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Field() = default;
  virtual SymbolKind kind() const;
};

class S_Method : public Symbol,
                 public NameableImpl,
                 public LocationableImpl,
                 public detail::TypeableImpl,
                 public detail::OwnableImpl,
                 public detail::AstableImpl,
                 public detail::ScopeImpl {
public:
  S_Method(const Cowstr &name, const Location &location,
           std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner);
  virtual ~S_Method() = default;
  virtual SymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> params;
};

// symbol }

// type symbol {

/**
 * plain type
 *  integer: byte, ubyte, short, ushort, int, uint, long, ulong
 *  float: float, double
 *  char
 *  boolean
 *  void
 */
class Ts_Plain : public TypeSymbol,
                 public NameableImpl,
                 public LocationableImpl,
                 public detail::OwnableImpl {
public:
  Ts_Plain(const Cowstr &name);
  virtual ~Ts_Plain() = default;
  virtual TypeSymbolKind kind() const;
};

class Ts_Class : public TypeSymbol,
                 public NameableImpl,
                 public LocationableImpl,
                 public detail::OwnableImpl,
                 public detail::ScopeImpl {
public:
  Ts_Class(const Cowstr &name, const Location &location,
           std::shared_ptr<Scope> owner);
  virtual ~Ts_Class() = default;
  virtual TypeSymbolKind kind() const;

  std::vector<std::shared_ptr<Symbol>> fields;
  std::vector<std::shared_ptr<Symbol>> methods;
};

class Ts_Func : public TypeSymbol,
                public NameableImpl,
                public LocationableImpl,
                public detail::OwnableImpl {
public:
  Ts_Func(const std::vector<std::shared_ptr<TypeSymbol>> a_params,
          std::shared_ptr<TypeSymbol> a_result, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~Ts_Func() = default;
  virtual TypeSymbolKind kind() const;

  std::vector<std::shared_ptr<TypeSymbol>> params;
  std::shared_ptr<TypeSymbol> result;

private:
  virtual Cowstr
  generateName(const std::vector<std::shared_ptr<TypeSymbol>> &params,
               std::shared_ptr<TypeSymbol> result);
};

// type symbol }

// scope {

class S_Local : public detail::ScopeImpl,
                public NameableImpl,
                public LocationableImpl,
                public detail::OwnableImpl,
                public detail::AstableImpl {
public:
  S_Local(const Cowstr &name, const Location &location,
          std::shared_ptr<Scope> owner);
  virtual ~S_Local() = default;
  virtual ScopeKind kind() const;
};

class S_Global : public detail::ScopeImpl,
                 public NameableImpl,
                 public LocationableImpl,
                 public detail::OwnableImpl {
public:
  S_Global(const Cowstr &name, const Location &location);
  virtual ~S_Global() = default;
  virtual ScopeKind kind() const;
};

// scope }
