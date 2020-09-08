// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Log.h"
#include "Strings.h"
#include "graphviz/cgraph.h"
#include <cstdio>
#include <list>
#include <sstream>
#include <utility>

#define AST_SP(x) (std::static_pointer_cast<x>(ast))

static char *getName(const std::string &name) {
  std::stringstream ss;
  ss << name << Counter::get();
  return Strings::get(ss.str().c_str());
}

struct AstGraphvizNode {
  char *name;
  char *header;
  std::list<std::pair<char *, char *>> label;

  AstGraphvizNode(char *a_header)
      : name(getName("n")), header(a_header), label() {
    LOG_ASSERT(name, "name must not null");
    LOG_ASSERT(header, "header must not null");
  }

  std::string toString() const {
    std::stringstream ss;
    ss << name << " [label=\"{ " << header;
    for (auto i = label.begin(); i != label.end(); i++) {
      ss << " | + " << i->first << ": " << i->second << "\\l ";
    }
    ss << " }\"]";
    return ss.str();
  }
};

struct AstGraphvizEdge {
  char *label;
  std::shared_ptr<AstGraphvizNode> head;
  std::shared_ptr<AstGraphvizNode> tail;

  AstGraphvizEdge(char *a_label = nullptr,
                  std::shared_ptr<AstGraphvizNode> a_head = nullptr,
                  std::shared_ptr<AstGraphvizNode> a_tail = nullptr)
      : label(a_label), head(a_head), tail(a_tail) {}

  std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("{} -> {} [label=\"{}\"]", head->name, tail->name, label);
    return ss.str();
  }
};

struct AstGraphviz {
  char *nodeShape;
  std::list<std::shared_ptr<AstGraphvizNode>> nodes;
  std::list<std::shared_ptr<AstGraphvizEdge>> edges;

  AstGraphviz() : nodeShape(nullptr), nodes(), edges() {}

