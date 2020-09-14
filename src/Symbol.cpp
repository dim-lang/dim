// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Ast.h"
#include "Log.h"

// symbol {

static std::shared_ptr<Symbol> fromImpl(Ast *ast,
                                        std::shared_ptr<Symbol> scope);

Symbol::Symbol(const Cowstr &name, const Location &location,
               std::shared_ptr<Symbol> owner)
    : Nameable(name), Locationable(location), owner_(owner) {}

std::shared_ptr<Symbol> Symbol::owner() const { return owner_; }

std::shared_ptr<Symbol> Symbol::from(Ast *compileUnit) {
  return fromImpl(compileUnit, std::shared_ptr<Symbol>(nullptr));
}

// symbol }

// type symbol {

TypeSymbol::TypeSymbol(const Cowstr &name, const Location &location,
                       std::shared_ptr<Symbol> owner)
    : Nameable(name), Locationable(location), owner_(owner) {}

std::shared_ptr<Symbol> TypeSymbol::owner() const { return owner_; }

// type symbol }

// scope {

SymbolTable::SymbolTable(const Cowstr &name, const Location &location,
                         std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

void SymbolTable::define(std::shared_ptr<Symbol> s,
                         std::shared_ptr<TypeSymbol> ts) {
  LOG_ASSERT(s, "s must not null");
  LOG_ASSERT(ts, "ts must not null");
  smap_.insert(s->name(), SymbolData(s, ts));
  tsmap_.insert(ts->name(), TypeSymbolData(ts));
}

SymbolData SymbolTable::s_resolve(const Cowstr &name) const {
  if (smap_.contains(name)) {
    return smap_.find(name)->second;
  }
  return owner()
             ? std::static_pointer_cast<SymbolTable>(owner())->s_resolve(name)
             : SymbolData(nullptr, nullptr);
}

TypeSymbolData SymbolTable::ts_resolve(const Cowstr &name) const {
  if (tsmap_.contains(name)) {
    return tsmap_.find(name)->second;
  }
  return owner()
             ? std::static_pointer_cast<SymbolTable>(owner())->ts_resolve(name)
             : TypeSymbolData(nullptr);
}

bool SymbolTable::s_resolveContains(const Cowstr &name) const {
  if (smap_.contains(name)) {
    return true;
  }
  return owner() ? std::static_pointer_cast<SymbolTable>(owner())
                       ->s_resolveContains(name)
                 : false;
}

bool SymbolTable::ts_resolveContains(const Cowstr &name) const {
  if (tsmap_.contains(name)) {
    return true;
  }
  return owner() ? std::static_pointer_cast<SymbolTable>(owner())
                       ->s_resolveContains(name)
                 : false;
}

bool SymbolTable::s_contains(const Cowstr &name) const {
  return smap_.contains(name);
}

bool SymbolTable::ts_contains(const Cowstr &name) const {
  return tsmap_.contains(name);
}

bool SymbolTable::s_empty() const { return smap_.empty(); }

bool SymbolTable::ts_empty() const { return tsmap_.empty(); }

int SymbolTable::s_size() const { return (int)smap_.size(); }

int SymbolTable::ts_size() const { return (int)tsmap_.size(); }

SymbolTable::s_iterator SymbolTable::s_begin() { return smap_.begin(); }

SymbolTable::s_const_iterator SymbolTable::s_begin() const {
  return smap_.begin();
}

SymbolTable::s_const_iterator SymbolTable::s_cbegin() const {
  return smap_.cbegin();
}

SymbolTable::s_iterator SymbolTable::s_end() { return smap_.end(); }

SymbolTable::s_const_iterator SymbolTable::s_end() const { return smap_.end(); }

SymbolTable::s_const_iterator SymbolTable::s_cend() const {
  return smap_.cend();
}

SymbolTable::ts_iterator SymbolTable::ts_begin() { return tsmap_.begin(); }

SymbolTable::ts_const_iterator SymbolTable::ts_begin() const {
  return tsmap_.begin();
}

SymbolTable::ts_const_iterator SymbolTable::ts_cbegin() const {
  return tsmap_.cbegin();
}

SymbolTable::ts_iterator SymbolTable::ts_end() { return tsmap_.end(); }

SymbolTable::ts_const_iterator SymbolTable::ts_end() const {
  return tsmap_.end();
}

SymbolTable::ts_const_iterator SymbolTable::ts_cend() const {
  return tsmap_.cend();
}

// scope }

S_Var::S_Var(const Cowstr &name, const Location &location,
             std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

SymbolKind S_Var::kind() const { return SymbolKind::Var; }

S_Func::S_Func(const Cowstr &name, const Location &location,
               std::shared_ptr<Symbol> owner)
    : SymbolTable(name, location, owner) {}

SymbolKind S_Func::kind() const { return SymbolKind::Func; }

S_Param::S_Param(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

SymbolKind S_Param::kind() const { return SymbolKind::Param; }

S_Class::S_Class(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : SymbolTable(name, location, owner) {}

SymbolKind S_Class::kind() const { return SymbolKind::Class; }

S_Field::S_Field(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : Symbol(name, location, owner) {}

SymbolKind S_Field::kind() const { return SymbolKind::Field; }

S_Method::S_Method(const Cowstr &name, const Location &location,
                   std::shared_ptr<Symbol> owner)
    : SymbolTable(name, location, owner) {}

SymbolKind S_Method::kind() const { return SymbolKind::Method; }

S_Local::S_Local(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : SymbolTable(name, location, owner) {}

SymbolKind S_Local::kind() const { return SymbolKind::Local; }

S_Global::S_Global(const Cowstr &name, const Location &location)
    : SymbolTable(name, location, std::shared_ptr<Symbol>(nullptr)) {}

SymbolKind S_Global::kind() const { return SymbolKind::Global; }

Ts_Builtin::Ts_Builtin(const Cowstr &name, const Location &location,
                       std::shared_ptr<Symbol> owner)
    : TypeSymbol(name, location, owner) {}

TypeSymbolKind Ts_Builtin::kind() const { return TypeSymbolKind::Builtin; }

Ts_Class::Ts_Class(const Cowstr &name, const Location &location,
                   std::shared_ptr<Symbol> owner,
                   const std::vector<std::shared_ptr<Symbol>> &a_fields,
                   const std::vector<std::shared_ptr<Symbol>> &a_methods)
    : TypeSymbol(name, location, owner), fields(a_fields), methods(a_methods) {}

TypeSymbolKind Ts_Class::kind() const { return TypeSymbolKind::Class; }

Ts_Func::Ts_Func(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner,
                 const std::vector<std::shared_ptr<TypeSymbol>> &a_params,
                 std::shared_ptr<TypeSymbol> a_result)
    : TypeSymbol(name, location, owner), params(a_params), result(a_result) {}

TypeSymbolKind Ts_Func::kind() const { return TypeSymbolKind::Func; }

using sptr = std::shared_ptr<Symbol>;
using tsptr = std::shared_ptr<TypeSymbol>;

static std::shared_ptr<Symbol> fromImpl(Ast *ast,
                                        std::shared_ptr<Symbol> scope) {
  if (!ast)
    return scope;
  switch (ast->kind()) {
  case AstKind::CompileUnit: {
    A_CompileUnit *e = static_cast<A_CompileUnit *>(ast);
    sptr global = sptr(new S_Global(e->name(), e->location()));
    fromImpl(e->topStats, global);
    return global;
  }
  case AstKind::TopStats: {
    A_TopStats *e = static_cast<A_TopStats *>(ast);
    if (e->topStat)
      fromImpl(e->topStat, scope);
    if (e->next)
      fromImpl(e->next, scope);
    return scope;
  }
  case AstKind::FuncDef: {
    A_FuncDef *e = static_cast<A_FuncDef *>(ast);
    A_FuncSign *sign = static_cast<A_FuncSign *>(e->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);
    sptr s_func(new S_Func(varId->literal(), e->location(), scope));
    fromImpl(sign, s_func);
    fromImpl(e->resultType, s_func);
    fromImpl(e->body, s_func);
    return s_func;
  }
  case AstKind::FuncSign: {
    A_FuncSign *e = static_cast<A_FuncSign *>(ast);
    if (e->params)
      fromImpl(e->params, scope);
    return scope;
  }
  case AstKind::Params: {
    A_Params *e = static_cast<A_Params *>(ast);
    if (e->param)
      fromImpl(e->param, scope);
    if (e->next)
      fromImpl(e->next, scope);
    return scope;
  }
  case AstKind::Param: {
    A_Param *e = static_cast<A_Param *>(ast);
    fromImpl(e->id, scope);
    fromImpl(e->type, scope);
    return scope;
  }
  case AstKind::VarDef: {
    A_VarDef *e = static_cast<A_VarDef *>(ast);
    fromImpl(e->id, scope);
    fromImpl(e->type, scope);
    fromImpl(e->expr, scope);
  }
  default:
    LOG_ASSERT(false, "invalid ast:{} {}", ast->name(),
               ast->kind()._to_string());
  }
}
