// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "SymbolBuilder.h"
#include "Ast.h"
#include "NameGenerator.h"
#include "Symbol.h"
#include "infra/Log.h"

static NameGenerator SymbolNG(".");

SymbolBuilder::SymbolBuilder()
    : Phase("SymbolBuilder"), currentScope_(nullptr) {}

void SymbolBuilder::run(Ast *ast) { ast->accept(this); }

void SymbolBuilder::visitLoop(A_Loop *ast) {
  // scope
  Sc_Local *sc_loop =
      new Sc_Local(SymbolNG.generate("loop", ast->location().str()),
                   ast->location(), currentScope_);
  sc_loop->ast() = ast;
  ast->scope() = sc_loop;
  currentScope_->sc_define(sc_loop);

  // update scope
  currentScope_ = sc_loop;

  ast->condition->accept(this);
  ast->body->accept(this);

  // update scope
  currentScope_ = currentScope_->owner();
}

void SymbolBuilder::visitLoopEnumerator(A_LoopEnumerator *ast) {
  A_VarId *varId = static_cast<A_VarId *>(ast->id);
  A_PlainType *varType = static_cast<A_PlainType *>(ast->type);

  // type symbol
  TypeSymbol *ts_var = currentScope_->ts_resolve(varType->name());
  LOG_ASSERT(ts_var, "variable type symbol {}:{} must not null",
             varType->name(), varType->location());

  // symbol
  S_Var *s_var =
      new S_Var(varId->name(), varId->location(), currentScope_, ts_var);
  s_var->ast() = varId;
  varId->symbol() = s_var;
  currentScope_->s_define(s_var);
}

void SymbolBuilder::visitBlock(A_Block *ast) {
  // scope
  Sc_Local *sc_block =
      new Sc_Local(SymbolNG.generate("block", ast->location().str()),
                   ast->location(), currentScope_);
  sc_block->ast() = ast;
  ast->scope() = sc_block;
  currentScope_->sc_define(sc_block);

  // update scope
  currentScope_ = sc_block;

  if (ast->blockStats) {
    ast->blockStats->accept(this);
  }

  // update scope
  currentScope_ = currentScope_->owner();
}

void SymbolBuilder::visitVarDef(A_VarDef *ast) {
  A_VarId *varId = static_cast<A_VarId *>(ast->id);
  A_PlainType *varType = static_cast<A_PlainType *>(ast->type);

  // type symbol
  TypeSymbol *ts_var = currentScope_->ts_resolve(varType->name());
  LOG_ASSERT(ts_var, "variable type symbol {}:{} must not null",
             varType->name(), varType->location());

  // symbol
  S_Var *s_var =
      new S_Var(varId->name(), varId->location(), currentScope_, ts_var);
  s_var->ast() = varId;
  varId->symbol() = s_var;
  currentScope_->s_define(s_var);
}

void SymbolBuilder::visitParam(A_Param *ast) {
  A_VarId *paramId = static_cast<A_VarId *>(ast->id);
  A_PlainType *paramType = static_cast<A_PlainType *>(ast->type);

  TypeSymbol *ts_param = currentScope_->ts_resolve(paramType->name());
  LOG_ASSERT(ts_param, "paramType {}:{} must not null", paramType->name(),
             paramType->location());

  S_Param *s_param = new S_Param(paramId->name(), paramId->location(),
                                 currentScope_, ts_param);
  s_param->ast() = paramId;
  paramId->symbol() = s_param;
  currentScope_->s_define(s_param);

  Symbol *sym = dynamic_cast<Symbol *>(currentScope_);
  LOG_ASSERT(sym->kind() == +SymbolKind::Func,
             "currentScope_ kind {} != SymbolKind::Func",
             sym->kind()._to_string());
  S_Func *s_func = static_cast<S_Func *>(sym);
  s_func->params.push_back(s_param);
}

void SymbolBuilder::visitFuncDef(A_FuncDef *ast) {
  A_VarId *funcId = static_cast<A_VarId *>(ast->getId());
  std::vector<std::pair<Ast *, Ast *>> funcArgs = ast->getArguments();
  A_PlainType *funcResultType = static_cast<A_PlainType *>(ast->resultType);

  // parameter types
  std::vector<TypeSymbol *> ts_params;
  for (int i = 0; i < (int)funcArgs.size(); i++) {
    A_PlainType *argType = static_cast<A_PlainType *>(funcArgs[i].second);
    TypeSymbol *ts_param = currentScope_->ts_resolve(argType->name());
    ts_params.push_back(ts_param);
  }

  // result type
  TypeSymbol *ts_result = currentScope_->ts_resolve(funcResultType->name());

  // symbol
  Ts_Func *ts_func =
      new Ts_Func(ts_params, ts_result, funcId->location(), currentScope_);
  S_Func *s_func =
      new S_Func(funcId->name(), funcId->location(), currentScope_, ts_func);
  s_func->ast() = funcId;
  funcId->symbol() = s_func;
  currentScope_->s_define(s_func);

  // update scope
  currentScope_ = s_func;

  ast->funcSign->accept(this);
  ast->body->accept(this);

  // update scope
  currentScope_ = currentScope_->owner();
}

void SymbolBuilder::visitCompileUnit(A_CompileUnit *ast) {
  // scope
  Sc_Global *sc_global = new Sc_Global("global", ast->location());
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

  sc_global->ast() = ast;
  ast->scope() = sc_global;

  // new scope
  currentScope_ = sc_global;

  if (ast->topStats) {
    ast->topStats->accept(this);
  }

  currentScope_ = currentScope_->owner();
  LOG_ASSERT(!currentScope_, "currentScope_ must be null: {}:{}",
             currentScope_->name(), currentScope_->location());
}
