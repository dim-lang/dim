// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Visitor.h"
#include "Ast.h"
#include "Log.h"
#include "Util.h"
#include <utility>

VisitorBinder::VisitorBinder(VisitorContext *context) : context_(context) {}

int VisitorBinder::bind(int astKind, Visitor *visitor) {
  LOG_ASSERT(visitor, "visitor must not null");
  if (visitors_.find(astKind) != visitors_.end()) {
    return -1;
  }
  visitors_.insert(std::make_pair(astKind, visitor));
  return 0;
}

Visitor *VisitorBinder::get(int astKind) const {
  auto it = visitors_.find(astKind);
  return it == visitors_.end() ? nullcast : it->second;
}

Visitor *VisitorBinder::get(Ast *ast) const {
  LOG_ASSERT(ast, "ast must not null");
  return get(ast->kind()._to_integral());
}

void VisitorBinder::setContext(VisitorContext *context) { context_ = context; }

VisitorContext *VisitorBinder::context() const { return context_; }

void Visitor::visit(Ast *ast) {}

void Visitor::postVisit(Ast *ast) {}

void Visitor::traverse(VisitorBinder *binder, Ast *ast) {
  if (!ast) {
    return;
  }
  LOG_ASSERT(binder, "binder must not null");
  Visitor *visitor = binder->get(ast);
  LOG_ASSERT(visitor, "visitor must not null for ast {}", ast->name());

  // visit
  visitor->visit(ast, binder->context());

  // visit
  switch (ast->kind()) {
    // no child node
  case AstKind::Integer:
  case AstKind::Float:
  case AstKind::Boolean:
  case AstKind::Character:
  case AstKind::String:
  case AstKind::Nil:
  case AstKind::Void:
  case AstKind::VarId:
  case AstKind::Break:
  case AstKind::Continue:
  case AstKind::PlainType:
    break;
    // one child node
  case AstKind::Throw:
    traverse(binder, cast<A_Throw>(ast)->expr);
    break;
  case AstKind::Return:
    traverse(binder, cast<A_Return>(ast)->expr);
    break;
  case AstKind::PostfixExpr:
    traverse(binder, cast<A_PostfixExpr>(ast)->expr);
    break;
  case AstKind::PrefixExpr:
    traverse(binder, cast<A_PrefixExpr>(ast)->expr);
    break;
  case AstKind::Yield:
    traverse(binder, cast<A_Yield>(ast)->expr);
    break;
  case AstKind::Block:
    traverse(binder, cast<A_Block>(ast)->blockStats);
    break;
  case AstKind::CompileUnit:
    traverse(binder, cast<A_CompileUnit>(ast)->topStats);
    break;
    // two child node
  case AstKind::Assign:
    traverse(binder, cast<A_Assign>(ast)->assignee);
    traverse(binder, cast<A_Assign>(ast)->assignor);
    break;
  case AstKind::InfixExpr:
    traverse(binder, cast<A_InfixExpr>(ast)->left);
    traverse(binder, cast<A_InfixExpr>(ast)->right);
    break;
  case AstKind::Call:
    traverse(binder, cast<A_Call>(ast)->id);
    traverse(binder, cast<A_Call>(ast)->args);
    break;
  case AstKind::Exprs:
    traverse(binder, cast<A_Exprs>(ast)->expr);
    traverse(binder, cast<A_Exprs>(ast)->next);
    break;
  case AstKind::Loop:
    traverse(binder, cast<A_Loop>(ast)->condition);
    traverse(binder, cast<A_Loop>(ast)->body);
    break;
  case AstKind::DoWhile:
    traverse(binder, cast<A_DoWhile>(ast)->body);
    traverse(binder, cast<A_DoWhile>(ast)->condition);
    break;
  case AstKind::BlockStats:
    traverse(binder, cast<A_BlockStats>(ast)->blockStat);
    traverse(binder, cast<A_BlockStats>(ast)->next);
    break;
  case AstKind::FuncSign:
    traverse(binder, cast<A_FuncSign>(ast)->id);
    traverse(binder, cast<A_FuncSign>(ast)->params);
    break;
  case AstKind::Params:
    traverse(binder, cast<A_Params>(ast)->param);
    traverse(binder, cast<A_Params>(ast)->next);
    break;
  case AstKind::Param:
    traverse(binder, cast<A_Param>(ast)->id);
    traverse(binder, cast<A_Param>(ast)->type);
    break;
  case AstKind::TopStats:
    traverse(binder, cast<A_TopStats>(ast)->topStat);
    traverse(binder, cast<A_TopStats>(ast)->next);
    break;
    // three child node
  case AstKind::If:
    traverse(binder, cast<A_If>(ast)->condition);
    traverse(binder, cast<A_If>(ast)->thenp);
    traverse(binder, cast<A_If>(ast)->elsep);
    break;
  case AstKind::LoopCondition:
    traverse(binder, cast<A_LoopCondition>(ast)->init);
    traverse(binder, cast<A_LoopCondition>(ast)->condition);
    traverse(binder, cast<A_LoopCondition>(ast)->update);
    break;
  case AstKind::LoopEnumerator:
    traverse(binder, cast<A_LoopEnumerator>(ast)->id);
    traverse(binder, cast<A_LoopEnumerator>(ast)->type);
    traverse(binder, cast<A_LoopEnumerator>(ast)->expr);
    break;
  case AstKind::Try:
    traverse(binder, cast<A_Try *>(ast)->tryp);
    traverse(binder, cast<A_Try *>(ast)->catchp);
    traverse(binder, cast<A_Try *>(ast)->finallyp);
    break;
  case AstKind::FuncDef:
    traverse(binder, cast<A_FuncDef>(ast)->funcSign);
    traverse(binder, cast<A_FuncDef>(ast)->resultType);
    traverse(binder, cast<A_FuncDef>(ast)->body);
    break;
  case AstKind::VarDef:
    traverse(binder, cast<A_VarDef>(ast)->id);
    traverse(binder, cast<A_VarDef>(ast)->type);
    traverse(binder, cast<A_VarDef>(ast)->expr);
    break;
  default:
    LOG_ASSERT(false, "invalid ast node kind: {}", ast->kind()._to_string());
  }

  // post visit
  visitor->postVisit(ast, binder->context());
}
