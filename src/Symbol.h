// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Name.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include <string>
#include <tuple>
#include <vector>

/*================ type start from 2000 ================*/
BETTER_ENUM(SymbolCategory, int,
            // variable
            Var = 2000,
            // function
            Func,
            // class
            Class, Field, Method,
            // scope
            Local, Global)

/*================ type symbol start from 3000 ================*/
BETTER_ENUM(TypeSymbolCategory, int,
            // builtin
            Builtin = 3000,
            // function
            Func,
            // class
            Class)

class Ast;
class Symbol;
class TypeSymbol;
class Scope;

class S_Var;
class S_Func;
class S_Param; // function formal parameter
class S_Class;
class S_Field;  // class member variable
class S_Method; // class method

class S_Local;  // Name to Symbol scope
class S_Global; // Name to Symbol scope

class Ts_Builtin; // builtin type
class Ts_Func;    // function type
class Ts_Class;   // class type

class Symbol : private boost::noncopyable {
public:
  Symbol(const Name &name, const Symbol *owner);
  virtual ~Symbol() = default;
  virtual SymbolCategory category() const;
  virtual const Name &name() const;
  virtual const Symbol *owner() const;
  virtual std::string toString() const;

  static const Symbol *global();

protected:
  Name name_;
  const Symbol *owner_;
};

class TypeSymbol : private boost::noncopyable {
public:
  TypeSymbol(const Name &name, const Symbol *owner);
  virtual ~TypeSymbol() = default;
  virtual TypeSymbolCategory category() const;
  virtual const Name &name() const;
  virtual const Symbol *owner() const;
  virtual std::string toString() const;

protected:
  Name name_;
  const Symbol *owner_;
};

/**
 * for example:
 *
 * `var a:int = 0;` will give:
 *
 *  symbol: a
 *  typeSymbol: int
 *  llvmValue: llvm::ConstantInt*
 */
struct SymbolData {
  Symbol *symbol;
  TypeSymbol *typeSymbol;

  SymbolData(Symbol *a_symbol = nullptr, TypeSymbol *a_typeSymbol = nullptr)
      : symbol(a_symbol), typeSymbol(a_typeSymbol) {}
};

/**
 * for example:
 *
 * `var f:()->int = () => { 1 };` will give:
 *
 * typeSymbol: ()->int
 */
struct TypeSymbolData {
  TypeSymbol *typeSymbol;

  TypeSymbolData(TypeSymbol *a_typeSymbol = nullptr)
      : typeSymbol(a_typeSymbol) {}
};

class Scope : private boost::noncopyable {
public:
  using S_Scope = LinkedHashMap<Name, SymbolData>;
  using Ts_Scope = LinkedHashMap<Name, TypeSymbolData>;
  using s_iterator = S_Scope::iterator;
  using s_const_iterator = S_Scope::const_iterator;
  using ts_iterator = Ts_Scope::iterator;
  using ts_const_iterator = Ts_Scope::const_iterator;

  // scope api
  virtual void s_define(const Symbol *s, const TypeSymbol *ts);
  virtual const SymbolData &s_resolve(const Name &name) const;
  virtual void ts_define(const TypeSymbol *symbol);
  virtual const TypeSymbolData &ts_resolve(const Name &name) const;
  virtual bool contains(const Name &name) const;

  virtual s_iterator s_begin();
  virtual s_const_iterator s_begin() const;
  virtual s_iterator s_end();
  virtual s_const_iterator s_end() const;

  virtual ts_iterator ts_begin();
  virtual ts_const_iterator ts_begin() const;
  virtual ts_iterator ts_end();
  virtual ts_const_iterator ts_end() const;

protected:
  S_Scope s_scope_;
  Ts_Scope ts_scope_;
};

// type symbol {

class Ts_Builtin : public TypeSymbol {
public:
  virtual ~Ts_Builtin() = default;
  virtual TypeSymbolCategory category() const;
  virtual std::string toString() const;

