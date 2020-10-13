// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Visitor.h"
#include "Ast.h"
#include "Log.h"
#include "Util.h"
#include <utility>

#define KIND_MAP(x)                                                            \
  { (+AstKind::x)._to_integral(), (+VisitorKind::x)._to_integral() }

static const std::unordered_map<int, int> AVKindMap = {
    KIND_MAP(Integer),
    KIND_MAP(Float),
};

VisitorBinder::VisitorBinder(const std::vector<Visitor *> &visitors) {
  for (int i = 0; i < (int)visitors.size(); i++) {
    Visitor *v = visitors[i];
    LOG_ASSERT(v, "v must not null");
    LOG_ASSERT(visitors_.find(v->kind()._to_integral()) == visitors_.end(),
               "v->kind {} already exist", v->kind()._to_string());
    visitors_.insert(std::make_pair(v->kind()._to_integral(), v));
  }
}

VisitorBinder::~VisitorBinder() { del(visitors_); }

int VisitorBinder::bind(Visitor *visitor) {
  LOG_ASSERT(visitor, "visitor must not null");
  if (visitors_.find(visitor->kind()._to_integral()) != visitors_.end()) {
    return -1;
  }
  visitors_.insert(std::make_pair(visitor->kind()._to_integral(), visitor));
  return 0;
}

Visitor *VisitorBinder::get(Ast *anode) const {
  LOG_ASSERT(anode, "anode must not null");
  auto it1 = AVKindMap.find(anode->kind()._to_integral());
  if (it1 == AVKindMap.end()) {
    return nullptr;
  }
  auto it2 = visitors_.find(it1->second);
  if (it2 == visitors_.end()) {
    return nullptr;
  }
  return it2->second;
}

void Visitor::visit(Ast *anode) {}

void Visitor::postVisit(Ast *anode) {}

void Visitor::traverse(VisitorBinder *binder, Ast *anode) {
  if (!anode) {
    return;
  }
  LOG_ASSERT(binder, "binder must not null");
  Visitor *visitor = binder->get(anode);
  LOG_ASSERT(visitor, "visitor must not null for anode {}", anode->name());

  // visit
  visitor->visit(this, anode);

  // visit
  switch (anode->kind()) {
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
    traverse(binder, ptr<A_Throw>(anode)->expr);
    break;
  case AstKind::Return:
    traverse(binder, ptr<A_Return>(anode)->expr);
    break;
  case AstKind::PostfixExpr:
    traverse(binder, ptr<A_PostfixExpr>(anode)->expr);
    break;
  case AstKind::PrefixExpr:
    traverse(binder, ptr<A_PrefixExpr>(anode)->expr);
    break;
  case AstKind::Yield:
    traverse(binder, ptr<A_Yield>(anode)->expr);
    break;
  case AstKind::Block:
    traverse(binder, ptr<A_Block>(anode)->blockStats);
    break;
  case AstKind::CompileUnit:
    traverse(binder, ptr<A_CompileUnit>(anode)->topStats);
    break;
    // two child node
  case AstKind::Assign:
    traverse(binder, ptr<A_Assign>(anode)->assignee);
    traverse(binder, ptr<A_Assign>(anode)->assignor);
    break;
  case AstKind::InfixExpr:
    traverse(binder, ptr<A_InfixExpr>(anode)->left);
    traverse(binder, ptr<A_InfixExpr>(anode)->right);
    break;
  case AstKind::Call:
    traverse(binder, ptr<A_Call>(anode)->id);
    traverse(binder, ptr<A_Call>(anode)->args);
    break;
  case AstKind::Exprs:
    traverse(binder, ptr<A_Exprs>(anode)->expr);
    traverse(binder, ptr<A_Exprs>(anode)->next);
    break;
  case AstKind::Loop:
    traverse(binder, ptr<A_Loop>(anode)->condition);
    traverse(binder, ptr<A_Loop>(anode)->body);
    break;
  case AstKind::DoWhile:
    traverse(binder, ptr<A_DoWhile>(anode)->body);
    traverse(binder, ptr<A_DoWhile>(anode)->condition);
    break;
  case AstKind::BlockStats:
    traverse(binder, ptr<A_BlockStats>(anode)->blockStat);
    traverse(binder, ptr<A_BlockStats>(anode)->next);
    break;
  case AstKind::FuncSign:
    traverse(binder, ptr<A_FuncSign>(anode)->id);
    traverse(binder, ptr<A_FuncSign>(anode)->params);
    break;
  case AstKind::Params:
    traverse(binder, ptr<A_Params>(anode)->param);
    traverse(binder, ptr<A_Params>(anode)->next);
    break;
  case AstKind::Param:
    traverse(binder, ptr<A_Param>(anode)->id);
    traverse(binder, ptr<A_Param>(anode)->type);
    break;
  case AstKind::TopStats:
    traverse(binder, ptr<A_TopStats>(anode)->topStat);
    traverse(binder, ptr<A_TopStats>(anode)->next);
    break;
    // three child node
  case AstKind::If:
    traverse(binder, ptr<A_If>(anode)->condition);
    traverse(binder, ptr<A_If>(anode)->thenp);
    traverse(binder, ptr<A_If>(anode)->elsep);
    break;
  case AstKind::LoopCondition:
    traverse(binder, ptr<A_LoopCondition>(anode)->init);
    traverse(binder, ptr<A_LoopCondition>(anode)->condition);
    traverse(binder, ptr<A_LoopCondition>(anode)->update);
    break;
  case AstKind::LoopEnumerator:
    traverse(binder, ptr<A_LoopEnumerator>(anode)->id);
    traverse(binder, ptr<A_LoopEnumerator>(anode)->type);
    traverse(binder, ptr<A_LoopEnumerator>(anode)->expr);
    break;
  case AstKind::Try:
    traverse(binder, ptr<A_Try *>(anode)->tryp);
    traverse(binder, ptr<A_Try *>(anode)->catchp);
    traverse(binder, ptr<A_Try *>(anode)->finallyp);
    break;
  case AstKind::FuncDef:
    traverse(binder, ptr<A_FuncDef>(anode)->funcSign);
    traverse(binder, ptr<A_FuncDef>(anode)->resultType);
    traverse(binder, ptr<A_FuncDef>(anode)->body);
    break;
  case AstKind::VarDef:
    traverse(binder, ptr<A_VarDef>(anode)->id);
    traverse(binder, ptr<A_VarDef>(anode)->type);
    traverse(binder, ptr<A_VarDef>(anode)->expr);
    break;
  default:
    LOG_ASSERT(false, "invalid ast node kind: {}", anode->kind()._to_string());
  }

  // post visit
  visitor->postVisit(this, anode);
}
