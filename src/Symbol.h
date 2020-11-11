// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "AstClasses.h"
#include "SymbolClasses.h"
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include "iface/Identifiable.h"
#include "iface/LLVMTypable.h"
#include "iface/LLVMValuable.h"
#include "iface/Locationable.h"
#include "iface/Nameable.h"
#include "infra/LinkedHashMap.h"
#include "infra/Log.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <functional>

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

namespace detail {

class Ownable {
public:
  Ownable(Scope *owner = nullptr);
  virtual ~Ownable() = default;
  virtual Scope *&owner();
  virtual Scope *owner() const;

protected:
  Scope *ownable_;
};

class Typeable {
public:
  Typeable(TypeSymbol *type = nullptr);
  virtual ~Typeable() = default;
  virtual TypeSymbol *&type();
  virtual TypeSymbol *type() const;

protected:
  TypeSymbol *typeable_;
};

class Astable {
public:
  Astable(Ast *ast = nullptr);
  virtual ~Astable() = default;
  virtual Ast *&ast();
  virtual Ast *ast() const;

protected:
  Ast *astable_;
};

} // namespace detail

/**
 * Symbol/TypeSymbol/Scope relationship:
 *
 * Symbol: Var, Param
 * Symbol+Scope: Func, Method
 * TypeSymbol: Plain, Func
 * TypeSymbol+Scope: Class
 * Scope: Local, Global
 *
 * TypeSymbol is a specilized Symbol
 */
class Symbol : public virtual Nameable,
               public virtual Locationable,
               public virtual Identifiable,
               public virtual detail::Ownable,
               public virtual detail::Astable,
               public virtual LLVMValuable,
               public detail::Typeable,
               private boost::noncopyable {
public:
  Symbol(TypeSymbol *type = nullptr);
  virtual ~Symbol() = default;
  virtual SymbolKind kind() const = 0;

  static bool isDefined(A_VarId *ast);
};

class TypeSymbol : public virtual Nameable,
                   public virtual Locationable,
                   public virtual Identifiable,
                   public virtual detail::Ownable,
                   public virtual detail::Astable,
                   public virtual LLVMTypable,
                   private boost::noncopyable {
public:
  virtual ~TypeSymbol() = default;
  virtual TypeSymbolKind kind() const = 0;

  static bool isDefined(A_VarId *ast);
  static TypeSymbol *ts_byte();
  static TypeSymbol *ts_ubyte();
  static TypeSymbol *ts_short();
  static TypeSymbol *ts_ushort();
  static TypeSymbol *ts_int();
  static TypeSymbol *ts_uint();
  static TypeSymbol *ts_long();
  static TypeSymbol *ts_ulong();
  static TypeSymbol *ts_float();
  static TypeSymbol *ts_double();
  static TypeSymbol *ts_char();
  static TypeSymbol *ts_boolean();
  static TypeSymbol *ts_void();
};

class Scope : public virtual Nameable,
              public virtual Locationable,
              public virtual Identifiable,
              public virtual detail::Ownable,
              public virtual detail::Astable,
              public virtual LLVMValuable,
              public virtual LLVMTypable,
              private boost::noncopyable {
public:
  using s_map = LinkedHashMap<Cowstr, Symbol *>;
  using s_iterator = s_map::iterator;
  using s_const_iterator = s_map::const_iterator;
  using ts_map = LinkedHashMap<Cowstr, TypeSymbol *>;
  using ts_iterator = ts_map::iterator;
  using ts_const_iterator = ts_map::const_iterator;
  using scope_map = LinkedHashMap<Cowstr, Scope *>;
  using sc_iterator = scope_map::iterator;
  using sc_const_iterator = scope_map::const_iterator;

  virtual ~Scope() = default;

  /**
   * symbol interface
   */
  virtual void s_define(Symbol *symbol) = 0;
  virtual Symbol *s_resolve(const Cowstr &name) const = 0;
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
  virtual void ts_define(TypeSymbol *symbol) = 0;
  virtual TypeSymbol *ts_resolve(const Cowstr &name) const = 0;
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
  virtual void subscope_define(Scope *scope) = 0;
  virtual Scope *subscope_resolve(const Cowstr &name) const = 0;
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
  virtual ~ScopeImpl();

  /**
   * symbol interface
   */
  virtual void s_define(Symbol *symbol);
  virtual Symbol *s_resolve(const Cowstr &name) const;
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
  virtual void ts_define(TypeSymbol *symbol);
  virtual TypeSymbol *ts_resolve(const Cowstr &name) const;
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
  virtual void subscope_define(Scope *scope);
  virtual Scope *subscope_resolve(const Cowstr &name) const;
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

class S_Var : public Symbol {
public:
  S_Var(const Cowstr &name, const Location &location, Scope *owner,
        TypeSymbol *type);
  virtual ~S_Var() = default;
  virtual SymbolKind kind() const;
};

class S_Func : public Symbol, public detail::ScopeImpl {
public:
  S_Func(const Cowstr &name, const Location &location, Scope *owner,
         TypeSymbol *type);
  virtual ~S_Func() = default;
  virtual SymbolKind kind() const;

  std::vector<Symbol *> params;
};

class S_Param : public Symbol {
public:
  S_Param(const Cowstr &name, const Location &location, Scope *owner,
          TypeSymbol *type);
  virtual ~S_Param() = default;
  virtual SymbolKind kind() const;
};

class S_Field : public Symbol {
public:
  S_Field(const Cowstr &name, const Location &location, Scope *owner,
          TypeSymbol *type);
  virtual ~S_Field() = default;
  virtual SymbolKind kind() const;
};

class S_Method : public Symbol, public detail::ScopeImpl {
public:
  S_Method(const Cowstr &name, const Location &location, Scope *owner,
           TypeSymbol *type);
  virtual ~S_Method() = default;
  virtual SymbolKind kind() const;

  std::vector<Symbol *> params;
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
class Ts_Plain : public TypeSymbol {
public:
  Ts_Plain(const Cowstr &name);
  virtual ~Ts_Plain() = default;
  virtual TypeSymbolKind kind() const;
};

class Ts_Class : public TypeSymbol, public detail::ScopeImpl {
public:
  Ts_Class(const Cowstr &name, const Location &location, Scope *owner);
  virtual ~Ts_Class() = default;
  virtual TypeSymbolKind kind() const;

  std::vector<Symbol *> fields;
  std::vector<Symbol *> methods;
};

class Ts_Func : public TypeSymbol {
public:
  Ts_Func(const std::vector<TypeSymbol *> &a_params, TypeSymbol *a_result,
          const Location &location, Scope *owner);
  virtual ~Ts_Func() = default;
  virtual TypeSymbolKind kind() const;

  std::vector<TypeSymbol *> params;
  TypeSymbol *result;
};

// type symbol }

// scope {

class Sc_Local : public detail::ScopeImpl {
public:
  Sc_Local(const Cowstr &name, const Location &location, Scope *owner);
  virtual ~Sc_Local() = default;
};

class Sc_Global : public detail::ScopeImpl {
public:
  Sc_Global(const Cowstr &name, const Location &location);
  virtual ~Sc_Global() = default;
};

// scope }
