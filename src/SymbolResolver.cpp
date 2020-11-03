// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolResolver.h"
#include "Ast.h"
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

namespace symbol_resolver {

struct Context : public VisitorContext, public Scoped {};

struct VISITOR(Loop) : public Visitor {
  VISITOR(Loop)
  () : Visitor("SymbolResolver::" BOOST_PP_STRINGIZE(VISITOR(Loop))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_Loop)
    ctx->scope() = node->scope();
  }

  virtual void finishVisit(Ast *ast) {
    VINIT(A_Loop)
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VISITOR(Block) : public Visitor {
  VISITOR(Block)
  () : Visitor("SymbolResolver::" BOOST_PP_STRINGIZE(VISITOR(Block))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_Block)
    ctx->scope() = node->scope();
  }

  virtual void finishVisit(Ast *ast) {
    VINIT(A_Block)
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VISITOR(FuncDef) : public Visitor {
  VISITOR(FuncDef)
  () : Visitor("SymbolResolver::" BOOST_PP_STRINGIZE(VISITOR(FuncDef))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_FuncDef)
    A_FuncSign *funcSign = static_cast<A_FuncSign *>(node->funcSign);
    A_VarId *funcId = static_cast<A_VarId *>(funcSign->id);

    ctx->scope() = dynamic_cast<Scope *>(funcId->symbol());
  }

  virtual void finishVisit(Ast *ast) {
    VINIT(A_FuncDef)
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VISITOR(CompileUnit) : public Visitor {
  VISITOR(CompileUnit)
  () : Visitor("SymbolResolver::" BOOST_PP_STRINGIZE(VISITOR(CompileUnit))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_CompileUnit)
    ctx->scope() = node->scope();
  }

  virtual void finishVisit(Ast *ast) {
    VINIT(A_CompileUnit)
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(!ctx->scope(), "global scope owner scope must null",
               ctx->scope()->name());
  }
};

struct VISITOR(VarId) : public Visitor {
  VISITOR(VarId)
  () : Visitor("SymbolResolver::" BOOST_PP_STRINGIZE(VISITOR(VarId))) {}

  virtual void visit(Ast *ast) {
    VINIT(A_VarId)
    Symbol *sym = ctx->scope()->s_resolve(node->name());
    TypeSymbol *tsym = ctx->scope()->ts_resolve(node->name());
    if (node->symbol()) {
      LOG_ASSERT(sym, "symbol {}:{} cannot resolve in scope {}:{}",
                 node->name(), node->location(), ctx->scope()->name(),
                 ctx->scope()->location());
      LOG_ASSERT(sym->ast() == node && node->symbol() == sym,
                 "symbol {}:{} must defined in scope {}:{}", sym->name(),
                 sym->location(), ctx->scope()->name(),
                 ctx->scope()->location());
    } else if (node->typeSymbol()) {
      LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
                 node->name(), node->location(), ctx->scope()->name(),
                 ctx->scope()->location());
      LOG_ASSERT(tsym->ast() == node && node->typeSymbol() == tsym,
                 "tyep symbol {}:{} must defined in scope {}:{}", tsym->name(),
                 tsym->location(), ctx->scope()->name(),
                 ctx->scope()->location());
    } else {
      if (sym) {
        node->symbol() = sym;
        LOG_ASSERT(sym->ast() != node, "symbol {}:{} resolve as varId {}:{}",
                   sym->name(), sym->location(), node->name(),
                   node->location());
      } else if (tsym) {
        node->typeSymbol() = tsym;
        LOG_ASSERT(tsym->ast() != node,
                   "type symbol {}:{} resolve as varId {}:{}", tsym->name(),
                   tsym->location(), node->name(), node->location());
      } else {
        LOG_ASSERT(false,
                   "varId {}:{} cannot resolve neither as symbol or as type "
                   "symbol in scope {}:{}",
                   node->name(), node->location(), ctx->scope()->name(),
                   ctx->scope()->location());
      }
    }
  }
};

} // namespace symbol_resolver

} // namespace detail

// SymbolResolver {

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::symbol_resolver::VISITOR(x)();                    \
    binder_.bind((+AstKind::x), v);                                            \
    visitors_.push_back(v);                                                    \
  } while (0)

SymbolResolver::SymbolResolver()
    : Phase("SymbolResolver"), context_(new detail::symbol_resolver::Context()),
      binder_(context_) {
  BIND(Loop);
  BIND(Block);
  BIND(FuncDef);
  BIND(CompileUnit);
  BIND(VarId);
}

SymbolResolver::~SymbolResolver() {
  delete context_;
  context_ = nullptr;
  for (int i = 0; i < (int)visitors_.size(); i++) {
    LOG_ASSERT(visitors_[i], "visitors_[{}] must not null", i);
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
  visitors_.clear();
}

void SymbolResolver::run(Ast *ast) { Visitor::traverse(&binder_, ast); }

// SymbolResolver }
