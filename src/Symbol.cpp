// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Ast.h"
#include "Location.h"
#include "Log.h"
#include "Name.h"
#include "Token.h"

namespace detail {

SymbolImpl::SymbolImpl(const Cowstr &name, const Location &location,
                       std::shared_ptr<Symbol> owner)
    : Nameable(name), Locationable(location), owner_(owner) {}

std::shared_ptr<Symbol> SymbolImpl::owner() const { return owner_; }

} // namespace detail

static std::shared_ptr<Symbol> fromImpl(Ast *ast,
                                        std::shared_ptr<Symbol> scope);

std::shared_ptr<Symbol> Symbol::from(Ast *compileUnit) {
  return fromImpl(compileUnit, std::shared_ptr<Symbol>(nullptr));
}

bool Symbol::isSymbol(std::shared_ptr<Symbol> sptr) {
  if (!sptr) {
    return false;
  }
  switch (sptr->kind()) {
  case SymbolKind::Var:
  case SymbolKind::Func:
  case SymbolKind::Param:
  case SymbolKind::Class:
  case SymbolKind::Field:
  case SymbolKind::Method:
  case SymbolKind::Local:
  case SymbolKind::Global:
    return true;
  default:
    return false;
  }
  return false;
}

bool Symbol::isTypeSymbol(std::shared_ptr<Symbol> tsptr) {
  return tsptr ? (!isSymbol(tsptr)) : false;
}

// scope {

void SymbolTable::s_define(std::shared_ptr<Symbol> s,
                           std::shared_ptr<TypeSymbol> ts) {
  LOG_ASSERT(s, "s must not null");
  LOG_ASSERT(ts, "ts must not null");
  smap_.insert(s->name(), SymbolData(s, ts));
}

