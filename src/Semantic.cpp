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

// scope name generator
static NameGenerator SNG(".");

// SymbolBuilder {

// do action {

void SymbolBuilder::Loop::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_Loop *e = cast(A_Loop, ast);

  // create local scope
  S_Local *sc_loop = new S_Local(SNG.generate("loop", e->location().str()),
                                 e->location(), sbc->scope);
  sc_loop->ast() = e;
  e->localScope = sc_loop;
  sbc->scope->subscope_define(sc_loop);

  // push loop scope down to subscope
  sbc->scope = sc_loop;
}

void SymbolBuilder::Loop::postVisit(Ast *ast, VisitorContext *context) {
  // pop loop scope back to owner scope
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  sbc->scope = sbc->scope->owner();
}

void SymbolBuilder::LoopEnumerator::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_LoopEnumerator *e = cast(A_LoopEnumerator, ast);
  A_VarId *varId = cast(A_VarId, e->id);
  A_PlainType *varType = cast(A_PlainType, e->type);

  // get variable type symbol
  TypeSymbol *ts_var = sbc->scope->ts_resolve(tokenName(varType->token));
  LOG_ASSERT(ts_var, "variable type {} is null", tokenName(varType->token));

  // create variable symbol
  S_Var *s_var =
      new S_Var(varId->name(), varId->location(), sbc->scope, ts_var);
  s_var->ast() = varId;
  varId->symbol = s_var;
  sbc->scope->s_define(s_var);
}

void SymbolBuilder::Block::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_Block *e = cast(A_Block, ast);

  // create block scope
  S_Local *sc_block = new S_Local(SNG.generate("block", e->location().str()),
                                  e->location(), sbc->scope);
  sc_block->ast() = e;
  e->localScope = sc_block;
  sbc->scope->subscope_define(sc_block);

  // push block scope down to subscope
  sbc->scope = sc_block;
}

void SymbolBuilder::Block::postVisit(Ast *ast, VisitorContext *context) {
  // pop block scope back to owner scope
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  sbc->scope = sbc->scope->owner();
}

void SymbolBuilder::VarDef::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_VarDef *e = cast(A_VarDef, ast);
  A_VarId *varId = cast(A_VarId, e->id);
  A_PlainType *varType = cast(A_PlainType, e->type);

  // get variable type symbol
  TypeSymbol *ts_var = sbc->scope->ts_resolve(tokenName(varType->token));
  LOG_ASSERT(ts_var, "variable type {} is null", tokenName(varType->token));

  // create variable symbol
  S_Var *s_var =
      new S_Var(varId->name(), varId->location(), sbc->scope, ts_var);
  s_var->ast() = varId;
  varId->symbol = s_var;
  sbc->scope->s_define(s_var);
}

void SymbolBuilder::Param::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_Param *e = cast(A_Param, ast);
  A_VarId *varId = cast(A_VarId, e->id);
  A_PlainType *plainType = cast(A_PlainType, e->type);

  // get parameter type symbol
  TypeSymbol *ts_param = scope()->ts_resolve(tokenName(plainType->token));
  LOG_ASSERT(ts_param, "plain type {} is null", tokenName(plainType->token));

  // create parameter symbol
  S_Param *s_param =
      new S_Param(varId->name(), varId->location(), sbc->scope, ts_param);
  s_param->ast() = varId;
  varId->symbol = s_param;
  sbc->scope->s_define(s_param);

  // add param to owner function scope
  LOG_ASSERT(cast(Symbol, sbc->scope)->kind() == (+SymbolKind::Func),
             "sbc->scope->kind {} != SymbolKind::Func",
             cast(Symbol, sbc->scope)->kind()._to_string());
  cast(S_Func, sbc->scope)->params.push_back(s_param);
}

void SymbolBuilder::FuncDef::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_FuncDef *e = cast(A_FuncDef, ast);
  A_FuncSign *sign = cast(A_FuncSign, e->funcSign);
  A_VarId *varId = cast(A_VarId, sign->id);

  // get function parameter types
  std::vector<TypeSymbol *> ts_params;
  for (A_Params *params = sign->params; params; params = params->next) {
    LOG_ASSERT(params->param, "params's ({}) param is null", params->name());
    TypeSymbol *ts_param = sbc->scope->ts_resolve(
        tokenName(cast(A_PlainType, params->param->type)->token));
    ts_params.push_back(ts_param);
  }

  // get result type
  TypeSymbol *ts_result = sbc->scope->ts_resolve(
      tokenName(cast(A_PlainType, e->resultType)->token));

  // create function type and symbol
  Ts_Func *ts_func =
      new Ts_Func(ts_params, ts_result, sign->location(), sbc->scope);
  S_Func *s_func =
      new S_Func(varId->name(), varId->location(), ts_func, sbc->scope);
  s_func->ast() = varId;
  varId->symbol = s_func;
  sbc->scope->s_define(s_func);

  // push function scope down to subscope
  sbc->scope = s_func;
}

void SymbolBuilder::FuncDef::postVisit(Ast *ast, VisitorContext *context) {
  // pop function scope back to owner scope
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  sbc->scope = sbc->scope->owner();
}

void SymbolBuilder::CompileUnit::visit(Ast *ast, VisitorContext *context) {
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  A_CompileUnit *e = cast(A_CompileUnit, ast);

  // create global scope
  S_Global *sc_global = new S_Global("global", e->location());
  sc_global->ts_define(TypeSymbol::ts_byte());
  sc_global->ts_define(TypeSymbol::ts_ubyte());
  sc_global->ts_define(TypeSymbol::ts_short());
  sc_global->ts_define(TypeSymbol::ts_ushort());
  sc_global->ts_define(TypeSymbol::ts_int());
  sc_global->ts_define(TypeSymbol::ts_uint());
  sc_global->ts_define(TypeSymbol::ts_long());
  sc_global->ts_define(TypeSymbol::ts_ulong());
  sc_global->ts_define(TypeSymbol::ts_float());
  sc_global->ts_define(TypeSymbol::ts_double());
  sc_global->ts_define(TypeSymbol::ts_char());
  sc_global->ts_define(TypeSymbol::ts_boolean());
  sc_global->ts_define(TypeSymbol::ts_void());

  sc_global->ast() = e;
  e->globalScope = sc_global;

  // pass global scope down to subscope
  sbc->scope = sc_global;
}

void SymbolBuilder::CompileUnit::postVisit(Ast *ast, VisitorContext *context) {
  // pop global scope back to owner scope
  SymbolBuilder::Context *sbc = cast(SymbolBuilder::Context, context);
  sbc->scope = sbc->scope->owner();
  LOG_ASSERT(!sbc->scope, "global scope has no owner scope, while not null:{}!",
             sbc->scope->name());
}

// do action }

SymbolBuilder::Context::Context() : scope(nullptr) {}

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new SymbolBuilder::x();                                       \
    binder_.bind((+AstKind::x)._to_integral(), v);                             \
    visitors_.push_back(v);                                                    \
  } while (0)

SymbolBuilder::SymbolBuilder() : binder_(&context_) {
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

SymbolBuilder::~SymbolBuilder() { del_array(visitors_); }

void SymbolBuilder::run(Ast *ast) { Visitor::traverse(binder_, ast); }

// SymbolBuilder }

// SymbolReviewer {

SymbolReviewer::SymbolReviewer() {}

SymbolReviewer::~SymbolReviewer() {}

void SymbolReviewer::run(Ast *ast) {}

// SymbolReviewer }
