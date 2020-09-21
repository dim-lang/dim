// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Ast.h"
#include "Counter.h"
#include "Location.h"
#include "Log.h"
#include "Name.h"
#include "Token.h"
#include "container/LinkedHashMap.hpp"

Ownable::Ownable(std::shared_ptr<Scope> owner) : owner_(owner) {}

std::shared_ptr<Scope> Ownable::owner() const { return owner_; }

// scope {

static std::shared_ptr<Scope> fromImpl(Ast *ast, std::shared_ptr<Scope> scope);

std::shared_ptr<Scope> Scope::from(Ast *compileUnit) {
  return fromImpl(compileUnit, std::shared_ptr<Scope>(nullptr));
}

void Scope::s_define(std::shared_ptr<Symbol> s,
                     std::shared_ptr<TypeSymbol> ts) {
  LOG_ASSERT(s, "s must not null");
  LOG_ASSERT(ts, "ts must not null");
  LOG_ASSERT(!smap_.contains(s->name()), "symbol already exist: {}", s->name());
  smap_.insert(s->name(), SymbolData(s, ts));
}

void Scope::ts_define(std::shared_ptr<TypeSymbol> ts) {
  LOG_ASSERT(ts, "ts must not null");
  LOG_ASSERT(!tsmap_.contains(ts->name()), "type symbol already exist: {}",
             ts->name());
  tsmap_.insert(ts->name(), TypeSymbolData(ts));
}

SymbolData Scope::s_resolve(const Cowstr &name) const {
  if (smap_.contains(name)) {
    return smap_.find(name)->second;
  }
  return owner() ? owner()->s_resolve(name) : SymbolData(nullptr, nullptr);
}

TypeSymbolData Scope::ts_resolve(const Cowstr &name) const {
  if (tsmap_.contains(name)) {
    return tsmap_.find(name)->second;
  }
  return owner() ? owner()->ts_resolve(name) : TypeSymbolData(nullptr);
}

bool Scope::s_resolveContains(const Cowstr &name) const {
  if (smap_.contains(name)) {
    return true;
  }
  return owner() ? owner()->s_resolveContains(name) : false;
}

bool Scope::ts_resolveContains(const Cowstr &name) const {
  if (tsmap_.contains(name)) {
    return true;
  }
  return owner() ? owner()->ts_resolveContains(name) : false;
}

bool Scope::s_contains(const Cowstr &name) const {
  return smap_.contains(name);
}

bool Scope::ts_contains(const Cowstr &name) const {
  return tsmap_.contains(name);
}

bool Scope::s_empty() const { return smap_.empty(); }

bool Scope::ts_empty() const { return tsmap_.empty(); }

int Scope::s_size() const { return (int)smap_.size(); }

int Scope::ts_size() const { return (int)tsmap_.size(); }

Scope::s_iterator Scope::s_begin() { return smap_.begin(); }

Scope::s_const_iterator Scope::s_begin() const { return smap_.begin(); }

Scope::s_const_iterator Scope::s_cbegin() const { return smap_.cbegin(); }

Scope::s_iterator Scope::s_end() { return smap_.end(); }

Scope::s_const_iterator Scope::s_end() const { return smap_.end(); }

Scope::s_const_iterator Scope::s_cend() const { return smap_.cend(); }

Scope::ts_iterator Scope::ts_begin() { return tsmap_.begin(); }

Scope::ts_const_iterator Scope::ts_begin() const { return tsmap_.begin(); }

Scope::ts_const_iterator Scope::ts_cbegin() const { return tsmap_.cbegin(); }

Scope::ts_iterator Scope::ts_end() { return tsmap_.end(); }

Scope::ts_const_iterator Scope::ts_end() const { return tsmap_.end(); }

Scope::ts_const_iterator Scope::ts_cend() const { return tsmap_.cend(); }

// scope }

// symbol {

#define SYMBOL_GENERATOR(s)                                                    \
  Cowstr &s::name() { return Nameable::name(); }                               \
  const Cowstr &s::name() const { return Nameable::name(); }                   \
  Location &s::location() { return Locationable::location(); }                 \
  const Location &s::location() const { return Locationable::location(); }     \
  std::shared_ptr<Scope> s::owner() const { return Ownable::owner(); }