void SymbolTable::ts_define(std::shared_ptr<TypeSymbol> ts) {
  LOG_ASSERT(ts, "ts must not null");
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

// symbol {

#define SYMBOL_GENERATOR(s)                                                    \
  Cowstr &s::name() { return detail::SymbolImpl::name(); }                     \
  const Cowstr &s::name() const { return detail::SymbolImpl::name(); }         \
  Location &s::location() { return detail::SymbolImpl::location(); }           \
  const Location &s::location() const {                                        \
    return detail::SymbolImpl::location();                                     \
  }                                                                            \
  std::shared_ptr<Symbol> s::owner() const {                                   \
    return detail::SymbolImpl::owner();                                        \
  }

S_Var::S_Var(const Cowstr &name, const Location &location,
             std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Var::kind() const { return SymbolKind::Var; }

SYMBOL_GENERATOR(S_Var)

S_Func::S_Func(const Cowstr &name, const Location &location,
               std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Func::kind() const { return SymbolKind::Func; }

SYMBOL_GENERATOR(S_Func)

S_Param::S_Param(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Param::kind() const { return SymbolKind::Param; }

SYMBOL_GENERATOR(S_Param)

S_Class::S_Class(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Class::kind() const { return SymbolKind::Class; }

SYMBOL_GENERATOR(S_Class)

S_Field::S_Field(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Field::kind() const { return SymbolKind::Field; }

SYMBOL_GENERATOR(S_Field)

S_Method::S_Method(const Cowstr &name, const Location &location,
                   std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Method::kind() const { return SymbolKind::Method; }

SYMBOL_GENERATOR(S_Method)

S_Local::S_Local(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind S_Local::kind() const { return SymbolKind::Local; }

SYMBOL_GENERATOR(S_Local)

S_Global::S_Global(const Cowstr &name, const Location &location)
    : detail::SymbolImpl(name, location, std::shared_ptr<Symbol>(nullptr)) {}

SymbolKind S_Global::kind() const { return SymbolKind::Global; }

SYMBOL_GENERATOR(S_Global)

// symbol }

// type symbol {

Ts_Plain::Ts_Plain(const Cowstr &name, std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, Location(), owner) {}

SymbolKind Ts_Plain::kind() const { return SymbolKind::PlainType; }

SYMBOL_GENERATOR(Ts_Plain)

Ts_Class::Ts_Class(const Cowstr &name, const Location &location,
                   std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind Ts_Class::kind() const { return SymbolKind::ClassType; }

SYMBOL_GENERATOR(Ts_Class)

Ts_Func::Ts_Func(const Cowstr &name, const Location &location,
                 std::shared_ptr<Symbol> owner)
    : detail::SymbolImpl(name, location, owner) {}

SymbolKind Ts_Func::kind() const { return SymbolKind::FuncType; }

SYMBOL_GENERATOR(Ts_Func)

// type symbol }

using sptr = std::shared_ptr<Symbol>;
using tsptr = std::shared_ptr<TypeSymbol>;

static SymbolNameGenerator nameGenerator;

static std::shared_ptr<Symbol> fromImpl(Ast *ast,
                                        std::shared_ptr<Symbol> scope) {
  if (!ast)
    return scope;
  switch (ast->kind()) {
  case AstKind::CompileUnit: {
    A_CompileUnit *e = static_cast<A_CompileUnit *>(ast);
    std::shared_ptr<S_Global> s_global(new S_Global(e->name(), e->location()));
    // integer
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("byte", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("ubyte", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("short", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("ushort", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("int", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("uint", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("long", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("ulong", s_global)));
    // float
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("float", s_global)));
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("double", s_global)));
    // char
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("char", s_global)));
    // boolean
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("boolean", s_global)));
    // void
    s_global->ts_define(
        std::shared_ptr<TypeSymbol>(new Ts_Plain("void", s_global)));
    fromImpl(e->topStats, s_global);
    return s_global;
  }
  case AstKind::TopStats: {
    A_TopStats *e = static_cast<A_TopStats *>(ast);
    fromImpl(e->topStat, scope);
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
    fromImpl(e->params, scope);
    return scope;
  }
  case AstKind::Params: {
    A_Params *e = static_cast<A_Params *>(ast);
    fromImpl(e->param, scope);
    fromImpl(e->next, scope);
    return scope;
  }
  case AstKind::Param: {
    A_Param *e = static_cast<A_Param *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    A_PlainType *plainType = static_cast<A_PlainType *>(e->type);
    sptr s_param(new S_Param(varId->literal(), e->location(), scope));
    LOG_ASSERT(scope->kind() == (+SymbolKind::Func),
               "scope->kind {} must be SymbolKind::Func",
               scope->kind()._to_string());
    std::shared_ptr<S_Func> func_scope =
        std::static_pointer_cast<S_Func>(scope);
    LOG_ASSERT(func_scope->ts_resolveContains(tokenName(plainType->token)),
               "scope {} must contains type symbol {}", func_scope->name(),
               tokenName(plainType->token));
    TypeSymbolData ts_data =
        func_scope->ts_resolve(tokenName(plainType->token));
    func_scope->s_define(s_param, ts_data.typeSymbol);
    func_scope->params.push_back(s_param);
    return scope;
  }
  case AstKind::VarDef: {
    A_VarDef *e = static_cast<A_VarDef *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    sptr s_var(new S_Var(varId->literal(), e->location(), scope));
    return scope;
  }
  case AstKind::Block: {
    A_Block *e = static_cast<A_Block *>(ast);
    sptr s_local(new S_Local(nameGenerator.from(ast), e->location(), scope));
    fromImpl(e->blockStats, s_local);
    return s_local;
  }
  case AstKind::BlockStats: {
    A_BlockStats *e = static_cast<A_BlockStats *>(ast);
    fromImpl(e->blockStat, scope);
    fromImpl(e->next, scope);
    return scope;
  }
  case AstKind::If: {
    A_If *e = static_cast<A_If *>(ast);
    fromImpl(e->condition, scope);
    fromImpl(e->thenp, scope);
    fromImpl(e->elsep, scope);
    return scope;
  }
  case AstKind::Loop: {
    A_Loop *e = static_cast<A_Loop *>(ast);
    fromImpl(e->condition, scope);
    fromImpl(e->body, scope);
    return scope;
  }
  case AstKind::LoopCondition: {
    A_LoopCondition *e = static_cast<A_LoopCondition *>(ast);
    fromImpl(e->init, scope);
    fromImpl(e->condition, scope);
    fromImpl(e->update, scope);
    return scope;
  }
  case AstKind::LoopEnumerator: {
    A_LoopEnumerator *e = static_cast<A_LoopEnumerator *>(ast);
    fromImpl(e->id, scope);
    fromImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Yield: {
    A_Yield *e = static_cast<A_Yield *>(ast);
    fromImpl(e->expr, scope);
    return scope;
  }
  case AstKind::DoWhile: {
    A_DoWhile *e = static_cast<A_DoWhile *>(ast);
    fromImpl(e->body, scope);
    fromImpl(e->condition, scope);
    return scope;
  }
  case AstKind::Try: {
    A_Try *e = static_cast<A_Try *>(ast);
    fromImpl(e->tryp, scope);
    fromImpl(e->catchp, scope);
    fromImpl(e->finallyp, scope);
    return scope;
  }
  case AstKind::Throw: {
    A_Throw *e = static_cast<A_Throw *>(ast);
    fromImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Return: {
    A_Return *e = static_cast<A_Return *>(ast);
    fromImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Assign: {
    A_Assign *e = static_cast<A_Assign *>(ast);
    fromImpl(e->assignee, scope);
    fromImpl(e->assignor, scope);
    return scope;
  }
  case AstKind::PostfixExpr: {
    A_PostfixExpr *e = static_cast<A_PostfixExpr *>(ast);
    fromImpl(e->expr, scope);
    return scope;
  }
  case AstKind::InfixExpr: {
    A_InfixExpr *e = static_cast<A_InfixExpr *>(ast);
    fromImpl(e->left, scope);
    fromImpl(e->right, scope);
    return scope;
  }
  case AstKind::PrefixExpr: {
    A_PrefixExpr *e = static_cast<A_PrefixExpr *>(ast);
    fromImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Call: {
    A_Call *e = static_cast<A_Call *>(ast);
    fromImpl(e->id, scope);
    fromImpl(e->args, scope);
    return scope;
  }
  case AstKind::Exprs: {
    A_Exprs *e = static_cast<A_Exprs *>(ast);
    fromImpl(e->expr, scope);
    fromImpl(e->next, scope);
    return scope;
  }
  default:
    // Break
    // Continue
    // VarId
    // Integer
    // Float
    // Boolean
    // Character
    // String
    // Nil
    // Void
    return scope;
  }
}
