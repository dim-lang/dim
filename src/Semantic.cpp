// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Ast.h"
#include "Log.h"
#include "Name.h"
#include "Symbol.h"
#include "Token.h"
#include "Util.h"
#include "boost/preprocessor/cat.hpp"

#define BIND(x) binder_->bind(new SymbolBuilder::x())

#define PASSING1(x, a)                                                         \
  void SymbolBuilder::x::visit(VisitorBinder *binder, Ast *anode) {            \
    BOOST_PP_CAT(A_, x) *node = cast<BOOST_PP_CAT(A_, x)>(anode);              \
    Visitor *u = binder->get(node->a);                                         \
    cast<SymbolBuilder::BaseVisitor>(u)->scope() = scope();                    \
  }

#define PASSING2(x, a, b)                                                      \
  void SymbolBuilder::x::visit(VisitorBinder *binder, Ast *anode) {            \
    BOOST_PP_CAT(A_, x) *node = cast<BOOST_PP_CAT(A_, x)>(anode);              \
    Visitor *u = binder->get(node->a);                                         \
    Visitor *v = binder->get(node->b);                                         \
    cast<SymbolBuilder::BaseVisitor>(u)->scope() = scope();                    \
    cast<SymbolBuilder::BaseVisitor>(v)->scope() = scope();                    \
  }

#define PASSING3(x, a, b, c)                                                   \
  void SymbolBuilder::x::visit(VisitorBinder *binder, Ast *anode) {            \
    BOOST_PP_CAT(A_, x) *node = cast<BOOST_PP_CAT(A_, x)>(anode);              \
    Visitor *u = binder->get(node->a);                                         \
    Visitor *v = binder->get(node->b);                                         \
    Visitor *p = binder->get(node->c);                                         \
    cast<SymbolBuilder::BaseVisitor>(u)->scope() = scope();                    \
    cast<SymbolBuilder::BaseVisitor>(v)->scope() = scope();                    \
    cast<SymbolBuilder::BaseVisitor>(p)->scope() = scope();                    \
  }

static NameGenerator ScopeNameGenerator(".");

// passing scope {

PASSING1(Throw, expr)
PASSING1(Return, expr)
PASSING2(Assign, assignee, assignor)
PASSING1(PostfixExpr, expr)
PASSING1(PrefixExpr, expr)
PASSING2(InfixExpr, left, right)
PASSING2(Call, id, args)
PASSING2(Exprs, expr, next)
PASSING3(If, condition, thenp, elsep)
PASSING1(Yield, expr)
PASSING3(LoopCondition, init, condition, update)
PASSING2(DoWhile, body, condition)
PASSING3(Try, tryp, catchp, finallyp)
PASSING2(BlockStats, blockStat, next)
PASSING2(FuncSign, id, params)
PASSING2(Params, param, next)
PASSING2(TopStats, topStat, next)

// passing scope }

// do action {

void SymbolBuilder::Loop::visit(VisitorBinder *binder, Ast *anode) {
  A_Loop *e = cast<A_Loop>(anode);

  // create local scope
  Scope *loopScope =
      new S_Local(ScopeNameGenerator.generate("loop", e->location().str()),
                  e->location(), scope());
  cast<S_Local>(loopScope)->ast() = e;
  e->localScope = loopScope;
  scope()->subscope_define(loopScope);

  // pass to subscope
  Visitor *a = binder->get(e->condition);
  Visitor *b = binder->get(e->body);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = scope();
  cast<SymbolBuilder::BaseVisitor>(b)->scope() = scope();
}

void SymbolBuilder::LoopEnumerator::visit(VisitorBinder *binder, Ast *anode) {
  A_LoopEnumerator *e = cast<A_LoopEnumerator>(anode);
  A_VarId *varId = cast<A_VarId>(e->id);
  A_PlainType *varType = cast<A_PlainType>(e->type);

  // get variable type symbol
  TypeSymbol *variableTypeSymbol =
      scope()->ts_resolve(tokenName(varType->token));
  LOG_ASSERT(variableTypeSymbol, "variable type {} type symbol must exist",
             tokenName(varType->token));

  // create variable symbol
  Symbol *variableSymbol =
      new S_Var(varId->name(), varId->location(), variableTypeSymbol, scope());
  cast<S_Var>(variableSymbol)->ast() = varId;
  varId->symbol = variableSymbol;
  scope()->s_define(variableSymbol);

  // pass to subscope
  Visitor *a = binder->get(e->id);
  Visitor *b = binder->get(e->type);
  Visitor *c = binder->get(e->expr);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = scope();
  cast<SymbolBuilder::BaseVisitor>(b)->scope() = scope();
  cast<SymbolBuilder::BaseVisitor>(c)->scope() = scope();
}

void SymbolBuilder::Block::visit(VisitorBinder *binder, Ast *anode) {

  A_Block *e = cast<A_Block>(anode);
  Scope *localScope =
      new S_Local(ScopeNameGenerator.generate("local", e->location().str()),
                  e->location(), scope());
  cast<S_Local>(localScope)->ast() = e;
  e->localScope = localScope;
  scope()->subscope_define(localScope);

  // pass to subscope
  Visitor *a = binder->get(e->blockStats);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = scope();
}

