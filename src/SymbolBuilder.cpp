// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolBuilder.h"
#include "Ast.h"
#include "NameGenerator.h"
#include "Symbol.h"
#include "Token.h"
#include "Visitor.h"
#include "iface/Scoped.h"
#include "infra/Log.h"

#define INITIALIZE(astype)                                                     \
  Context *ctx = static_cast<Context *>(context());                            \
  astype *node = static_cast<astype *>(ast);                                   \
  (void)ctx;                                                                   \
  (void)node;

namespace detail {

namespace symbol_builder {

static NameGenerator SNG(".");

struct Context : public VisitorContext, public Scoped {};

struct Loop : public Visitor {
  Loop() : Visitor("SymbolBuilder::Visitor::Loop") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_Loop)

    // create local scope
    S_Local *sc_loop = new S_Local(SNG.generate("loop", node->location().str()),
                                   node->location(), ctx->scope());
    sc_loop->ast() = node;
    node->scope() = sc_loop;
    ctx->scope()->subscope_define(sc_loop);

    // push loop scope down to subscope
    ctx->scope() = sc_loop;
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_Loop)
    // pop loop scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct LoopEnumerator : public Visitor {
  LoopEnumerator() : Visitor("SymbolBuilder::Visitor::LoopEnumerator") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_LoopEnumerator)
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varType = static_cast<A_PlainType *>(node->type);

    // get variable type symbol
    TypeSymbol *ts_var = ctx->scope()->ts_resolve(tokenName(varType->token));
    LOG_ASSERT(ts_var, "variable type {} is null", tokenName(varType->token));

    // create variable symbol
    S_Var *s_var =
        new S_Var(varId->name(), varId->location(), ctx->scope(), ts_var);
    s_var->ast() = varId;
    varId->symbol() = s_var;
    ctx->scope()->s_define(s_var);
  }
};

struct Block : public Visitor {
  Block() : Visitor("SymbolBuilder::Visitor::Block") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_Block)

    // create block scope
    S_Local *sc_block =
        new S_Local(SNG.generate("block", node->location().str()),
                    node->location(), ctx->scope());
    sc_block->ast() = node;
    node->scope() = sc_block;
    ctx->scope()->subscope_define(sc_block);

    // push block scope down to subscope
    ctx->scope() = sc_block;
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_Block)
    // pop block scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VarDef : public Visitor {
  VarDef() : Visitor("SymbolBuilder::Visitor::VarDef") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_VarDef)
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varType = static_cast<A_PlainType *>(node->type);

    // get variable type symbol
    TypeSymbol *ts_var = ctx->scope()->ts_resolve(tokenName(varType->token));
    LOG_ASSERT(ts_var, "variable type {} is null", tokenName(varType->token));

    // create variable symbol
    S_Var *s_var =
        new S_Var(varId->name(), varId->location(), ctx->scope(), ts_var);
    s_var->ast() = varId;
    varId->symbol() = s_var;
    ctx->scope()->s_define(s_var);
  }
};

struct Param : public Visitor {
  Param() : Visitor("SymbolBuilder::Visitor::Param") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_Param)
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *plainType = static_cast<A_PlainType *>(node->type);

    // get parameter type symbol
    TypeSymbol *ts_param =
        ctx->scope()->ts_resolve(tokenName(plainType->token));
    LOG_ASSERT(ts_param, "plain type {} is null", tokenName(plainType->token));

    // create parameter symbol
    S_Param *s_param =
        new S_Param(varId->name(), varId->location(), ctx->scope(), ts_param);
    s_param->ast() = varId;
    varId->symbol() = s_param;
    ctx->scope()->s_define(s_param);

    // add param to owner function scope
    LOG_ASSERT(dynamic_cast<Symbol *>(ctx->scope())->kind() ==
                   (+SymbolKind::Func),
               "ctx->scope->kind {} != SymbolKind::Func",
               dynamic_cast<Symbol *>(ctx->scope())->kind()._to_string());
    static_cast<S_Func *>(ctx->scope())->params.push_back(s_param);
  }
};

struct FuncDef : public Visitor {
  FuncDef() : Visitor("SymbolBuilder::Visitor::FuncDef") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_FuncDef)
    A_FuncSign *sign = static_cast<A_FuncSign *>(node->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);

    // get function parameter types
    std::vector<TypeSymbol *> ts_params;
    for (A_Params *params = sign->params; params; params = params->next) {
      LOG_ASSERT(params->param, "params's ({}) param is null", params->name());
      TypeSymbol *ts_param = ctx->scope()->ts_resolve(
          tokenName(static_cast<A_PlainType *>(params->param->type)->token));
      ts_params.push_back(ts_param);
    }

    // get result type
    TypeSymbol *ts_result = ctx->scope()->ts_resolve(
        tokenName(static_cast<A_PlainType *>(node->resultType)->token));

    // create function type and symbol
    Ts_Func *ts_func =
        new Ts_Func(ts_params, ts_result, sign->location(), ctx->scope());
    S_Func *s_func =
        new S_Func(varId->name(), varId->location(), ctx->scope(), ts_func);
    s_func->ast() = varId;
    varId->symbol() = s_func;
    ctx->scope()->s_define(s_func);

    // push function scope down to subscope
    ctx->scope() = s_func;
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_FuncDef)
    // pop function scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("SymbolBuilder::Visitor::CompileUnit") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_CompileUnit)

    // create global scope
    S_Global *sc_global = new S_Global("global", node->location());
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

    sc_global->ast() = node;
    node->scope() = sc_global;

    // pass global scope down to subscope
    ctx->scope() = sc_global;
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_CompileUnit)
    // pop global scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(!ctx->scope(),
               "global scope has no owner scope, while not null:{}!",
               ctx->scope()->name());
  }
};

} // namespace symbol_builder

} // namespace detail

// SymbolBuilder {

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::symbol_builder::x();                              \
    binder_.bind((+AstKind::x), v);                                            \
    visitors_.push_back(v);                                                    \
  } while (0)

SymbolBuilder::SymbolBuilder()
    : Phase("SymbolBuilder"), context_(new detail::symbol_builder::Context()),
      binder_(context_) {
  BIND(Loop);
  BIND(LoopEnumerator);
  BIND(Block);
  BIND(VarDef);
  BIND(Param);
  BIND(FuncDef);
  BIND(CompileUnit);
}

SymbolBuilder::~SymbolBuilder() {
  delete context_;
  context_ = nullptr;
  for (int i = 0; i < (int)visitors_.size(); i++) {
    LOG_ASSERT(visitors_[i], "visitors_[{}] must not null", i);
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
  visitors_.clear();
}

void SymbolBuilder::run(Ast *ast) { Visitor::traverse(&binder_, ast); }

// SymbolBuilder }
