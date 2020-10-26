// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Visitor.h"
#include "Ast.h"
#include "Log.h"
#include <utility>

VisitorBinder::VisitorBinder(VisitorContext *context)
    : context_(context), idleVisitor_(new Visitor("IdleVisitor")) {}

VisitorBinder::~VisitorBinder() {
  delete idleVisitor_;
  idleVisitor_ = nullptr;
}

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
  return it == visitors_.end() ? idleVisitor_ : it->second;
}

Visitor *VisitorBinder::get(Ast *ast) const {
  LOG_ASSERT(ast, "ast must not null");
  return get(ast->kind()._to_integral());
}

VisitorContext *VisitorBinder::context() const { return context_; }

Visitor::Visitor(const Cowstr &name) : Nameable(name) {}

void Visitor::visit(Ast *ast, VisitorContext *context) {}

void Visitor::visitBefore(Ast *ast, Ast *child, VisitorContext *context) {}

void Visitor::visitAfter(Ast *ast, Ast *child, VisitorContext *context) {}

void Visitor::finishVisit(Ast *ast, VisitorContext *context) {}

#define TRAVEL1(ast, astype, child1)                                           \
  do {                                                                         \
    Ast *c1 = static_cast<astype *>(ast)->child1;                              \
    visitor->visitBefore(ast, c1, binder->context());                          \
    traverse(binder, c1);                                                      \
    visitor->visitAfter(ast, c1, binder->context());                           \
  } while (0)

#define TRAVEL2(ast, astype, child1, child2)                                   \
  do {                                                                         \
    Ast *c1 = static_cast<astype *>(ast)->child1;                              \
    Ast *c2 = static_cast<astype *>(ast)->child2;                              \
    visitor->visitBefore(ast, c1, binder->context());                          \
    traverse(binder, c1);                                                      \
    visitor->visitAfter(ast, c1, binder->context());                           \
    visitor->visitBefore(ast, c2, binder->context());                          \
    traverse(binder, c2);                                                      \
    visitor->visitAfter(ast, c2, binder->context());                           \
  } while (0)

#define TRAVEL3(ast, astype, child1, child2, child3)                           \
  do {                                                                         \
    Ast *c1 = static_cast<astype *>(ast)->child1;                              \
    Ast *c2 = static_cast<astype *>(ast)->child2;                              \
    Ast *c3 = static_cast<astype *>(ast)->child3;                              \
    visitor->visitBefore(ast, c1, binder->context());                          \
    traverse(binder, c1);                                                      \
    visitor->visitAfter(ast, c1, binder->context());                           \
    visitor->visitBefore(ast, c2, binder->context());                          \
    traverse(binder, c2);                                                      \
    visitor->visitAfter(ast, c2, binder->context());                           \
    visitor->visitBefore(ast, c3, binder->context());                          \
    traverse(binder, c3);                                                      \
    visitor->visitAfter(ast, c3, binder->context());                           \
  } while (0)

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
  case AstKind::Throw: {
    TRAVEL1(ast, A_Throw, expr);
    break;
  }
  case AstKind::Return: {
    TRAVEL1(ast, A_Return, expr);
    break;
  }
  case AstKind::PostfixExpr: {
    TRAVEL1(ast, A_PostfixExpr, expr);
    break;
  }
  case AstKind::PrefixExpr: {
    TRAVEL1(ast, A_PrefixExpr, expr);
    break;
  }
  case AstKind::Yield: {
    TRAVEL1(ast, A_Yield, expr);
    break;
  }
  case AstKind::Block: {
    TRAVEL1(ast, A_Block, blockStats);
    break;
  }
  case AstKind::CompileUnit: {
    TRAVEL1(ast, A_CompileUnit, topStats);
    break;
  }
    // two child node
  case AstKind::Assign: {
    TRAVEL2(ast, A_Assign, assignee, assignor);
    break;
  }
  case AstKind::InfixExpr: {
    TRAVEL2(ast, A_InfixExpr, left, right);
    break;
  }
  case AstKind::Call: {
    TRAVEL2(ast, A_Call, id, args);
    break;
  }
  case AstKind::Exprs: {
    TRAVEL2(ast, A_Exprs, expr, next);
    break;
  }
  case AstKind::Loop: {
    TRAVEL2(ast, A_Loop, condition, body);
    break;
  }
  case AstKind::DoWhile: {
    TRAVEL2(ast, A_DoWhile, body, condition);
    break;
  }
  case AstKind::BlockStats: {
    TRAVEL2(ast, A_BlockStats, blockStat, next);
    break;
  }
  case AstKind::FuncSign: {
    TRAVEL2(ast, A_FuncSign, id, params);
    break;
  }
  case AstKind::Params: {
    TRAVEL2(ast, A_Params, param, next);
    break;
  }
  case AstKind::Param: {
    TRAVEL2(ast, A_Param, id, type);
    break;
  }
  case AstKind::TopStats: {
    TRAVEL2(ast, A_TopStats, topStat, next);
    break;
  }
    // three child node
  case AstKind::If: {
    TRAVEL3(ast, A_If, condition, thenp, elsep);
    break;
  }
  case AstKind::LoopCondition: {
    TRAVEL3(ast, A_LoopCondition, init, condition, update);
    break;
  }
  case AstKind::LoopEnumerator: {
    TRAVEL3(ast, A_LoopEnumerator, id, type, expr);
    break;
  }
  case AstKind::Try: {
    TRAVEL3(ast, A_Try, tryp, catchp, finallyp);
    break;
  }
  case AstKind::FuncDef: {
    TRAVEL3(ast, A_FuncDef, funcSign, resultType, body);
    break;
  }
  case AstKind::VarDef: {
    TRAVEL3(ast, A_VarDef, id, type, expr);
    break;
  }
  default:
    LOG_ASSERT(false, "invalid ast node kind: {}", ast->kind()._to_string());
  }

  // post visit
  visitor->finishVisit(ast, binder->context());
}
