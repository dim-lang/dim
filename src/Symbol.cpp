// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "Ast.h"
#include "Counter.h"
#include "LinkedHashMap.hpp"
#include "Location.h"
#include "Log.h"
#include "Name.h"
#include "Token.h"
#include <algorithm>
#include <cctype>

#define SYMBOL_CONSTRUCTOR                                                     \
  NameableImpl(name), LocationableImpl(location), detail::TypeableImpl(type),  \
      detail::OwnableImpl(owner)

#define TYPE_SYMBOL_CONSTRUCTOR                                                \
  NameableImpl(name), LocationableImpl(location), detail::OwnableImpl(owner)

namespace detail {

// OwnableImpl {

OwnableImpl::OwnableImpl(std::shared_ptr<Scope> ownableImpl)
    : ownableImpl_(ownableImpl) {}

std::shared_ptr<Scope> &OwnableImpl::owner() { return ownableImpl_; }

std::shared_ptr<Scope> OwnableImpl::owner() const { return ownableImpl_; }

// OwnableImpl }

// TypeableImpl {

TypeableImpl ::TypeableImpl(std::shared_ptr<TypeSymbol> typeableImpl)
    : typeableImpl_(typeableImpl) {}

std::shared_ptr<TypeSymbol> &TypeableImpl::type() { return typeableImpl_; }

std::shared_ptr<TypeSymbol> TypeableImpl::type() const { return typeableImpl_; }

// TypeableImpl }

// ScopeImpl {

// symbol api {

void ScopeImpl::s_define(std::shared_ptr<Symbol> symbol) {
  LOG_ASSERT(symbol, "symbol must not null");
  LOG_ASSERT(s_data_.find(symbol->name()) == s_data_.end(),
             "symbol {} already exist", symbol->name());
  s_data_.insert(symbol->name(), symbol);
}

std::shared_ptr<Symbol> ScopeImpl::s_resolve(const Cowstr &name) const {
  s_const_iterator it = s_data_.find(name);
  if (it != s_data_.end()) {
    return it->second;
  }
  return owner() ? owner()->s_resolve(name) : nullptr;
}

bool ScopeImpl::s_contains(const Cowstr &name) const {
  return s_data_.find(name) != s_data_.end();
}

bool ScopeImpl::s_empty() const { return s_data_.empty(); }

int ScopeImpl::s_size() const { return s_data_.size(); }

Scope::s_iterator ScopeImpl::s_begin() { return s_data_.begin(); }

Scope::s_const_iterator ScopeImpl::s_begin() const { return s_data_.begin(); }

Scope::s_const_iterator ScopeImpl::s_cbegin() const { return s_data_.cbegin(); }

Scope::s_iterator ScopeImpl::s_end() { return s_data_.end(); }

Scope::s_const_iterator ScopeImpl::s_end() const { return s_data_.end(); }

Scope::s_const_iterator ScopeImpl::s_cend() const { return s_data_.end(); }

// symbol api }

// type symbol api {

void ScopeImpl::ts_define(std::shared_ptr<TypeSymbol> symbol) {
  LOG_ASSERT(symbol, "symbol must not null");
  LOG_ASSERT(ts_data_.find(symbol->name()) == ts_data_.end(),
             "symbol {} already exist", symbol->name());
  ts_data_.insert(symbol->name(), symbol);
}

std::shared_ptr<TypeSymbol> ScopeImpl::ts_resolve(const Cowstr &name) const {
  ts_const_iterator it = ts_data_.find(name);
  if (it != ts_data_.end()) {
    return it->second;
  }
  return owner() ? owner()->ts_resolve(name) : nullptr;
}

bool ScopeImpl::ts_contains(const Cowstr &name) const {
  return ts_data_.find(name) != ts_data_.end();
}

bool ScopeImpl::ts_empty() const { return ts_data_.empty(); }

int ScopeImpl::ts_size() const { return ts_data_.size(); }

Scope::ts_iterator ScopeImpl::ts_begin() { return ts_data_.begin(); }

Scope::ts_const_iterator ScopeImpl::ts_begin() const {
  return ts_data_.begin();
}

Scope::ts_const_iterator ScopeImpl::ts_cbegin() const {
  return ts_data_.cbegin();
}

Scope::ts_iterator ScopeImpl::ts_end() { return ts_data_.end(); }

Scope::ts_const_iterator ScopeImpl::ts_end() const { return ts_data_.end(); }

Scope::ts_const_iterator ScopeImpl::ts_cend() const { return ts_data_.cend(); }

// type symbol api }

// sub scope api {

void ScopeImpl::subscope_define(std::shared_ptr<Scope> scope) {
  LOG_ASSERT(scope, "scope must not null");
  LOG_ASSERT(!sc_data_.contains(scope->name()), "scope already exist: {}",
             scope->name());
  sc_data_.insert(scope->name(), scope);
}

std::shared_ptr<Scope> ScopeImpl::subscope_resolve(const Cowstr &name) const {
  auto it = sc_data_.find(name);
  return it == sc_data_.end() ? nullptr : it->second;
}

bool ScopeImpl::subscope_contains(const Cowstr &name) const {
  return sc_data_.find(name) != sc_data_.end();
}

bool ScopeImpl::subscope_empty() const { return sc_data_.empty(); }

int ScopeImpl::subscope_size() const { return sc_data_.size(); }

Scope::sc_iterator ScopeImpl::subscope_begin() { return sc_data_.begin(); }

Scope::sc_const_iterator ScopeImpl::subscope_begin() const {
  return sc_data_.begin();
}

Scope::sc_const_iterator ScopeImpl::subscope_cbegin() const {
  return sc_data_.cbegin();
}

Scope::sc_iterator ScopeImpl::subscope_end() { return sc_data_.end(); }

Scope::sc_const_iterator ScopeImpl::subscope_end() const {
  return sc_data_.end();
}

Scope::sc_const_iterator ScopeImpl::subscope_cend() const {
  return sc_data_.cend();
}

// sub scope api }

// ScopeImpl }

} // namespace detail

