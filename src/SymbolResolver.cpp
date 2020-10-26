// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolResolver.h"
#include "Ast.h"
#include "Log.h"
#include "Name.h"
#include "Symbol.h"
#include "Token.h"
#include "Visitor.h"

namespace detail {

namespace symbol_resolver {

struct Context : public VisitorContext {
  Context() : scope(nullptr) {}
  Scope *scope;
};

struct Loop : public Visitor {
  Loop() : Visitor("SymbolResolver::Loop::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    // push loop scope down to subscope
    static_cast<Context *>(context)->scope =
        static_cast<A_Loop *>(ast)->localScope;
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    // pop loop scope back to owner scope
    Context *ctx = static_cast<Context *>(context);
    ctx->scope = ctx->scope->owner();
  }
};

struct Block : public Visitor {
  Block() : Visitor("SymbolResolver::Block::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    // push block scope down to subscope
    static_cast<Context *>(context)->scope =
        static_cast<A_Block *>(ast)->localScope;
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    // pop block scope back to owner scope
    Context *ctx = static_cast<Context *>(context);
    ctx->scope = ctx->scope->owner();
  }
};

struct FuncDef : public Visitor {
  FuncDef() : Visitor("SymbolResolver::FuncDef::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_FuncDef *e = static_cast<A_FuncDef *>(ast);
    A_FuncSign *sign = static_cast<A_FuncSign *>(e->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);

    // push block scope down to subscope
    ctx->scope = dynamic_cast<Scope *>(varId->symbol);
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    // pop block scope back to owner scope
    Context *ctx = static_cast<Context *>(context);
    ctx->scope = ctx->scope->owner();
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("SymbolResolver::CompileUnit::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    // pass global scope down to subscope
    static_cast<Context *>(context)->scope =
        static_cast<A_CompileUnit *>(ast)->globalScope;
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    // pop global scope back to owner scope
    Context *ctx = static_cast<Context *>(context);
    ctx->scope = ctx->scope->owner();
    LOG_ASSERT(
        !ctx->scope,
        "global scope must has no owner scope, but the owner is not null:{}!",
        ctx->scope->name());
  }
};

struct VarId : public Visitor {
  VarId() : Visitor("SymbolResolver::VarId::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Scope *scope = static_cast<Context *>(context)->scope;
    A_VarId *varId = static_cast<A_VarId *>(ast);
    Symbol *sym = scope->s_resolve(varId->name());
    TypeSymbol *tsym = scope->ts_resolve(varId->name());
    if (varId->symbol) {
      LOG_ASSERT(sym, "symbol [{}:{}] not exist in scope [{}:{}]",
                 varId->name(), varId->location().str(), scope->name(),
                 scope->location().str());
    } else if (varId->typeSymbol) {
      LOG_ASSERT(tsym, "symbol [{}:{}] not exist in scope [{}:{}]",
                 varId->name(), varId->location().str(), scope->name(),
                 scope->location().str());
    } else {
      if (sym) {
        varId->symbol = sym;
        LOG_ASSERT(sym->ast() != varId, "symbol {}:{} resolve in varId {}:{}",
                   sym->name(), sym->location().str(), varId->name(),
                   varId->location().str());
      } else if (tsym) {
        varId->typeSymbol = tsym;
        LOG_ASSERT(tsym->ast() != varId,
                   "type symbol {}:{} resolve in varId {}:{}", tsym->name(),
                   tsym->location().str(), varId->name(),
                   varId->location().str());
      } else {
        LOG_ASSERT(false,
                   "varId [{}:{}] not exist as both symbol and type symbol in "
                   "scope [{}:{}]",
                   varId->name(), varId->location().str(), scope->name(),
                   scope->location().str());
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
    binder_.bind((+AstKind::x)._to_integral(), v);                             \
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