S_Var::S_Var(const Cowstr &name, const Location &location,
             std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

SymbolKind S_Var::kind() const { return SymbolKind::Var; }

SYMBOL_GENERATOR(S_Var)

S_Func::S_Func(const Cowstr &name, const Location &location,
               std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

SymbolKind S_Func::kind() const { return SymbolKind::Func; }

bool S_Func::isSymbol() const { return true; }

bool S_Func::isTypeSymbol() const { return false; }

SYMBOL_GENERATOR(S_Func)

S_Param::S_Param(const Cowstr &name, const Location &location,
                 std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

SymbolKind S_Param::kind() const { return SymbolKind::Param; }

SYMBOL_GENERATOR(S_Param)

S_Field::S_Field(const Cowstr &name, const Location &location,
                 std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

SymbolKind S_Field::kind() const { return SymbolKind::Field; }

SYMBOL_GENERATOR(S_Field)

S_Method::S_Method(const Cowstr &name, const Location &location,
                   std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

SymbolKind S_Method::kind() const { return SymbolKind::Method; }

bool S_Method::isSymbol() const { return true; }

bool S_Method::isTypeSymbol() const { return false; }

SYMBOL_GENERATOR(S_Method)

S_Local::S_Local(const Cowstr &name, const Location &location,
                 std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

SymbolKind S_Local::kind() const { return SymbolKind::Local; }

bool S_Local::isSymbol() const { return true; }

bool S_Local::isTypeSymbol() const { return false; }

SYMBOL_GENERATOR(S_Local)

S_Global::S_Global(const Cowstr &name, const Location &location)
    : Nameable(name), Locationable(location), Ownable(nullptr) {}

SymbolKind S_Global::kind() const { return SymbolKind::Global; }

bool S_Global::isSymbol() const { return true; }

bool S_Global::isTypeSymbol() const { return false; }

SYMBOL_GENERATOR(S_Global)

// symbol }

// type symbol {

Ts_Plain::Ts_Plain(const Cowstr &name, std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(Location()), Ownable(owner) {}

TypeSymbolKind Ts_Plain::kind() const { return TypeSymbolKind::Plain; }

SYMBOL_GENERATOR(Ts_Plain)

Ts_Class::Ts_Class(const Cowstr &name, const Location &location,
                   std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

TypeSymbolKind Ts_Class::kind() const { return TypeSymbolKind::Class; }

bool Ts_Class::isSymbol() const { return false; }

bool Ts_Class::isTypeSymbol() const { return true; }

SYMBOL_GENERATOR(Ts_Class)

Ts_Func::Ts_Func(const Cowstr &name, const Location &location,
                 std::shared_ptr<Scope> owner)
    : Nameable(name), Locationable(location), Ownable(owner) {}

TypeSymbolKind Ts_Func::kind() const { return TypeSymbolKind::Func; }

SYMBOL_GENERATOR(Ts_Func)

// type symbol }

using sptr = std::shared_ptr<Symbol>;
using tsptr = std::shared_ptr<TypeSymbol>;
using scptr = std::shared_ptr<Scope>;

static Cowstr nameGenerate(const Cowstr &hint, const Cowstr &delimiter = ".") {
  static Counter counter;
  std::stringstream ss;
  ss << hint << delimiter << counter.next();
  return ss.str();
}

static Cowstr generateFuncTypeName(const std::vector<tsptr> &paramTypes,
                                   tsptr resultType) {
  std::stringstream ss;
  ss << "(";
  for (int i = 0; i < (int)paramTypes.size(); i++) {
    ss << paramTypes[i]->name();
    if (i < (int)paramTypes.size() - 1) {
      ss << ",";
    }
  }
  ss << ")=>" << resultType->name();
  return ss.str();
}

static std::shared_ptr<Scope> fromImpl(Ast *ast, std::shared_ptr<Scope> scope) {
  if (!ast)
    return scope;
  switch (ast->kind()) {
  case AstKind::CompileUnit: {
    A_CompileUnit *e = static_cast<A_CompileUnit *>(ast);
    scptr s_global(new S_Global("global", e->location()));
    // integer
    s_global->ts_define(tsptr(new Ts_Plain("byte", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("ubyte", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("short", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("ushort", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("int", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("uint", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("long", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("ulong", s_global)));
    // float
    s_global->ts_define(tsptr(new Ts_Plain("float", s_global)));
    s_global->ts_define(tsptr(new Ts_Plain("double", s_global)));
    // char
    s_global->ts_define(tsptr(new Ts_Plain("char", s_global)));
    // boolean
    s_global->ts_define(tsptr(new Ts_Plain("boolean", s_global)));
    // void
    s_global->ts_define(tsptr(new Ts_Plain("void", s_global)));
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
    sptr s_func(new S_Func(varId->name(), e->location(), scope));
    std::vector<std::shared_ptr<TypeSymbol>> paramTypes;
    for (A_Params *params = sign->params; params; params = params->next) {
      LOG_ASSERT(params->param, "params->param must not null: {}",
                 params->name());
      TypeSymbolData tsdata = scope->ts_resolve(
          tokenName(static_cast<A_PlainType *>(params->param->type)->token));
      paramTypes.push_back(tsdata.typeSymbol);
    }
    TypeSymbolData tsdata = scope->ts_resolve(
        tokenName(static_cast<A_PlainType *>(e->resultType)->token));
    tsptr resultType = tsdata.typeSymbol;
    tsptr ts_func(new Ts_Func(generateFuncTypeName(paramTypes, resultType),
                              e->location(), scope));
    std::dynamic_pointer_cast<Ts_Func>(ts_func)->params = paramTypes;
    std::dynamic_pointer_cast<Ts_Func>(ts_func)->result = resultType;
    scope->s_define(s_func, ts_func);
    scptr s_func_scope = std::dynamic_pointer_cast<Scope>(s_func);
    fromImpl(sign, s_func_scope);
    fromImpl(e->body, s_func_scope);
    return s_func_scope;
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
    sptr s_param(new S_Param(varId->name(), e->location(), scope));
    LOG_ASSERT(scope->isSymbol(), "scope must be symbol");
    LOG_ASSERT(std::dynamic_pointer_cast<Symbol>(scope)->kind() ==
                   (+SymbolKind::Func),
               "scope->kind {} must be SymbolKind::Func",
               std::dynamic_pointer_cast<Symbol>(scope)->kind()._to_string());
    LOG_ASSERT(scope->ts_resolveContains(tokenName(plainType->token)),
               "scope {} must contains type symbol {}",
               std::dynamic_pointer_cast<Symbol>(scope)->name(),
               tokenName(plainType->token));
    TypeSymbolData ts_data = scope->ts_resolve(tokenName(plainType->token));
    scope->s_define(s_param, ts_data.typeSymbol);
    std::dynamic_pointer_cast<S_Func>(scope)->params.push_back(s_param);
    return scope;
  }
  case AstKind::VarDef: {
    A_VarDef *e = static_cast<A_VarDef *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    A_PlainType *varType = static_cast<A_PlainType *>(e->type);
    sptr s_var(new S_Var(varId->name(), e->location(), scope));
    TypeSymbolData tsdata = scope->ts_resolve(tokenName(varType->token));
    tsptr ts_var = tsdata.typeSymbol;
    scope->s_define(s_var, ts_var);
    return scope;
  }
  case AstKind::Block: {
    A_Block *e = static_cast<A_Block *>(ast);
    sptr s_local(new S_Local(nameGenerate("local"), e->location(), scope));
    TypeSymbolData tsdata = scope->ts_resolve("void");
    tsptr ts_local = tsdata.typeSymbol;
    scope->s_define(s_local, ts_local);
    scptr s_local_scope = std::dynamic_pointer_cast<Scope>(s_local);
    fromImpl(e->blockStats, s_local_scope);
    return s_local_scope;
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