  void draw(const std::string &output) {
    FILE *fp = std::fopen(output.c_str(), "w");
    std::fprintf(fp, "digraph {\n");
    std::fprintf(fp, "    node [shape=%s]\n\n", nodeShape);
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      std::fprintf(fp, "    %s\n", (*i)->toString().c_str());
    }
    std::fprintf(fp, "\n");
    for (auto i = edges.begin(); i != edges.end(); i++) {
      std::fprintf(fp, "    %s\n", (*i)->toString().c_str());
    }
    std::fprintf(fp, "}\n");
    std::fclose(fp);
  }

  std::shared_ptr<AstGraphvizNode> drawImpl(std::shared_ptr<Ast> ast) {
    LOG_ASSERT(ast, "ast must not null");
    switch (ast->category()) {
    case AstCategory::Integer: {
      std::shared_ptr<AstGraphvizNode> u(
          new AstGraphvizNode((char *)"integer"));
      u->label.push_back(std::make_pair(Strings::get("literal"),
                                        AST_SP(A_Integer)->name().raw()));
      u->label.push_back(
          std::make_pair(Strings::get("symbolName"),
                         AST_SP(A_Integer)->name().toSymbolName()));
      u->label.push_back(std::make_pair(
          Strings::get("llvmName"), AST_SP(A_Integer)->name().toLLVMName()));
      nodes.push_back(u);
      return u;
    }
    case AstCategory::Float: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"float"));
      return u;
    }
    case AstCategory::Boolean: {
      std::shared_ptr<AstGraphvizNode> u(
          new AstGraphvizNode((char *)"boolean"));
      return u;
    }
    case AstCategory::Character: {
      std::shared_ptr<AstGraphvizNode> u(
          new AstGraphvizNode((char *)"character"));
      return u;
    }
    case AstCategory::String: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"string"));
      return u;
    }
    case AstCategory::Nil: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"nil"));
      return u;
    }
    case AstCategory::Void: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"void"));
      return u;
    }
    case AstCategory::VarId: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"varId"));
      return u;
    }
    case AstCategory::Break: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"break"));
      return u;
    }
    case AstCategory::Continue: {
      std::shared_ptr<AstGraphvizNode> u(
          new AstGraphvizNode((char *)"continue"));
      return u;
    }
    case AstCategory::Throw: {
      std::shared_ptr<AstGraphvizNode> u(new AstGraphvizNode((char *)"throw"));
      std::shared_ptr<AstGraphvizNode> v = drawImpl(AST_SP(A_Throw)->expr);
      std::shared_ptr<AstGraphvizEdge> e(
          new AstGraphvizEdge((char *)"expr", u, v));
      edges.push_back(e);
      return u;
    }
    /* case AstCategory::Return: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   if (AST_SP(A_Return)->expr) { */
    /*     Agnode_t *v = drawImpl(AST_SP(A_Return)->expr, g); */
    /*     AST_EDGE(v); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::Assign: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_Assign)->assignee, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_Assign)->assignor, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::PostfixExpr: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *v = drawImpl(AST_SP(A_PostfixExpr)->expr, g); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    /* case AstCategory::InfixExpr: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_InfixExpr)->left, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_InfixExpr)->right, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::PrefixExpr: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *v = drawImpl(AST_SP(A_PrefixExpr)->expr, g); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    /* case AstCategory::Call: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_Call)->id, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_Call)->args, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::Exprs: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   if (AST_SP(A_Exprs)->expr) { */
    /*     Agnode_t *p = drawImpl(AST_SP(A_Exprs)->expr, g); */
    /*     AST_EDGE(p); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   if (AST_SP(A_Exprs)->next) { */
    /*     Agnode_t *q = drawImpl(AST_SP(A_Exprs)->next, g); */
    /*     AST_EDGE(q); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::If: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_If)->condition, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_If)->thenp, g); */
    /*   if (AST_SP(A_If)->elsep) { */
    /*     Agnode_t *v = drawImpl(AST_SP(A_If)->elsep, g); */
    /*     AST_EDGE(v); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::Loop: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_Loop)->condition, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_Loop)->body, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::Yield: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *v = drawImpl(AST_SP(A_Yield)->expr, g); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    /* case AstCategory::LoopCondition: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   if (AST_SP(A_LoopCondition)->init) { */
    /*     Agnode_t *p = drawImpl(AST_SP(A_LoopCondition)->init, g); */
    /*     AST_EDGE(p); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   if (AST_SP(A_LoopCondition)->condition) { */
    /*     Agnode_t *q = drawImpl(AST_SP(A_LoopCondition)->condition, g); */
    /*     AST_EDGE(q); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   if (AST_SP(A_LoopCondition)->update) { */
    /*     Agnode_t *v = drawImpl(AST_SP(A_LoopCondition)->update, g); */
    /*     AST_EDGE(v); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::LoopEnumerator: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_LoopEnumerator)->id, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_LoopEnumerator)->expr, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::DoWhile: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_DoWhile)->body, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_DoWhile)->condition, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::Try: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_Try)->tryp, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_Try)->catchp, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   if (AST_SP(A_Try)->finallyp) { */
    /*     Agnode_t *v = drawImpl(AST_SP(A_Try)->finallyp, g); */
    /*     AST_EDGE(v); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::Block: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *v = drawImpl(AST_SP(A_Block)->blockStats, g); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    /* case AstCategory::BlockStats: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   if (AST_SP(A_BlockStats)->blockStat) { */
    /*     Agnode_t *p = drawImpl(AST_SP(A_BlockStats)->blockStat, g); */
    /*     AST_EDGE(p); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   if (AST_SP(A_BlockStats)->next) { */
    /*     Agnode_t *q = drawImpl(AST_SP(A_BlockStats)->next, g); */
    /*     AST_EDGE(q); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::PlainType: { */
    /*   return AST_NODE; */
    /* } */
    /* case AstCategory::FuncDef: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_FuncDef)->funcSign, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_FuncDef)->resultType, g); */
    /*   Agnode_t *v = drawImpl(AST_SP(A_FuncDef)->body, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    /* case AstCategory::FuncSign: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_FuncSign)->id, g); */
    /*   AST_EDGE(p); */
    /*   if (AST_SP(A_FuncSign)->params) { */
    /*     Agnode_t *q = drawImpl(AST_SP(A_FuncSign)->params, g); */
    /*     AST_EDGE(q); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::Params: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_Params)->param, g); */
    /*   AST_EDGE(p); */
    /*   if (AST_SP(A_Params)->next) { */
    /*     Agnode_t *q = drawImpl(AST_SP(A_Params)->next, g); */
    /*     AST_EDGE(q); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::Param: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_Param)->id, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_Param)->type, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   return u; */
    /* } */
    /* case AstCategory::VarDef: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *p = drawImpl(AST_SP(A_VarDef)->id, g); */
    /*   Agnode_t *q = drawImpl(AST_SP(A_VarDef)->type, g); */
    /*   Agnode_t *v = drawImpl(AST_SP(A_VarDef)->expr, g); */
    /*   AST_EDGE(p); */
    /*   AST_EDGE(q); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    /* case AstCategory::TopStats: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   if (AST_SP(A_TopStats)->topStat) { */
    /*     Agnode_t *p = drawImpl(AST_SP(A_TopStats)->topStat, g); */
    /*     AST_EDGE(p); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   if (AST_SP(A_TopStats)->next) { */
    /*     Agnode_t *q = drawImpl(AST_SP(A_TopStats)->next, g); */
    /*     AST_EDGE(q); */
    /*   } else { */
    /*     Agnode_t *v = AST_NIL; */
    /*     AST_EDGE(v); */
    /*   } */
    /*   return u; */
    /* } */
    /* case AstCategory::CompileUnit: { */
    /*   Agnode_t *u = AST_NODE; */
    /*   Agnode_t *v = drawImpl(AST_SP(A_CompileUnit)->topStats, g); */
    /*   AST_EDGE(v); */
    /*   return u; */
    /* } */
    default:
      LOG_ASSERT(false, "invalid ast category: {}",
                 ast->category()._to_string());
    }
  }
};