void SymbolBuilder::VarDef::visit(VisitorBinder *binder, Ast *anode) {
  A_VarDef *e = cast<A_VarDef>(anode);
  A_VarId *varId = cast<A_VarId>(e->id);
  A_PlainType *varType = cast<A_PlainType>(e->type);

  // get variable type symbol
  TypeSymbol *variableTypeSymbol =
      scope()->ts_resolve(tokenName(varType->token));
  LOG_ASSERT(variableTypeSymbol, "variable type {} type symbol must exist",
             tokenName(varType->token));

  // create variable symbol
  Symbol *variableSymbol =
      new S_Var(varId->name(), varId->location(), variableTypeSymbol, scope());
  cast<S_Var>(variableSymbol)->ast() = varId;
  varId->symbol = variableSymbol;
  scope()->s_define(variableSymbol);

  // pass to subscope
  Visitor *a = binder->get(e->id);
  Visitor *b = binder->get(e->type);
  Visitor *c = binder->get(e->expr);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = scope();
  cast<SymbolBuilder::BaseVisitor>(b)->scope() = scope();
  cast<SymbolBuilder::BaseVisitor>(c)->scope() = scope();
}

void SymbolBuilder::Param::visit(VisitorBinder *binder, Ast *anode) {
  A_Param *e = cast<A_Param>(anode);
  A_VarId *varId = cast<A_VarId>(e->id);
  A_PlainType *plainType = cast<A_PlainType>(e->type);

  // get parameter type symbol
  TypeSymbol *parameterTypeSymbol =
      scope()->ts_resolve(tokenName(plainType->token));
  LOG_ASSERT(parameterTypeSymbol,
             "plainType token name {} must has type symbol",
             tokenName(plainType->token));

  // create parameter symbol
  Symbol *parameterSymbol = new S_Param(varId->name(), varId->location(),
                                        parameterTypeSymbol, scope());
  cast<S_Param>(parameterSymbol)->ast() = varId;
  varId->symbol = parameterSymbol;
  scope()->s_define(parameterSymbol);
  LOG_ASSERT(cast<Symbol>(scope())->kind() == (+SymbolKind::Func),
             "scope->kind {} must be SymbolKind::Func",
             cast<Symbol>(scope())->kind()._to_string());
  cast<S_Func>(scope())->params.push_back(parameterSymbol);

  // pass to subscope
  Visitor *a = binder->get(e->id);
  Visitor *b = binder->get(e->type);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = scope();
  cast<SymbolBuilder::BaseVisitor>(b)->scope() = scope();
}

void SymbolBuilder::FuncDef::visit(VisitorBinder *binder, Ast *anode) {
  A_FuncDef *e = cast<A_FuncDef>(anode);
  A_FuncSign *sign = cast<A_FuncSign>(e->funcSign);
  A_VarId *varId = cast<A_VarId>(sign->id);

  // get function parameter types and result type
  std::vector<TypeSymbol *> parameterTypeSymbols;
  for (A_Params *params = sign->params; params; params = params->next) {
    LOG_ASSERT(params->param, "params {} param must not null", params->name());
    TypeSymbol *parameterTypeSymbol = scope()->ts_resolve(
        tokenName(static_cast<A_PlainType *>(params->param->type)->token));
    parameterTypeSymbols.push_back(parameterTypeSymbol);
  }
  TypeSymbol *resultTypeSymbol = scope()->ts_resolve(
      tokenName(static_cast<A_PlainType *>(e->resultType)->token));

  // create function type and symbol
  TypeSymbol *functionTypeSymbol = new Ts_Func(
      parameterTypeSymbols, resultTypeSymbol, sign->location(), scope());
  Symbol *functionSymbol =
      new S_Func(varId->name(), varId->location(), functionTypeSymbol, scope());
  cast<S_Func>(functionSymbol)->ast() = varId;
  varId->symbol = functionSymbol;
  scope()->s_define(functionSymbol);

  // pass function symbol to subscope
  Scope *functionScope = cast<Scope>(functionSymbol);
  Visitor *a = binder->get(e->funcSign);
  Visitor *b = binder->get(e->body);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = functionScope;
  cast<SymbolBuilder::BaseVisitor>(b)->scope() = functionScope;
}

void SymbolBuilder::CompileUnit::visit(VisitorBinder *binder, Ast *anode) {
  A_CompileUnit *e = cast<A_CompileUnit>(anode);

  // create global scope
  Scope *globalScope = new S_Global("global", e->location());
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
  cast<S_Global>(globalScope)->ast() = e;
  e->globalScope = globalScope;

  // pass globalScope to subscope
  Visitor *a = binder->get(e->topStats);
  cast<SymbolBuilder::BaseVisitor>(a)->scope() = globalScope;
}

// do action }

SymbolBuilder::SymbolBuilder() : binder_(new VisitorBinder()) {
  BIND(Integer);
  BIND(Float);
  BIND(Boolean);
  BIND(Character);
  BIND(String);
  BIND(Nil);
  BIND(Void);
  BIND(VarId);
  BIND(Break);
  BIND(Continue);
  BIND(PlainType);

  BIND(Throw);
  BIND(Return);
  BIND(Assign);
  BIND(PostfixExpr);
  BIND(PrefixExpr);
  BIND(InfixExpr);
  BIND(Call);
  BIND(Exprs);
  BIND(If);
  BIND(Yield);
  BIND(LoopCondition);
  BIND(DoWhile);
  BIND(Try);
  BIND(BlockStats);
  BIND(FuncSign);
  BIND(Params);
  BIND(TopStats);

  BIND(Loop);
  BIND(LoopEnumerator);
  BIND(Block);
  BIND(VarDef);
  BIND(Param);
  BIND(FuncDef);
  BIND(CompileUnit);
}

SymbolBuilder::~SymbolBuilder() { del(binder_); }

void SymbolBuilder::run(Ast *anode) { Visitor::traverse(binder_, anode); }