// TypeSymbol {

std::shared_ptr<TypeSymbol> TypeSymbol::ts_byte() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("byte"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_ubyte() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("ubyte"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_short() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("short"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_ushort() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("ushort"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_int() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("int"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_uint() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("uint"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_long() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("long"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_ulong() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("ulong"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_float() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("float"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_double() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("double"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_char() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("char"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_boolean() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("boolean"));
  return ts;
}

std::shared_ptr<TypeSymbol> TypeSymbol::ts_void() {
  static std::shared_ptr<TypeSymbol> ts(new Ts_Plain("void"));
  return ts;
}

// TypeSymbol {

// scope {

static std::shared_ptr<Scope> createImpl(Ast *ast,
                                         std::shared_ptr<Scope> scope);

static void checkImpl(Ast *ast, std::shared_ptr<Scope> scope);

void Scope::create(Ast *compileUnit) {
  createImpl(compileUnit, nullptr);
  checkImpl(compileUnit, nullptr);
}

// symbol {

S_Var::S_Var(const Cowstr &name, const Location &location,
             std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Var::kind() const { return SymbolKind::Var; }

S_Func::S_Func(const Cowstr &name, const Location &location,
               std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Func::kind() const { return SymbolKind::Func; }

S_Param::S_Param(const Cowstr &name, const Location &location,
                 std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Param::kind() const { return SymbolKind::Param; }

S_Field::S_Field(const Cowstr &name, const Location &location,
                 std::shared_ptr<TypeSymbol> type, std::shared_ptr<Scope> owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Field::kind() const { return SymbolKind::Field; }

S_Method::S_Method(const Cowstr &name, const Location &location,
                   std::shared_ptr<TypeSymbol> type,
                   std::shared_ptr<Scope> owner)
    : SYMBOL_CONSTRUCTOR {}

SymbolKind S_Method::kind() const { return SymbolKind::Method; }

// symbol }

// type symbol {

Ts_Plain::Ts_Plain(const Cowstr &name)
    : NameableImpl(name),
      LocationableImpl(Location(Position(-1, -1), Position(-1, -1))),
      detail::OwnableImpl(nullptr) {}

TypeSymbolKind Ts_Plain::kind() const { return TypeSymbolKind::Plain; }

Ts_Class::Ts_Class(const Cowstr &name, const Location &location,
                   std::shared_ptr<Scope> owner)
    : TYPE_SYMBOL_CONSTRUCTOR {}

TypeSymbolKind Ts_Class::kind() const { return TypeSymbolKind::Class; }

Ts_Func::Ts_Func(const std::vector<std::shared_ptr<TypeSymbol>> a_params,
                 std::shared_ptr<TypeSymbol> a_result, const Location &location,
                 std::shared_ptr<Scope> owner)
    : NameableImpl(generateName(a_params, a_result)),
      LocationableImpl(location), detail::OwnableImpl(owner), params(a_params),
      result(a_result) {}

TypeSymbolKind Ts_Func::kind() const { return TypeSymbolKind::Func; }

Cowstr
Ts_Func::generateName(const std::vector<std::shared_ptr<TypeSymbol>> &params,
                      std::shared_ptr<TypeSymbol> result) {
  std::stringstream ss;
  ss << "(";
  for (int i = 0; i < (int)params.size(); i++) {
    ss << params[i]->name();
    if (i < (int)params.size() - 1) {
      ss << ",";
    }
  }
  ss << ")=>" << result->name();
  return ss.str();
}

// type symbol }

// scope {

S_Local::S_Local(const Cowstr &name, const Location &location,
                 std::shared_ptr<Scope> owner)
    : TYPE_SYMBOL_CONSTRUCTOR {}

ScopeKind S_Local::kind() const { return ScopeKind::Local; }

S_Global::S_Global(const Cowstr &name, const Location &location)
    : NameableImpl(name),
      LocationableImpl(location), detail::OwnableImpl(nullptr) {}

ScopeKind S_Global::kind() const { return ScopeKind::Global; }

// scope }

using sptr = std::shared_ptr<Symbol>;
using tsptr = std::shared_ptr<TypeSymbol>;
using scptr = std::shared_ptr<Scope>;

static NameGenerator ScopeNameGenerator(".");

static std::shared_ptr<Scope> createImpl(Ast *ast,
                                         std::shared_ptr<Scope> scope) {
  if (!ast)
    return scope;
  switch (ast->kind()) {
  case AstKind::CompileUnit: {
    A_CompileUnit *e = static_cast<A_CompileUnit *>(ast);
    scptr globalScope(new S_Global("global", e->location()));
    globalScope->ts_define(TypeSymbol::ts_byte());
    globalScope->ts_define(TypeSymbol::ts_ubyte());
    globalScope->ts_define(TypeSymbol::ts_short());
    globalScope->ts_define(TypeSymbol::ts_ushort());
    globalScope->ts_define(TypeSymbol::ts_int());
    globalScope->ts_define(TypeSymbol::ts_uint());
    globalScope->ts_define(TypeSymbol::ts_long());
    globalScope->ts_define(TypeSymbol::ts_ulong());
    globalScope->ts_define(TypeSymbol::ts_float());
    globalScope->ts_define(TypeSymbol::ts_double());
    globalScope->ts_define(TypeSymbol::ts_char());
    globalScope->ts_define(TypeSymbol::ts_boolean());
    globalScope->ts_define(TypeSymbol::ts_void());
    std::static_pointer_cast<S_Global>(globalScope)->ast() = e;
    e->globalScope = globalScope;
    createImpl(e->topStats, globalScope);
    return globalScope;
  }
  case AstKind::TopStats: {
    A_TopStats *e = static_cast<A_TopStats *>(ast);
    createImpl(e->topStat, scope);
    createImpl(e->next, scope);
    return scope;
  }
  case AstKind::FuncDef: {
    A_FuncDef *e = static_cast<A_FuncDef *>(ast);
    A_FuncSign *sign = static_cast<A_FuncSign *>(e->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);
    // get function parameter type symbols
    std::vector<std::shared_ptr<TypeSymbol>> parameterTypeSymbols;
    for (A_Params *params = sign->params; params; params = params->next) {
      LOG_ASSERT(params->param, "params {} param must not null",
                 params->name());
      tsptr parameterTypeSymbol = scope->ts_resolve(
          tokenName(static_cast<A_PlainType *>(params->param->type)->token));
      parameterTypeSymbols.push_back(parameterTypeSymbol);
    }
    // get function result type symbol
    tsptr resultTypeSymbol = scope->ts_resolve(
        tokenName(static_cast<A_PlainType *>(e->resultType)->token));
    // create function type symbol
    tsptr functionTypeSymbol(new Ts_Func(parameterTypeSymbols, resultTypeSymbol,
                                         sign->location(), scope));
    // create function symbol
    sptr functionSymbol(new S_Func(varId->name(), varId->location(),
                                   functionTypeSymbol, scope));
    std::static_pointer_cast<S_Func>(functionSymbol)->ast() = varId;
    varId->symbol = functionSymbol;
    scope->s_define(functionSymbol);
    // use function symbol as enclosing scope
    scptr functionScope = std::dynamic_pointer_cast<Scope>(functionSymbol);
    createImpl(sign, functionScope);
    createImpl(e->body, functionScope);
    return functionScope;
  }
  case AstKind::FuncSign: {
    A_FuncSign *e = static_cast<A_FuncSign *>(ast);
    createImpl(e->params, scope);
    return scope;
  }
  case AstKind::Params: {
    A_Params *e = static_cast<A_Params *>(ast);
    createImpl(e->param, scope);
    createImpl(e->next, scope);
    return scope;
  }
  case AstKind::Param: {
    A_Param *e = static_cast<A_Param *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    A_PlainType *plainType = static_cast<A_PlainType *>(e->type);
    // get parameter type symbol
    tsptr parameterTypeSymbol = scope->ts_resolve(tokenName(plainType->token));
    LOG_ASSERT(parameterTypeSymbol,
               "plainType token name {} must has type symbol",
               tokenName(plainType->token));
    // create parameter symbol
    sptr parameterSymbol(new S_Param(varId->name(), varId->location(),
                                     parameterTypeSymbol, scope));
    std::static_pointer_cast<S_Param>(parameterSymbol)->ast() = varId;
    varId->symbol = parameterSymbol;
    scope->s_define(parameterSymbol);
    LOG_ASSERT(std::dynamic_pointer_cast<Symbol>(scope)->kind() ==
                   (+SymbolKind::Func),
               "scope->kind {} must be SymbolKind::Func",
               std::dynamic_pointer_cast<Symbol>(scope)->kind()._to_string());
    std::dynamic_pointer_cast<S_Func>(scope)->params.push_back(s_param);
    return scope;
  }
  case AstKind::VarDef: {
    A_VarDef *e = static_cast<A_VarDef *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    A_PlainType *varType = static_cast<A_PlainType *>(e->type);
    // get variable type symbol
    tsptr variableTypeSymbol = scope->ts_resolve(tokenName(varType->token));
    LOG_ASSERT(variableTypeSymbol, "variable type {} type symbol must exist",
               tokenName(varType->token));
    // create variable symbol
    sptr variableSymbol(
        new S_Var(varId->name(), varId->location(), variableTypeSymbol, scope));
    std::static_pointer_cast<S_Var>(variableSymbol)->ast() = varId;
    varId->symbol = variableSymbol;
    scope->s_define(variableSymbol);
    return scope;
  }
  case AstKind::Block: {
    A_Block *e = static_cast<A_Block *>(ast);
    scptr localScope(
        new S_Local(ScopeNameGenerator.generate("local", e->location().str()),
                    e->location(), scope));
    std::static_pointer_cast<S_Local>(localScope)->ast() = e;
    e->localScope = localScope;
    scope->subscope_define(localScope);
    // use local scope as new enclosing scope
    createImpl(e->blockStats, localScope);
    return localScope;
  }
  case AstKind::BlockStats: {
    A_BlockStats *e = static_cast<A_BlockStats *>(ast);
    createImpl(e->blockStat, scope);
    createImpl(e->next, scope);
    return scope;
  }
  case AstKind::If: {
    A_If *e = static_cast<A_If *>(ast);
    createImpl(e->condition, scope);
    createImpl(e->thenp, scope);
    createImpl(e->elsep, scope);
    return scope;
  }
  case AstKind::Loop: {
    A_Loop *e = static_cast<A_Loop *>(ast);
    // create local scope
    scptr loopScope(
        new S_Local(ScopeNameGenerator.generate("loop", e->location().str()),
                    e->location(), scope));
    std::static_pointer_cast<S_Local>(loopScope)->ast() = e;
    e->localScope = loopScope;
    scope->subscope_define(loopScope);
    // use local scope as new enclosing scope
    createImpl(e->condition, loopScope);
    createImpl(e->body, loopScope);
    return loopScope;
  }
  case AstKind::LoopCondition: {
    A_LoopCondition *e = static_cast<A_LoopCondition *>(ast);
    createImpl(e->init, scope);
    createImpl(e->condition, scope);
    createImpl(e->update, scope);
    return scope;
  }
  case AstKind::LoopEnumerator: {
    A_LoopEnumerator *e = static_cast<A_LoopEnumerator *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    A_PlainType *varType = static_cast<A_PlainType *>(e->type);
    // get variable type symbol
    tsptr variableTypeSymbol = scope->ts_resolve(tokenName(varType->token));
    LOG_ASSERT(variableTypeSymbol, "variable type {} type symbol must exist",
               tokenName(varType->token));
    // create variable symbol
    sptr variableSymbol(
        new S_Var(varId->name(), varId->location(), variableTypeSymbol, scope));
    std::static_pointer_cast<S_Var>(variableSymbol)->ast() = varId;
    varId->symbol = variableSymbol;
    scope->s_define(variableSymbol);
    // process other parts recursively
    createImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Yield: {
    A_Yield *e = static_cast<A_Yield *>(ast);
    createImpl(e->expr, scope);
    return scope;
  }
  case AstKind::DoWhile: {
    A_DoWhile *e = static_cast<A_DoWhile *>(ast);
    // don't create local scope in do-while
    createImpl(e->body, scope);
    createImpl(e->condition, scope);
    return scope;
  }
  case AstKind::Try: {
    A_Try *e = static_cast<A_Try *>(ast);
    createImpl(e->tryp, scope);
    createImpl(e->catchp, scope);
    createImpl(e->finallyp, scope);
    return scope;
  }
  case AstKind::Throw: {
    A_Throw *e = static_cast<A_Throw *>(ast);
    createImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Return: {
    A_Return *e = static_cast<A_Return *>(ast);
    createImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Assign: {
    A_Assign *e = static_cast<A_Assign *>(ast);
    createImpl(e->assignee, scope);
    createImpl(e->assignor, scope);
    return scope;
  }
  case AstKind::PostfixExpr: {
    A_PostfixExpr *e = static_cast<A_PostfixExpr *>(ast);
    createImpl(e->expr, scope);
    return scope;
  }
  case AstKind::InfixExpr: {
    A_InfixExpr *e = static_cast<A_InfixExpr *>(ast);
    createImpl(e->left, scope);
    createImpl(e->right, scope);
    return scope;
  }
  case AstKind::PrefixExpr: {
    A_PrefixExpr *e = static_cast<A_PrefixExpr *>(ast);
    createImpl(e->expr, scope);
    return scope;
  }
  case AstKind::Call: {
    A_Call *e = static_cast<A_Call *>(ast);
    createImpl(e->id, scope);
    createImpl(e->args, scope);
    return scope;
  }
  case AstKind::Exprs: {
    A_Exprs *e = static_cast<A_Exprs *>(ast);
    createImpl(e->expr, scope);
    createImpl(e->next, scope);
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

static void checkImpl(Ast *ast, std::shared_ptr<Scope> scope) {
  if (!ast)
    return;
  switch (ast->kind()) {
  case AstKind::CompileUnit: {
    A_CompileUnit *e = static_cast<A_CompileUnit *>(ast);
    checkImpl(e->topStats, e->globalScope);
    break;
  }
  case AstKind::TopStats: {
    A_TopStats *e = static_cast<A_TopStats *>(ast);
    checkImpl(e->topStat, scope);
    checkImpl(e->next, scope);
    break;
  }
  case AstKind::FuncDef: {
    A_FuncDef *e = static_cast<A_FuncDef *>(ast);
    A_FuncSign *sign = static_cast<A_FuncSign *>(e->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);
    LOG_ASSERT(varId->symbol, "function {} symbol must not null",
               varId->name());
    LOG_ASSERT(varId->symbol->kind() == (+SymbolKind::Func),
               "function->symbol->kind {} == SymbolKind::Func",
               varId->symbol->kind()._to_string());
    scptr functionScope = std::static_pointer_cast<Scope>(varId->symbol);
    createImpl(e->funcSign, functionScope);
    createImpl(e->body, functionScope);
    break;
  }
  case AstKind::FuncSign: {
    A_FuncSign *e = static_cast<A_FuncSign *>(ast);
    createImpl(e->params, scope);
    break;
  }
  case AstKind::Block: {
    A_Block *e = static_cast<A_Block *>(ast);
    scptr localScope = e->localScope;
    // use local scope as new enclosing scope
    createImpl(e->blockStats, localScope);
    break;
  }
  case AstKind::BlockStats: {
    A_BlockStats *e = static_cast<A_BlockStats *>(ast);
    createImpl(e->blockStat, scope);
    createImpl(e->next, scope);
    break;
  }
  case AstKind::If: {
    A_If *e = static_cast<A_If *>(ast);
    createImpl(e->condition, scope);
    createImpl(e->thenp, scope);
    createImpl(e->elsep, scope);
    break;
  }
  case AstKind::Loop: {
    A_Loop *e = static_cast<A_Loop *>(ast);
    scptr loopScope = e->localScope;
    // use local scope as new enclosing scope
    createImpl(e->condition, loopScope);
    createImpl(e->body, loopScope);
    break;
  }
  case AstKind::LoopCondition: {
    A_LoopCondition *e = static_cast<A_LoopCondition *>(ast);
    createImpl(e->init, scope);
    createImpl(e->condition, scope);
    createImpl(e->update, scope);
    break;
  }
  case AstKind::LoopEnumerator: {
    A_LoopEnumerator *e = static_cast<A_LoopEnumerator *>(ast);
    createImpl(e->expr, scope);
    break;
  }
  case AstKind::Yield: {
    A_Yield *e = static_cast<A_Yield *>(ast);
    createImpl(e->expr, scope);
    break;
  }
  case AstKind::DoWhile: {
    A_DoWhile *e = static_cast<A_DoWhile *>(ast);
    createImpl(e->body, scope);
    createImpl(e->condition, scope);
    break;
  }
  case AstKind::Try: {
    A_Try *e = static_cast<A_Try *>(ast);
    createImpl(e->tryp, scope);
    createImpl(e->catchp, scope);
    createImpl(e->finallyp, scope);
    break;
  }
  case AstKind::Throw: {
    A_Throw *e = static_cast<A_Throw *>(ast);
    createImpl(e->expr, scope);
    break;
  }
  case AstKind::Return: {
    A_Return *e = static_cast<A_Return *>(ast);
    createImpl(e->expr, scope);
    break;
  }
  case AstKind::Assign: {
    A_Assign *e = static_cast<A_Assign *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->assignee);
    sptr variableSymbol = scope->s_resolve(varId->name());
    LOG_ASSERT(variableSymbol, "assignee {} must already defined in scope {}",
               varId->name(), scope->name());
    LOG_ASSERT(variableSymbol->type(),
               "assignee {} must already defined with type {}", varId->name(),
               variableSymbol->type()->name());
    createImpl(e->assignor, scope);
    break;
  }
  case AstKind::PostfixExpr: {
    A_PostfixExpr *e = static_cast<A_PostfixExpr *>(ast);
    createImpl(e->expr, scope);
    break;
  }
  case AstKind::InfixExpr: {
    A_InfixExpr *e = static_cast<A_InfixExpr *>(ast);
    createImpl(e->left, scope);
    createImpl(e->right, scope);
    break;
  }
  case AstKind::PrefixExpr: {
    A_PrefixExpr *e = static_cast<A_PrefixExpr *>(ast);
    createImpl(e->expr, scope);
    break;
  }
  case AstKind::VarId: {
    A_VarId *varId = static_cast<A_VarId *>(ast);
    sptr variableSymbol = scope->s_resolve(varId->name());
    LOG_ASSERT(variableSymbol, "variable {} must already defined in scope {}",
               varId->name(), scope->name());
    LOG_ASSERT(variableSymbol->kind() == (+SymbolKind::Var),
               "variableSymbol->kind {} == SymbolKind::Var",
               variableSymbol->kind()._to_string());
    LOG_ASSERT(variableSymbol->type(),
               "variable {} must already defined with type {}", varId->name(),
               variableSymbol->type()->name());
    break;
  }
  case AstKind::Call: {
    A_Call *e = static_cast<A_Call *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(e->id);
    sptr functionSymbol = scope->s_resolve(varId->name());
    LOG_ASSERT(functionSymbol, "function {} must already defined in scope {}",
               varId->name(), scope->name());
    LOG_ASSERT(functionSymbol->kind() == (+SymbolKind::Func),
               "functionSymbol->kind {} == SymbolKind::Func",
               functionSymbol->kind()._to_string());
    LOG_ASSERT(functionSymbol->type(),
               "function {} must already defined with type {}", varId->name(),
               functionSymbol->type()->name());
    createImpl(e->args, scope);
    break;
  }
  case AstKind::Exprs: {
    A_Exprs *e = static_cast<A_Exprs *>(ast);
    createImpl(e->expr, scope);
    createImpl(e->next, scope);
    break;
  }
  default:
    // Params
    // Param
    // Break
    // Continue
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
