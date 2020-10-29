// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolResolver.h"
#include "Ast.h"
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

namespace symbol_resolver {

struct Context : public VisitorContext, public Scoped {};

struct Loop : public Visitor {
  Loop() : Visitor("SymbolResolver::Visitor::Loop") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_Loop)
    // push loop scope down to subscope
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_Loop)
    // pop loop scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct Block : public Visitor {
  Block() : Visitor("SymbolResolver::Visitor::Block") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_Block)
    // push block scope down to subscope
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_Block)
    // pop block scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct FuncDef : public Visitor {
  FuncDef() : Visitor("SymbolResolver::Visitor::FuncDef") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_FuncDef)
    A_FuncSign *sign = static_cast<A_FuncSign *>(node->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);

    // push block scope down to subscope
    ctx->scope() = dynamic_cast<Scope *>(varId->symbol());
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_FuncDef)
    // pop block scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("SymbolResolver::Visitor::CompileUnit") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_CompileUnit)
    // pass global scope down to subscope
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast) {
    INITIALIZE(A_CompileUnit)
    // pop global scope back to owner scope
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(
        !ctx->scope(),
        "global scope must has no owner scope, but the owner is not null:{}!",
        ctx->scope()->name());
  }
};

struct VarId : public Visitor {
  VarId() : Visitor("SymbolResolver::Visitor::VarId") {}
  virtual void visit(Ast *ast) {
    INITIALIZE(A_VarId)
    Symbol *sym = ctx->scope()->s_resolve(node->name());
    TypeSymbol *tsym = ctx->scope()->ts_resolve(node->name());
    if (node->symbol()) {
      LOG_ASSERT(sym, "symbol {}:{} not exist in scope {}:{}", node->name(),
                 node->location(), ctx->scope()->name(),
                 ctx->scope()->location());
    } else if (node->typeSymbol()) {
      LOG_ASSERT(tsym, "symbol {}:{} not exist in scope {}:{}", node->name(),
                 node->location(), ctx->scope()->name(),
                 ctx->scope()->location());
    } else {
      if (sym) {
        node->symbol() = sym;
        LOG_ASSERT(sym->ast() != node, "symbol {}:{} resolve in varId {}:{}",
                   sym->name(), sym->location(), node->name(),
                   node->location());
      } else if (tsym) {
        node->typeSymbol() = tsym;
        LOG_ASSERT(tsym->ast() != node,
                   "type symbol {}:{} resolve in varId {}:{}", tsym->name(),
                   tsym->location(), node->name(), node->location());
      } else {
        LOG_ASSERT(false,
                   "varId {}:{} not exist as both symbol and type symbol in "
                   "scope {}:{}",
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
    Visitor *v = new detail::symbol_resolver::x();                             \
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