#define AST_SP(x) (std::static_pointer_cast<x>(ast))
#define AST_NODE (agnode(g, getName(ast->name().toSymbolName()), TRUE))
#define AST_NIL (agnode(g, getName("nil."), TRUE))
#define AST_EDGE(tail) (agedge(g, u, tail, getName("e."), TRUE))

/* static std::shared_ptr<AstGraphvizNode> */
/* drawImpl(std::shared_ptr<Ast> ast, std::shared_ptr<AstGraphviz> g) { */
/*   LOG_ASSERT(ast, "ast must not null"); */
/*   LOG_ASSERT(g, "g must not null"); */
/*   switch (ast->category()) { */
/*   case AstCategory::Integer: { */
/*     std::shared_ptr<AstGraphvizNode> n( */
/*         new AstGraphvizNode(getName("n"), (char *)"Integer")); */
/*     n->label.push_back(std::make_pair((char *)Strings::get("literal"), */
/*                                       (char
 * *)AST_SP(A_Integer)->name().raw())); */
/*     n->label.push_back( */
/*         std::make_pair((char *)Strings::get("symbolName"), */
/*                        (char *)AST_SP(A_Integer)->name().toSymbolName())); */
/*     n->label.push_back( */
/*         std::make_pair((char *)Strings::get("llvmName"), */
/*                        (char *)AST_SP(A_Integer)->name().toLLVMName())); */
/*     g->nodes.push_back(n); */
/*     return n; */
/*   } */
/*   case AstCategory::Float: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Boolean: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Character: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::String: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Nil: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Void: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::VarId: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Break: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Continue: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::Throw: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *v = drawImpl(AST_SP(A_Throw)->expr, g); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::Return: { */
/*     Agnode_t *u = AST_NODE; */
/*     if (AST_SP(A_Return)->expr) { */
/*       Agnode_t *v = drawImpl(AST_SP(A_Return)->expr, g); */
/*       AST_EDGE(v); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::Assign: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_Assign)->assignee, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_Assign)->assignor, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::PostfixExpr: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *v = drawImpl(AST_SP(A_PostfixExpr)->expr, g); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::InfixExpr: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_InfixExpr)->left, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_InfixExpr)->right, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::PrefixExpr: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *v = drawImpl(AST_SP(A_PrefixExpr)->expr, g); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::Call: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_Call)->id, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_Call)->args, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::Exprs: { */
/*     Agnode_t *u = AST_NODE; */
/*     if (AST_SP(A_Exprs)->expr) { */
/*       Agnode_t *p = drawImpl(AST_SP(A_Exprs)->expr, g); */
/*       AST_EDGE(p); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     if (AST_SP(A_Exprs)->next) { */
/*       Agnode_t *q = drawImpl(AST_SP(A_Exprs)->next, g); */
/*       AST_EDGE(q); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::If: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_If)->condition, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_If)->thenp, g); */
/*     if (AST_SP(A_If)->elsep) { */
/*       Agnode_t *v = drawImpl(AST_SP(A_If)->elsep, g); */
/*       AST_EDGE(v); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::Loop: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_Loop)->condition, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_Loop)->body, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::Yield: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *v = drawImpl(AST_SP(A_Yield)->expr, g); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::LoopCondition: { */
/*     Agnode_t *u = AST_NODE; */
/*     if (AST_SP(A_LoopCondition)->init) { */
/*       Agnode_t *p = drawImpl(AST_SP(A_LoopCondition)->init, g); */
/*       AST_EDGE(p); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     if (AST_SP(A_LoopCondition)->condition) { */
/*       Agnode_t *q = drawImpl(AST_SP(A_LoopCondition)->condition, g); */
/*       AST_EDGE(q); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     if (AST_SP(A_LoopCondition)->update) { */
/*       Agnode_t *v = drawImpl(AST_SP(A_LoopCondition)->update, g); */
/*       AST_EDGE(v); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::LoopEnumerator: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_LoopEnumerator)->id, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_LoopEnumerator)->expr, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::DoWhile: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_DoWhile)->body, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_DoWhile)->condition, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::Try: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_Try)->tryp, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_Try)->catchp, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     if (AST_SP(A_Try)->finallyp) { */
/*       Agnode_t *v = drawImpl(AST_SP(A_Try)->finallyp, g); */
/*       AST_EDGE(v); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::Block: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *v = drawImpl(AST_SP(A_Block)->blockStats, g); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::BlockStats: { */
/*     Agnode_t *u = AST_NODE; */
/*     if (AST_SP(A_BlockStats)->blockStat) { */
/*       Agnode_t *p = drawImpl(AST_SP(A_BlockStats)->blockStat, g); */
/*       AST_EDGE(p); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     if (AST_SP(A_BlockStats)->next) { */
/*       Agnode_t *q = drawImpl(AST_SP(A_BlockStats)->next, g); */
/*       AST_EDGE(q); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::PlainType: { */
/*     return AST_NODE; */
/*   } */
/*   case AstCategory::FuncDef: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_FuncDef)->funcSign, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_FuncDef)->resultType, g); */
/*     Agnode_t *v = drawImpl(AST_SP(A_FuncDef)->body, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::FuncSign: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_FuncSign)->id, g); */
/*     AST_EDGE(p); */
/*     if (AST_SP(A_FuncSign)->params) { */
/*       Agnode_t *q = drawImpl(AST_SP(A_FuncSign)->params, g); */
/*       AST_EDGE(q); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::Params: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_Params)->param, g); */
/*     AST_EDGE(p); */
/*     if (AST_SP(A_Params)->next) { */
/*       Agnode_t *q = drawImpl(AST_SP(A_Params)->next, g); */
/*       AST_EDGE(q); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::Param: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_Param)->id, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_Param)->type, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     return u; */
/*   } */
/*   case AstCategory::VarDef: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *p = drawImpl(AST_SP(A_VarDef)->id, g); */
/*     Agnode_t *q = drawImpl(AST_SP(A_VarDef)->type, g); */
/*     Agnode_t *v = drawImpl(AST_SP(A_VarDef)->expr, g); */
/*     AST_EDGE(p); */
/*     AST_EDGE(q); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   case AstCategory::TopStats: { */
/*     Agnode_t *u = AST_NODE; */
/*     if (AST_SP(A_TopStats)->topStat) { */
/*       Agnode_t *p = drawImpl(AST_SP(A_TopStats)->topStat, g); */
/*       AST_EDGE(p); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     if (AST_SP(A_TopStats)->next) { */
/*       Agnode_t *q = drawImpl(AST_SP(A_TopStats)->next, g); */
/*       AST_EDGE(q); */
/*     } else { */
/*       Agnode_t *v = AST_NIL; */
/*       AST_EDGE(v); */
/*     } */
/*     return u; */
/*   } */
/*   case AstCategory::CompileUnit: { */
/*     Agnode_t *u = AST_NODE; */
/*     Agnode_t *v = drawImpl(AST_SP(A_CompileUnit)->topStats, g); */
/*     AST_EDGE(v); */
/*     return u; */
/*   } */
/*   default: */
/*     LOG_ASSERT(false, "invalid ast category: {}",
 * ast->category()._to_string()); */
/*   } */
/* } */

int Graph::drawAst(std::shared_ptr<Ast> ast, const std::string &output) {
  Agraph_t *g = agopen((char *)"Graph", Agdirected, nullptr);
  LOG_ASSERT(g, "g must not null");
  // initialize attributes
  Agsym_t *s = agattr(g, AGNODE, (char *)"shape", (char *)"record");
  LOG_ASSERT(s, "s must not null");
  /* drawImpl(ast, g); */
  FILE *fp = std::fopen(output.c_str(), "w");
  int result = agwrite(g, fp);
  std::fclose(fp);
  agclose(g);
  return result;
}
