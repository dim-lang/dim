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

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new SymbolBuilder::x();                                       \
    binder_.bind((+AstKind::x)._to_integral(), v);                             \
    visitors_.push_back(v);                                                    \
  } while (0)

static NameGenerator ScopeNameGenerator(".");

// SymbolBuilder {

// do action {

void SymbolBuilder::Loop::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_Loop *e = cast(A_Loop, ast);

  // create local scope
  Scope *loopScope =
      new S_Local(ScopeNameGenerator.generate("loop", e->location().str()),
                  e->location(), sbcontext->scope);
  cast(S_Local, loopScope)->ast() = e;
  e->localScope = loopScope;
  sbcontext->scope->subscope_define(loopScope);
}

void SymbolBuilder::LoopEnumerator::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_LoopEnumerator *e = cast(A_LoopEnumerator, ast);
  A_VarId *varId = cast(A_VarId, e->id);
  A_PlainType *varType = cast(A_PlainType, e->type);

  // get variable type symbol
  TypeSymbol *variableTypeSymbol =
      sbcontext->scope->ts_resolve(tokenName(varType->token));
  LOG_ASSERT(variableTypeSymbol, "variable type {} type symbol must exist",
             tokenName(varType->token));

  // create variable symbol
  Symbol *variableSymbol = new S_Var(varId->name(), varId->location(),
                                     variableTypeSymbol, sbcontext->scope);
  cast(S_Var, variableSymbol)->ast() = varId;
  varId->symbol = variableSymbol;
  sbcontext->scope->s_define(variableSymbol);
}

void SymbolBuilder::Block::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_Block *e = cast(A_Block, ast);

  Scope *localScope =
      new S_Local(ScopeNameGenerator.generate("local", e->location().str()),
                  e->location(), sbcontext->scope);
  cast(S_Local, localScope)->ast() = e;
  e->localScope = localScope;
  sbcontext->scope->subscope_define(localScope);
}

void SymbolBuilder::VarDef::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_VarDef *e = cast(A_VarDef, ast);
  A_VarId *varId = cast(A_VarId, e->id);
  A_PlainType *varType = cast(A_PlainType, e->type);

  // get variable type symbol
  TypeSymbol *variableTypeSymbol =
      sbcontext->scope->ts_resolve(tokenName(varType->token));
  LOG_ASSERT(variableTypeSymbol, "variable type {} type symbol must exist",
             tokenName(varType->token));

  // create variable symbol
  Symbol *variableSymbol = new S_Var(varId->name(), varId->location(),
                                     variableTypeSymbol, sbcontext->scope);
  cast(S_Var, variableSymbol)->ast() = varId;
  varId->symbol = variableSymbol;
  sbcontext->scope->s_define(variableSymbol);
}

void SymbolBuilder::Param::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_Param *e = cast(A_Param, ast);
  A_VarId *varId = cast(A_VarId, e->id);
  A_PlainType *plainType = cast(A_PlainType, e->type);

  // get parameter type symbol
  TypeSymbol *parameterTypeSymbol =
      scope()->ts_resolve(tokenName(plainType->token));
  LOG_ASSERT(parameterTypeSymbol,
             "plainType token name {} must has type symbol",
             tokenName(plainType->token));

  // create parameter symbol
  Symbol *parameterSymbol = new S_Param(varId->name(), varId->location(),
                                        parameterTypeSymbol, sbcontext->scope);
  cast(S_Param, parameterSymbol)->ast() = varId;
  varId->symbol = parameterSymbol;
  sbcontext->scope->s_define(parameterSymbol);
  LOG_ASSERT(sbcontext->scope->kind() == (+SymbolKind::Func),
             "sbcontext->scope->kind {} must be SymbolKind::Func",
             sbcontext->scope->kind()._to_string());
  cast(S_Func, sbcontext->scope)->params.push_back(parameterSymbol);
}

void SymbolBuilder::FuncDef::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_FuncDef *e = cast(A_FuncDef, ast);
  A_FuncSign *sign = cast(A_FuncSign, e->funcSign);
  A_VarId *varId = cast(A_VarId, sign->id);

  // get function parameter types and result type
  std::vector<TypeSymbol *> parameterTypeSymbols;
  for (A_Params *params = sign->params; params; params = params->next) {
    LOG_ASSERT(params->param, "params {} param must not null", params->name());
    TypeSymbol *parameterTypeSymbol = sbcontext->scope->ts_resolve(
        tokenName(cast(A_PlainType, params)->param->type)->token));
    parameterTypeSymbols.push_back(parameterTypeSymbol);
  }
  TypeSymbol *resultTypeSymbol = sbcontext->scope->ts_resolve(
      tokenName(cast(A_PlainType, e->resultType)->token));

  // create function type and symbol
  TypeSymbol *functionTypeSymbol =
      new Ts_Func(parameterTypeSymbols, resultTypeSymbol, sign->location(),
                  sbcontext->scope);
  Symbol *functionSymbol = new S_Func(varId->name(), varId->location(),
                                      functionTypeSymbol, sbcontext->scope);
  cast(S_Func, functionSymbol)->ast() = varId;
  varId->symbol = functionSymbol;
  sbcontext->scope->s_define(functionSymbol);

  // pass function symbol to subscope
  Scope *functionScope = cast(Scope, functionSymbol);
  sbcontext->scope = functionScope;
}

void SymbolBuilder::CompileUnit::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbcontext = cast(SymbolBuilder::Context, context);
  A_CompileUnit *e = cast(A_CompileUnit, ast);

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
  cast(S_Global, globalScope)->ast() = e;
  e->globalScope = globalScope;

  // pass globalScope to subscope
  sbcontext->scope = globalScope;
}

// do action }

SymbolBuilder::Context::Context() : scope(nullptr) {}

SymbolBuilder::SymbolBuilder() {
  binder_.setContext(&context_);

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

SymbolBuilder::~SymbolBuilder() { del(visitors_); }

void SymbolBuilder::run(Ast *ast) { Visitor::traverse(binder_, ast); }

// SymbolBuilder }

// SymbolReviewer {

SymbolReviewer::SymbolReviewer() {}

SymbolReviewer::~SymbolReviewer() {}

void SymbolReviewer::run(Ast *ast) {}

// SymbolReviewer }