  static const Ts_Builtin *ts_byte();
  static const Ts_Builtin *ts_ubyte();
  static const Ts_Builtin *ts_short();
  static const Ts_Builtin *ts_ushort();
  static const Ts_Builtin *ts_int();
  static const Ts_Builtin *ts_uint();
  static const Ts_Builtin *ts_long();
  static const Ts_Builtin *ts_ulong();
  static const Ts_Builtin *ts_llong();
  static const Ts_Builtin *ts_ullong();
  static const Ts_Builtin *ts_float();
  static const Ts_Builtin *ts_double();
  static const Ts_Builtin *ts_boolean();
  static const Ts_Builtin *ts_character();
  static const Ts_Builtin *ts_void();

  static const Ts_Builtin *get(const Name &name);

private:
  Ts_Builtin(const Name &name);
};

class Ts_Class : public TypeSymbol {
public:
  Ts_Class(const Name &name, const Symbol *owner,
           const std::vector<Symbol *> &fields,
           const std::vector<Symbol *> &methods);
  virtual ~Ts_Class() = default;
  virtual TypeSymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<Symbol *> &fields();
  virtual const std::vector<Symbol *> &fields() const;
  virtual std::vector<Symbol *> &methods();
  virtual const std::vector<Symbol *> &methods() const;

private:
  std::vector<Symbol *> fields_;
  std::vector<Symbol *> methods_;
};

class Ts_Func : public TypeSymbol {
public:
  Ts_Func(const Name &name, const Symbol *owner,
          const std::vector<TypeSymbol *> &paramTypes,
          const TypeSymbol *resultType);
  virtual ~Ts_Func();
  virtual TypeSymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<TypeSymbol *> &params();
  virtual const std::vector<TypeSymbol *> &params() const;
  virtual TypeSymbol *&result();
  virtual const TypeSymbol *result() const;

private:
  std::vector<TypeSymbol *> paramTypes_;
  TypeSymbol *resultType_;
};

// type symbol }

// symbol {

class S_Var : public Symbol {
public:
  S_Var(const Name &name, const Symbol *owner);
  virtual ~S_Var() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;
};

class S_Func : public Symbol, public Scope {
public:
  S_Func(const Name &name, const Symbol *owner,
         const std::vector<Symbol *> &params);
  virtual ~S_Func() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<Symbol *> &params();
  virtual const std::vector<Symbol *> &params() const;

private:
  std::vector<Symbol *> params_;
};

class S_Param : public Symbol {
public:
  S_Param(const Name &name, const Symbol *owner);
  virtual ~S_Param() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;
};

class S_Class : public Symbol, public Scope {
public:
  S_Class(const Name &name, const Symbol *owner,
          const std::vector<Symbol *> &fields,
          const std::vector<Symbol *> &methods);
  virtual ~S_Class() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<Symbol *> &fields();
  virtual const std::vector<Symbol *> &fields() const;
  virtual std::vector<Symbol *> &methods();
  virtual const std::vector<Symbol *> &methods() const;

private:
  std::vector<Symbol *> fields_;
  std::vector<Symbol *> methods_;
};

class S_Field : public Symbol {
public:
  S_Field(const Name &name, const Symbol *owner);
  virtual ~S_Field() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;
};

class S_Method : public Symbol, public Scope {
public:
  S_Method(const Name &name, const Symbol *owner,
           const std::vector<Symbol *> &params);
  virtual ~S_Method() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<Symbol *> &params();
  virtual const std::vector<Symbol *> &params() const;

private:
  std::vector<Symbol *> params_;
};

class S_Local : public Symbol, public Scope {
public:
  S_Local(const Name &name, const Symbol *owner);
  virtual ~S_Local() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;
};

class S_Global : public Symbol, public Scope {
public:
  S_Global(const Name &name, const Symbol *owner);
  virtual ~S_Global() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;
};

// symbol }
