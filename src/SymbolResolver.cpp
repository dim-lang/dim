// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolResolver.h"
#include "Ast.h"
#include "Symbol.h"
#include "iface/Scoped.h"
#include "infra/Log.h"

SymbolResolver::SymbolResolver()
    : Phase("SymbolResolver"), currentScope_(nullptr) {}

SymbolResolver::~SymbolResolver() {}

void SymbolResolver::run(Ast *ast) { ast->accept(this); }

void SymbolResolver::visitLoop(A_Loop *ast) {
  currentScope_ = ast->scope();

  ast->condition->accept(this);
  ast->body->accept(this);

  currentScope_ = currentScope_->owner();
}

void SymbolResolver::visitBlock(A_Block *ast) {
  currentScope_ = ast->scope();
  if (ast->blockStats) {
    ast->blockStats->accept(this);
  }
  currentScope_ = currentScope_->owner();
}

void SymbolResolver::visitFuncDef(A_FuncDef *ast) {
  A_FuncSign *funcSign = static_cast<A_FuncSign *>(node->funcSign);
  A_VarId *funcId = static_cast<A_VarId *>(funcSign->id);

  currentScope_ = dynamic_cast<Scope *>(funcId->symbol());

  ast->resultType->accept(this);
  ast->funcSign->accept(this);
  ast->body->accept(this);

  currentScope_ = currentScope_->owner();
}

void SymbolResolver::visitCompileUnit(A_CompileUnit *ast) {
  currentScope_ = ast->scope();

  if (ast->topStats) {
    ast->topStats->accept(this);
  }

  currentScope_ = currentScope_->owner();
}

void SymbolResolver::visitVarId(A_VarId *ast) {
  Symbol *sym = currentScope_->s_resolve(ast->name());
  TypeSymbol *tsym = currentScope_->ts_resolve(ast->name());
  if (ast->symbol()) {
    LOG_ASSERT(sym, "symbol {}:{} cannot resolve in scope {}:{}", ast->name(),
               ast->location(), currentScope_->name(),
               currentScope_->location());
    LOG_ASSERT(sym->ast() == ast && ast->symbol() == sym,
               "symbol {}:{} must defined in scope {}:{}", sym->name(),
               sym->location(), currentScope_->name(),
               currentScope_->location());
  } else if (ast->typeSymbol()) {
    LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
               ast->name(), ast->location(), currentScope_->name(),
               currentScope_->location());
    LOG_ASSERT(tsym->ast() == ast && ast->typeSymbol() == tsym,
               "tyep symbol {}:{} must defined in scope {}:{}", tsym->name(),
               tsym->location(), currentScope_->name(),
               currentScope_->location());
  } else {
    if (sym) {
      ast->symbol() = sym;
      LOG_ASSERT(sym->ast() != ast, "symbol {}:{} resolve as varId {}:{}",
                 sym->name(), sym->location(), ast->name(), ast->location());
    } else if (tsym) {
      ast->typeSymbol() = tsym;
      LOG_ASSERT(tsym->ast() != ast, "type symbol {}:{} resolve as varId {}:{}",
                 tsym->name(), tsym->location(), ast->name(), ast->location());
    } else {
      LOG_ASSERT(false,
                 "varId {}:{} cannot resolve neither as symbol or as type "
                 "symbol in scope {}:{}",
                 ast->name(), ast->location(), currentScope_->name(),
                 currentScope_->location());
    }
  }
}
