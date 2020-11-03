// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolBuilder.h"
#include "Ast.h"
#include "NameGenerator.h"
#include "Symbol.h"
#include "Token.h"
#include "Visitor.h"
#include "boost/preprocessor/stringize.hpp"
#include "iface/Scoped.h"
#include "infra/Log.h"

#define VINIT(astype)                                                          \
  Context *ctx = static_cast<Context *>(context());                            \
  astype *node = static_cast<astype *>(ast);                                   \
  (void)ctx;                                                                   \
  (void)node;

namespace detail {

namespace symbol_builder {

static NameGenerator SymbolNG(".");

struct Context : public VisitorContext, public Scoped {};

struct VISITOR(Loop) : public Visitor {
  VISITOR(Loop)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(Loop))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_Loop)

    // scope
    Sc_Local *sc_loop =
        new Sc_Local(SymbolNG.generate("loop", node->location().str()),
                     node->location(), ctx->scope());
    sc_loop->ast() = node;
    node->scope() = sc_loop;
    ctx->scope()->subscope_define(sc_loop);

    // new scope
    ctx->scope() = sc_loop;
  }

  virtual void finishVisit(Ast *ast) {
    VINIT(A_Loop)
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(ctx->scope(), "scope must not null");
  }
};

struct VISITOR(LoopEnumerator) : public Visitor {
  VISITOR(LoopEnumerator)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(LoopEnumerator))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_LoopEnumerator)
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varType = static_cast<A_PlainType *>(node->type);

    // type symbol
    TypeSymbol *ts_var = ctx->scope()->ts_resolve(varType->name());
    LOG_ASSERT(ts_var, "variable type symbol {}:{} must not null",
               varType->name(), varType->location());

    // symbol
    S_Var *s_var =
        new S_Var(varId->name(), varId->location(), ctx->scope(), ts_var);
    s_var->ast() = varId;
    varId->symbol() = s_var;
    ctx->scope()->s_define(s_var);
  }
};

struct VISITOR(Block) : public Visitor {
  VISITOR(Block)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(Block))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_Block)

    // scope
    Sc_Local *sc_block =
        new Sc_Local(SymbolNG.generate("block", node->location().str()),
                     node->location(), ctx->scope());
    sc_block->ast() = node;
    node->scope() = sc_block;
    ctx->scope()->subscope_define(sc_block);

    // new scope
    ctx->scope() = sc_block;
  }
  virtual void finishVisit(Ast *ast) {
    VINIT(A_Block)
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(ctx->scope(), "scope must not null");
  }
};

struct VISITOR(VarDef) : public Visitor {
  VISITOR(VarDef)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(VarDef))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_VarDef)
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varType = static_cast<A_PlainType *>(node->type);

    // type symbol
    TypeSymbol *ts_var = ctx->scope()->ts_resolve(varType->name());
    LOG_ASSERT(ts_var, "variable type symbol {}:{} must not null",
               varType->name(), varType->location());

    // symbol
    S_Var *s_var =
        new S_Var(varId->name(), varId->location(), ctx->scope(), ts_var);
    s_var->ast() = varId;
    varId->symbol() = s_var;
    ctx->scope()->s_define(s_var);
  }
};

struct VISITOR(Param) : public Visitor {
  VISITOR(Param)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(Param))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_Param)
    A_VarId *paramId = static_cast<A_VarId *>(node->id);
    A_PlainType *paramType = static_cast<A_PlainType *>(node->type);

    // type symbol
    TypeSymbol *ts_param = ctx->scope()->ts_resolve(paramType->name());
    LOG_ASSERT(ts_param, "param type symbol {}:{} must not null",
               paramType->name(), paramType->location());

    // symbol
    S_Param *s_param = new S_Param(paramId->name(), paramId->location(),
                                   ctx->scope(), ts_param);
    s_param->ast() = paramId;
    paramId->symbol() = s_param;
    ctx->scope()->s_define(s_param);

    // add param
    Symbol *sym = dynamic_cast<Symbol *>(ctx->scope());
    S_Func *s_func = static_cast<S_Func *>(sym);
    LOG_ASSERT(sym->kind() == (+SymbolKind::Func),
               "ctx->scope {}:{} kind {} != SymbolKind::Func",
               ctx->scope()->name(), ctx->scope()->location(),
               sym->kind()._to_string());
    s_func->params.push_back(s_param);
  }
};

struct VISITOR(FuncDef) : public Visitor {
  VISITOR(FuncDef)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(FuncDef))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_FuncDef)
    A_FuncSign *funcSign = static_cast<A_FuncSign *>(node->funcSign);
    A_VarId *funcId = static_cast<A_VarId *>(funcSign->id);

    // parameter types
    std::vector<TypeSymbol *> ts_params;
    for (A_Params *params = funcSign->params; params; params = params->next) {
      LOG_ASSERT(params->param, "function params {}:{} param must not null",
                 params->name(), params->location());
      A_PlainType *paramType = static_cast<A_PlainType *>(params->param->type);
      TypeSymbol *ts_param = ctx->scope()->ts_resolve(paramType->name());
      ts_params.push_back(ts_param);
    }

    // result type
    TypeSymbol *ts_result = ctx->scope()->ts_resolve(
        tokenName(static_cast<A_PlainType *>(node->resultType)->token));

    // symbol
    Ts_Func *ts_func =
        new Ts_Func(ts_params, ts_result, funcSign->location(), ctx->scope());
    S_Func *s_func =
        new S_Func(funcId->name(), funcId->location(), ctx->scope(), ts_func);
    s_func->ast() = funcId;
    funcId->symbol() = s_func;
    ctx->scope()->s_define(s_func);

    // new scope
    ctx->scope() = s_func;
  }

  virtual void finishVisit(Ast *ast) {
    VINIT(A_FuncDef)
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VISITOR(CompileUnit) : public Visitor {
  VISITOR(CompileUnit)
  () : Visitor("SymbolBuilder::" BOOST_PP_STRINGIZE(VISITOR(CompileUnit))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_CompileUnit)

    // scope
    Sc_Global *sc_global = new Sc_Global("global", node->location());
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

    // new scope
    ctx->scope() = sc_global;
  }
  virtual void finishVisit(Ast *ast) {
    VINIT(A_CompileUnit)
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(!ctx->scope(), "global scope owner must null",
               ctx->scope()->name());
  }
};

} // namespace symbol_builder

} // namespace detail

// SymbolBuilder {

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::symbol_builder::VISITOR(x)();                     \
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
