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
  virtual const Name &name() const;
  virtual const Symbol *owner() const;
  virtual SymbolCategory category() const = 0;
  virtual std::string toString() const = 0;

  static const Symbol *global();

protected:
  Name name_;
  const Symbol *owner_;
};

class TypeSymbol : private boost::noncopyable {
public:
  TypeSymbol(const Name &name, const Symbol *owner);
  virtual ~TypeSymbol() = default;
  virtual const Name &name() const;
  virtual const Symbol *owner() const;
  virtual TypeSymbolCategory category() const = 0;
  virtual std::string toString() const = 0;

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
  Position position;
  bool escape;

  SymbolData(Symbol *a_symbol = nullptr, TypeSymbol *a_typeSymbol = nullptr,
             const Position &a_position = Position(), bool a_escape = false)
      : symbol(a_symbol), typeSymbol(a_typeSymbol), position(a_position),
        escape(a_escape) {}
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
  Position position;
  bool escape;

  TypeSymbolData(TypeSymbol *a_typeSymbol = nullptr,
                 const Position &position = Position(), bool a_escape = false)
      : typeSymbol(a_typeSymbol), position(a_position), escape(a_escape) {}
};

class Scope : public Symbol {
public:
  using S_Scope = LinkedHashMap<Name, SymbolData>;
  using Ts_Scope = LinkedHashMap<Name, TypeSymbolData>;
  using s_iterator = S_Scope::iterator;
  using s_const_iterator = S_Scope::const_iterator;
  using ts_iterator = Ts_Scope::iterator;
  using ts_const_iterator = Ts_Scope::const_iterator;

  Scope(const Name &name, const Symbol *owner);
  virtual ~Scope() = default;

  virtual void define(const Symbol *s, const TypeSymbol *ts);
  virtual const SymbolData &s_resolve(const Name &name) const;
  virtual const TypeSymbolData &ts_resolve(const Name &name) const;
  virtual int resolveContains(const Name &name) const;
  virtual int s_resolveContains(const Name &name) const;
  virtual int ts_resolveContains(const Name &name) const;

  // work only in plain level
  virtual bool contains(const Name &name) const;
  virtual bool empty() const;
  virtual int size() const;

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
  Ts_Builtin(const Name &name, const Symbol *owner);
  virtual ~Ts_Builtin() = default;
  virtual TypeSymbolCategory category() const;
  virtual std::string toString() const;
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
          const std::vector<TypeSymbol *> &params,
          const TypeSymbol *resultType);
  virtual ~Ts_Func();
  virtual TypeSymbolCategory category() const;
  virtual std::string toString() const;

  virtual std::vector<TypeSymbol *> &params();
  virtual const std::vector<TypeSymbol *> &params() const;
  virtual TypeSymbol *&result();
  virtual const TypeSymbol *result() const;

private:
  std::vector<TypeSymbol *> params_;
  TypeSymbol *result_;
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

class S_Method : public Scope {
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

class S_Local : public Scope {
public:
  S_Local(const Name &name, const Symbol *owner);
  virtual ~S_Local() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;
};

class S_Global : public Scope {
public:
  virtual ~S_Global() = default;
  virtual SymbolCategory category() const;
  virtual std::string toString() const;

  static const S_Global *global();

private:
  S_Global(const Name &name, const Symbol *owner);
};

// symbol }
