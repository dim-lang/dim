// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "AstGraph.h"
#include "Ast.h"
#include "Counter.h"
#include "Log.h"
#include "graphviz/cgraph.h"
#include <cstdio>

#define SP_CAST(x) (std::static_pointer_cast<x>(ast))
#define NEW_NODE (agnode(g, (char *)ast->name().toSymbolName().c_str(), TRUE))
#define NEW_EDGE(tail) (agedge(g, tail, u, counterName(), TRUE))

static char *counterName() {
  std::stringstream ss;
  ss << "e" << Counter::get();
  return (char *)ss.str().c_str();
}

static Agnode_t *draw(std::shared_ptr<Ast> ast, Agraph_t *g) {
  switch (ast->category()) {
  case AstCategory::Integer: {
    return NEW_NODE;
  }
  case AstCategory::Float: {
    return NEW_NODE;
  }
  case AstCategory::Boolean: {
    return NEW_NODE;
  }
  case AstCategory::Character: {
    return NEW_NODE;
  }
  case AstCategory::String: {
    return NEW_NODE;
  }
  case AstCategory::Nil: {
    return NEW_NODE;
  }
  case AstCategory::Void: {
    return NEW_NODE;
  }
  case AstCategory::VarId: {
    return NEW_NODE;
  }
  case AstCategory::Break: {
    return NEW_NODE;
  }
  case AstCategory::Continue: {
    return NEW_NODE;
  }
  case AstCategory::Throw: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *v = draw(SP_CAST(A_Throw)->expr, g);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::Return: {
    Agnode_t *u = NEW_NODE;
    if (SP_CAST(A_Return)->expr) {
      Agnode_t *v = draw(SP_CAST(A_Return)->expr, g);
      NEW_EDGE(v);
    }
    return u;
  }
  case AstCategory::Assign: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Assign)->assignee, g);
    Agnode_t *q = draw(SP_CAST(A_Assign)->assignor, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::PostfixExpr: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *v = draw(SP_CAST(A_PostfixExpr)->expr, g);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::InfixExpr: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_InfixExpr)->left, g);
    Agnode_t *q = draw(SP_CAST(A_InfixExpr)->right, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::PrefixExpr: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *v = draw(SP_CAST(A_PrefixExpr)->expr, g);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::Call: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Call)->id, g);
    Agnode_t *q = draw(SP_CAST(A_Call)->args, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::Exprs: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Exprs)->expr, g);
    Agnode_t *q = draw(SP_CAST(A_Exprs)->next, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::If: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_If)->condition, g);
    Agnode_t *q = draw(SP_CAST(A_If)->thenp, g);
    if (SP_CAST(A_If)->elsep) {
      Agnode_t *v = draw(SP_CAST(A_If)->elsep, g);
      NEW_EDGE(v);
    }
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::Loop: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Loop)->condition, g);
    Agnode_t *q = draw(SP_CAST(A_Loop)->body, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::Yield: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *v = draw(SP_CAST(A_Yield)->expr, g);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::LoopCondition: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_LoopCondition)->init, g);
    Agnode_t *q = draw(SP_CAST(A_LoopCondition)->condition, g);
    Agnode_t *v = draw(SP_CAST(A_LoopCondition)->update, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::LoopEnumerator: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_LoopEnumerator)->id, g);
    Agnode_t *q = draw(SP_CAST(A_LoopEnumerator)->expr, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::DoWhile: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_DoWhile)->body, g);
    Agnode_t *q = draw(SP_CAST(A_DoWhile)->condition, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::Try: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Try)->tryp, g);
    Agnode_t *q = draw(SP_CAST(A_Try)->catchp, g);
    Agnode_t *v = draw(SP_CAST(A_Try)->finallyp, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::Block: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *v = draw(SP_CAST(A_Block)->blockStats, g);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::BlockStats: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_BlockStats)->blockStat, g);
    Agnode_t *q = draw(SP_CAST(A_BlockStats)->next, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::PlainType: {
    return NEW_NODE;
  }
  case AstCategory::FuncDef: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_FuncDef)->funcSign, g);
    Agnode_t *q = draw(SP_CAST(A_FuncDef)->resultType, g);
    Agnode_t *v = draw(SP_CAST(A_FuncDef)->body, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::FuncSign: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_FuncSign)->id, g);
    Agnode_t *q = draw(SP_CAST(A_FuncSign)->params, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::Params: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Params)->param, g);
    Agnode_t *q = draw(SP_CAST(A_Params)->next, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::Param: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_Param)->id, g);
    Agnode_t *q = draw(SP_CAST(A_Param)->type, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::VarDef: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_VarDef)->id, g);
    Agnode_t *q = draw(SP_CAST(A_VarDef)->type, g);
    Agnode_t *v = draw(SP_CAST(A_VarDef)->expr, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    NEW_EDGE(v);
    return u;
  }
  case AstCategory::TopStats: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *p = draw(SP_CAST(A_TopStats)->topStat, g);
    Agnode_t *q = draw(SP_CAST(A_TopStats)->next, g);
    NEW_EDGE(p);
    NEW_EDGE(q);
    return u;
  }
  case AstCategory::CompileUnit: {
    Agnode_t *u = NEW_NODE;
    Agnode_t *v = draw(SP_CAST(A_CompileUnit)->topStats, g);
    NEW_EDGE(v);
    return u;
  }
  default:
    LOG_ASSERT(false, "invalid ast category: {}", ast->category()._to_string());
  }
}

AstGraph::AstGraph(std::shared_ptr<Ast> ast) : g(nullptr) {
  g = agopen((char *)"AstGraph", Agdirected, nullptr);
  LOG_ASSERT(g, "g must not null");
  draw(ast, g);
}

AstGraph::~AstGraph() {
  if (g) {
    agclose(g);
    g = nullptr;
  }
}

int AstGraph::write(const std::string &output) {
  FILE *fp = std::fopen(output.c_str(), "w");
  int result = agwrite(g, fp);
  std::fclose(fp);
  return result;
}
