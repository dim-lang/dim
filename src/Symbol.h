// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Name.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include "interface/Namely.h"
#include "interface/Stringify.h"
#include "llvm/IR/Value.h"
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
            Class, Member, Method,
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
class S_Class;
class S_Member;
class S_Method;
class S_Local;
class S_Global;

class Ts_Builtin;
class Ts_Func;
class Ts_Class;

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
 * ```
 * var a:int = 0;
 * ```
 *
 * will give us:
 *
 * ```
 *  symbol: a
 *  typeSymbol: int
 *  llvmValue: llvm::ConstantInt*
 * ```
 */
struct SymbolData {
  Symbol *symbol;
  TypeSymbol *typeSymbol;
  llvm::Value *llvmValue;

  SymbolData(Symbol *a_symbol = nullptr, TypeSymbol *a_typeSymbol = nullptr,
             llvm::Value *a_llvmValue = nullptr)
      : symbol(a_symbol), typeSymbol(a_typeSymbol), llvmValue(a_llvmValue) {}
};

/**
 * for example:
 *
 * ```
 * var f:()->int = () => { 1 };
 * ```
 *
 * will give us:
 *
 * ```
 * typeSymbol: ()->int
 * ```
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
  void s_define(const Symbol *s, const TypeSymbol *ts);
  const SymbolData &s_resolve(const Name &name) const;
  void ts_define(const TypeSymbol *symbol);
  const TypeSymbolData &ts_resolve(const Name &name) const;

  s_iterator s_begin();
  s_const_iterator s_begin() const;
  s_iterator s_end();
  s_const_iterator s_end() const;

  ts_iterator ts_begin();
  ts_const_iterator ts_begin() const;
  ts_iterator ts_end();
  ts_const_iterator ts_end() const;

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
           const std::vector<Symbol *> &fields);
  virtual ~Ts_Class() = default;
  virtual TypeSymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<Symbol *> &fields();
  virtual const std::vector<Symbol *> &fields() const;

  static const Ts_Class *ts_Any();
  static const Ts_Class *ts_Byte();
  static const Ts_Class *ts_UByte();
  static const Ts_Class *ts_Short();
  static const Ts_Class *ts_UShort();
  static const Ts_Class *ts_Int();
  static const Ts_Class *ts_UInt();
  static const Ts_Class *ts_Long();
  static const Ts_Class *ts_ULong();
  static const Ts_Class *ts_LLong();
  static const Ts_Class *ts_ULLong();
  static const Ts_Class *ts_Float();
  static const Ts_Class *ts_Double();
  static const Ts_Class *ts_Boolean();
  static const Ts_Class *ts_Character();
  static const Ts_Class *ts_String();

  static const Ts_Class *get(const Name &name);

private:
  std::vector<Symbol *> fields_;
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

class S_Class : public Symbol, public Scope {
public:
  S_Class(const Name &name, const Symbol *owner,
          const std::vector<Symbol *> &members,
          const std::vector<Symbol *> &methods);
  virtual ~S_Class() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<Symbol *> &members();
  virtual const std::vector<Symbol *> &members() const;
  virtual std::vector<Symbol *> &methods();
  virtual const std::vector<Symbol *> &methods() const;

private:
  std::vector<Symbol *> members_;
  std::vector<Symbol *> methods_;
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
