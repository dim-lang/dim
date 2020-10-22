// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Counter.h"
#include "Files.h"
#include "Log.h"
#include "Symbol.h"
#include "Token.h"
#include "Visitor.h"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace detail {

namespace graph {

static CounterNameGenerator CNG;

static Cowstr identify(Identifiable *identifier) {
  return Cowstr("id") + identifier->decIdentifier();
}

static const std::unordered_map<char, Cowstr> HtmlTranslator = {
    {'\\', "\\\\"},
    {'\n', "<BR/>"},
    {'\t', "&#9;"},
    {'\r', "\\r"},
    // {'\?', "\\?"},
    {'\a', "\\a"},
    {'\b', "\\b"},
    {'\f', "\\f"},
    {'\v', "\\v"},
    {'&', "&#38;"},
    // {'\'', "\\\'"},
    // {'\"', "\\\""},
    {'<', "&lt;"},
    {'>', "&gt;"},
    {'|', "&#124;"}};

#define TRANSLATE(x) (Cowstr(x).replace(HtmlTranslator))

/**
 * <TD PORT="id"> value </TD>
 * <TD PORT="id" COLSPAN="width"> value </TD>
 */
struct GCell {
  GCell(const Cowstr &a_value)
      : id(CNG.generate("cell")), value(TRANSLATE(a_value)), width(0) {}
  GCell(const Cowstr &a_id, const Cowstr &a_value)
      : id(a_id), value(TRANSLATE(a_value)), width(0) {}

  Cowstr id; // PORT="id"
  Cowstr value;
  int width; // COLSPAN="width"

  Cowstr str() const {
    return width ? fmt::format("<TD PORT=\"{}\" COLSPAN=\"{}\">{}</TD>", id,
                               width, value)
                 : fmt::format("<TD PORT=\"{}\">{}</TD>", id, value);
  }
};

/**
 * <TR> GCell+ </TR>
 */
struct GLine {
  GLine(const std::vector<GCell> &a_cells) : cells(a_cells) {}

  std::vector<GCell> cells;

  Cowstr str() const {
    std::stringstream ss;
    ss << "        <TR>";
    for (int i = 0; i < (int)cells.size(); i++) {
      ss << cells[i].str();
    }
    ss << "</TR>";
    return ss.str();
  }
};

/**
 * (node)id [label=<<TABLE attributes> GLine+ </TABLE>>]
 */
struct GNode {
  GNode() : ident(CNG.generate("id")) {}
  GNode(const Cowstr &a_ident) : ident(a_ident) {}
  virtual ~GNode() = default;

  Cowstr ident;
  std::vector<GLine> lines;

  virtual void adjust() {
    if (lines.empty()) {
      return;
    }
    int maxWidth = 0;
    for (int i = 0; i < (int)lines.size(); i++) {
      maxWidth = std::max<int>(lines[i].cells.size(), maxWidth);
    }
    for (int i = 0; i < (int)lines.size(); i++) {
      if (lines[i].cells.size() > 0 && lines[i].cells.size() < maxWidth) {
        lines[i].cells[0].width = maxWidth - lines[i].cells.size() + 1;
      }
    }
  }

  virtual Cowstr str() const {
    const std::unordered_map<Cowstr, Cowstr> &attributes = {
        {"BORDER", "\"0\""},
        {"CELLBORDER", "\"1\""},
        {"CELLSPACING", "\"0\""},
    };
    std::stringstream ss;
    ss << ident << " [label=<<TABLE";
    for (auto i = attributes.begin(); i != attributes.end(); i++) {
      ss << " " << i->first << "=" << i->second;
    }
    ss << ">\n";
    for (int i = 0; i < (int)lines.size(); i++) {
      ss << lines[i].str();
      if (i < (int)lines.size() - 1) {
        ss << "\n";
      }
    }
    ss << "</TABLE>>]";
    return ss.str();
  }
  virtual Cowstr id() const { return ident; }
};

struct NilNode : public GNode {
  NilNode() : GNode() {
    GCell c("nil");
    GLine l({c});
    lines.push_back(l);
  }
};

struct AstNode : public GNode {
  AstNode(Ast *ast) : GNode(identify(ast)) {
    add(ast->kind()._to_string());
    add("id", ast->decIdentifier());
    add("location", ast->location().str());
  }

  virtual void add(const Cowstr &a) {
    GCell c(a);
    GLine l({c});
    lines.push_back(l);
  }
  virtual void add(const Cowstr &a, const Cowstr &b) {
    GCell c1(a);
    GCell c2(b);
    GLine l({c1, c2});
    lines.push_back(l);
  }
};

struct ScopeNode : public GNode {
  ScopeNode(Scope *scope) : GNode(identify(scope)) {
    add(scope->name());
    add("id", scope->decIdentifier());
    add("location", scope->location().str());
  }

  virtual void add(const Cowstr &a) {
    GCell c1(a);
    GLine line({c1});
    lines.push_back(line);
  }
  virtual void add(const Cowstr &a, const Cowstr &b) {
    GCell c1(a);
    GCell c2(b);
    GLine line({c1, c2});
    lines.push_back(line);
  }
  virtual void add(const Cowstr &a, const Cowstr &b, const Cowstr &c) {
    GCell c1(a);
    GCell c2(b);
    GCell c3(c);
    GLine line({c1, c2, c3});
    lines.push_back(line);
  }
  virtual void add(const Cowstr &a, const Cowstr &b, const Cowstr &c,
                   const Cowstr &d) {
    GCell c1(a);
    GCell c2(b);
    GCell c3(c);
    GCell c4(d);
    GLine line({c1, c2, c3, c4});
    lines.push_back(line);
  }
  virtual void add(const Cowstr &a, const Cowstr &b, const Cowstr &c,
                   const Cowstr &d, const Cowstr &e) {
    GCell c1(a);
    GCell c2(b);
    GCell c3(c);
    GCell c4(d);
    GCell c5(e);
    GLine line({c1, c2, c3, c4, c5});
    lines.push_back(line);
  }
  virtual void addSymbol(Symbol *sym) {
    GCell c1(identify(sym), sym->decIdentifier());
    GCell c2(sym->name());
    GCell c3(sym->type()->name());
    GCell c4(sym->kind()._to_string());
    GCell c5(sym->location().str());
    GLine line({c1, c2, c3, c4, c5});
    lines.push_back(line);
  }
  virtual void addTypeSymbol(TypeSymbol *tsym) {
    GCell c1(identify(tsym), tsym->decIdentifier());
    GCell c2(tsym->name());
    GCell c3(tsym->kind()._to_string());
    GCell c4(tsym->location().str());
    GLine line({c1, c2, c3, c4});
    lines.push_back(line);
  }
  virtual void addScope(Scope *scope) {
    GCell c1(identify(scope), scope->decIdentifier());
    GCell c2(scope->name());
    GCell c3(scope->location().str());
    GLine line({c1, c2, c3});
    lines.push_back(line);
  }
};

struct GEdgeKey {
  GEdgeKey(const Cowstr &a_key1) : key1(a_key1) {}
  GEdgeKey(const Cowstr &a_key1, const Cowstr &a_key2)
      : key1(a_key1), key2(a_key2) {}

  Cowstr key1;
  Cowstr key2;

  static Cowstr ident(const GEdgeKey &from, const GEdgeKey &to) {
    return from.str() + "-" + to.str();
  }
  Cowstr str() const { return key2.empty() ? key1 : (key1 + ":" + key2); }
};

struct GEdge {
  GEdge(GEdgeKey a_from, GEdgeKey a_to, bool a_dotted)
      : from(a_from), to(a_to), dotted(a_dotted) {}
  virtual ~GEdge() = default;

  GEdgeKey from;
  GEdgeKey to;
  std::vector<GLine> lines;
  bool dotted;

  virtual Cowstr id() const { return GEdgeKey::ident(from, to); }
  virtual Cowstr str() const {
    const std::unordered_map<Cowstr, Cowstr> &attributes = {
        {"BORDER", "\"0\""},
        {"CELLBORDER", "\"0\""},
        {"CELLSPACING", "\"0\""},
    };
    std::stringstream ss;
    ss << from.str() << " -> " << to.str();
    ss << " [";
    if (dotted) {
      ss << "style=\"dashed\" ";
    }
    ss << "label=<<TABLE";
    for (auto i = attributes.begin(); i != attributes.end(); i++) {
      ss << " " << i->first << "=" << i->second;
    }
    ss << ">\n";
    for (int i = 0; i < (int)lines.size(); i++) {
      ss << lines[i].str();
      if (i < (int)lines.size() - 1) {
        ss << "\n";
      }
    }
    ss << "</TABLE>>]";
    return ss.str();
  }
};

struct AstToNilEdge : public GEdge {
  AstToNilEdge(Ast *from, const Cowstr &to, const Cowstr &name)
      : GEdge(GEdgeKey(identify(from)), GEdgeKey(to), false) {
    add(name);
  }

  virtual void add(const Cowstr &a) {
    GCell c(a);
    GLine line({c});
    lines.push_back(line);
  }
};

struct AstToAstEdge : public GEdge {
  AstToAstEdge(Ast *from, Ast *to, const Cowstr &name)
      : GEdge(GEdgeKey(identify(from)), GEdgeKey(identify(to)), false) {
    add(name);
  }

  static Cowstr ident(Ast *from, Ast *to) {
    GEdgeKey a(identify(from));
    GEdgeKey b(identify(to));
    return GEdgeKey::ident(a, b);
  }

  virtual void add(const Cowstr &a) {
    GCell c(a);
    GLine line({c});
    lines.push_back(line);
  }
};

struct AstToScopeEdge : public GEdge {
  AstToScopeEdge(Ast *from, Scope *to)
      : GEdge(GEdgeKey(identify(from)), GEdgeKey(identify(to)), true) {}

  static Cowstr ident(Ast *from, Scope *to) {
    GEdgeKey a(identify(from));
    GEdgeKey b(identify(to));
    return GEdgeKey::ident(a, b);
  }
};

struct AstToSymbolEdge : public GEdge {
  AstToSymbolEdge(Ast *from, Symbol *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(identify(to->owner()), identify(to)), true) {}
};

struct AstToTypeSymbolEdge : public GEdge {
  AstToTypeSymbolEdge(Ast *from, TypeSymbol *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(identify(to->owner()), identify(to)), true) {}
};

struct SymbolToAstEdge : public GEdge {
  SymbolToAstEdge(Symbol *from, Ast *to)
      : GEdge(GEdgeKey(identify(from->owner()), identify(from)),
              GEdgeKey(identify(to)), false) {}
};

struct TypeSymbolToAstEdge : public GEdge {
  TypeSymbolToAstEdge(TypeSymbol *from, Ast *to)
      : GEdge(GEdgeKey(identify(from->owner()), identify(from)),
              GEdgeKey(identify(to)), false) {}
};

struct Graph {
  std::unordered_map<Cowstr, GNode *> nodes;
  std::unordered_map<Cowstr, GEdge *> edges;

  void draw(const Cowstr &fileName) {
    FileWriter fwriter(fileName);
    fwriter.writeln("digraph {");
    // fontsize: 12
    fwriter.writeln(
        "    node [shape: none, fontname: \"Courier New, Courier\"]");
    fwriter.writeln("    edge [fontname: \"Courier New, Courier\"]");
    fwriter.writeln("    graph [fontname: \"Courier New, Courier\"]");
    fwriter.writeln();
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      i->second->adjust();
    }
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      fwriter.writeln(fmt::format("    {}", i->second->str()));
    }
    fwriter.writeln();
    for (auto i = edges.begin(); i != edges.end(); i++) {
      fwriter.writeln(fmt::format("    {}", i->second->str()));
    }
    fwriter.writeln("}");
    fwriter.flush();
  }
};

// kind, id, location
#define DECL1(x)                                                               \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
  };

// kind, id, location, literal
#define DECL2(x)                                                               \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      node->add("literal", ast->name());                                       \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
  };

// kind, id, location, token
#define DECL3(x, astype, tok)                                                  \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(static_cast<astype *>(ast)->tok));                   \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
  };

// kind, id, location, child1
#define DECL4(x, astype, child1)                                               \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      Graph *g = static_cast<Context *>(context)->g;                           \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
    }                                                                          \
  };

// kind, id, location, token, child1
#define DECL5(x, astype, tok, child1)                                          \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(static_cast<astype *>(ast)->tok));                   \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      Graph *g = static_cast<Context *>(context)->g;                           \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
    }                                                                          \
  };

// kind, id, location, child1, child2
#define DECL6(x, astype, child1, child2)                                       \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      Graph *g = static_cast<Context *>(context)->g;                           \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
      linkAstToAst(ast, static_cast<astype *>(ast)->child2,                    \
                   BOOST_PP_STRINGIZE(child2), g);                             \
    }                                                                          \
  };

// kind, id, location, token, child1, child2
#define DECL7(x, astype, tok, child1, child2)                                  \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(static_cast<astype *>(ast)->tok));                   \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      Graph *g = static_cast<Context *>(context)->g;                           \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
      linkAstToAst(ast, static_cast<astype *>(ast)->child2,                    \
                   BOOST_PP_STRINGIZE(child2), g);                             \
    }                                                                          \
  };

// kind, id, location, child1, child2, child3
#define DECL8(x, astype, child1, child2, child3)                               \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      Graph *g = static_cast<Context *>(context)->g;                           \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
      linkAstToAst(ast, static_cast<astype *>(ast)->child2,                    \
                   BOOST_PP_STRINGIZE(child2), g);                             \
      linkAstToAst(ast, static_cast<astype *>(ast)->child3,                    \
                   BOOST_PP_STRINGIZE(child3), g);                             \
    }                                                                          \
  };

static void linkAstToAst(Ast *a, Ast *b, const Cowstr &name, Graph *g) {
  LOG_ASSERT(a, "Ast a is null");
  auto u = g->nodes.find(identify(a));
  LOG_ASSERT(u != g->nodes.end() && u->second,
             "Ast a {} don't has related GNode {} in graph", a->name(),
             identify(a));
  if (b) {
    auto v = g->nodes.find(identify(b));
    LOG_ASSERT(v != g->nodes.end() && v->second,
               "Ast b {} don't has related GNode {} in graph", b->name(),
               identify(b));
    Cowstr eid = AstToAstEdge::ident(a, b);
    LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
               "eid {} not exist, Ast a {} hasn't link to Ast b {}", eid,
               a->name(), b->name());
    AstToAstEdge *e = new AstToAstEdge(a, b, name);
    LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
    g->edges.insert(std::make_pair(e->id(), e));
  } else {
    NilNode *node = new NilNode();
    g->nodes.insert(std::make_pair(node->id(), node));
    AstToNilEdge *e = new AstToNilEdge(a, node->id(), name);
    g->edges.insert(std::make_pair(e->id(), e));
  }
}

static void linkAstToVar(A_VarId *varId, Graph *g) {
  S_Var *variableSymbol = static_cast<S_Var *>(varId->symbol);
  GEdgeKey from(identify(varId));
  GEdgeKey to(identify(variableSymbol->owner()), identify(variableSymbol));
  Cowstr eid = GEdgeKey::ident(from, to);
  LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
             "eid {} not exist, varId {} has no link to Variable Symbol {}",
             eid, varId->name(), variableSymbol->name());
  AstToSymbolEdge *e = new AstToSymbolEdge(varId, variableSymbol);
  LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
  g->edges.insert(std::make_pair(e->id(), e));
}

static void linkAstToParam(A_VarId *varId, Graph *g) {
  S_Param *parameterSymbol = static_cast<S_Param *>(varId->symbol);
  GEdgeKey from(identify(varId));
  GEdgeKey to(identify(parameterSymbol->owner()), identify(parameterSymbol));
  Cowstr eid = GEdgeKey::ident(from, to);
  LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
             "eid {} not exist, varId {} has no link to Parameter Symbol {}",
             eid, varId->name(), parameterSymbol->name());
  AstToSymbolEdge *e = new AstToSymbolEdge(varId, parameterSymbol);
  LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
  g->edges.insert(std::make_pair(e->id(), e));
}

static void linkAstToFunction(A_VarId *varId, Graph *g) {
  S_Func *functionSymbol = static_cast<S_Func *>(varId->symbol);
  ScopeNode *node = new ScopeNode(functionSymbol);
  node->add("symbol");
  if (functionSymbol->s_empty()) {
    node->add("empty");
  } else {
    for (auto i = functionSymbol->s_begin(); i != functionSymbol->s_end();
         i++) {
      node->addSymbol(i->second);
    }
  }
  node->add("type symbol");
  if (functionSymbol->ts_empty()) {
    node->add("empty");
  } else {
    for (auto i = functionSymbol->ts_begin(); i != functionSymbol->ts_end();
         i++) {
      node->addTypeSymbol(i->second);
    }
  }
  node->add("sub scope");
  if (functionSymbol->subscope_empty()) {
    node->add("empty");
  } else {
    for (auto i = functionSymbol->subscope_begin();
         i != functionSymbol->subscope_end(); i++) {
      node->addScope(i->second);
    }
  }
  g->nodes.insert(std::make_pair(node->id(), node));
  Cowstr eid = AstToScopeEdge::ident(varId, functionSymbol);
  LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
             "eid {} not exist, varId {} has no link to Scope {}", eid,
             varId->name(), functionSymbol->name());
  AstToScopeEdge *e = new AstToScopeEdge(varId, functionSymbol);
  LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
  g->edges.insert(std::make_pair(e->id(), e));
}

static void linkAstToClass(A_VarId *varId, Graph *g) {
  LOG_ASSERT(false, "not implemented!");
}

static void linkAstToScope(Ast *ast, Scope *scope, Graph *g) {
  ScopeNode *node = new ScopeNode(scope);
  node->add("symbol");
  if (scope->s_empty()) {
    node->add("empty");
  } else {
    for (auto i = scope->s_begin(); i != scope->s_end(); i++) {
      node->addSymbol(i->second);
    }
  }
  node->add("type symbol");
  if (scope->ts_empty()) {
    node->add("empty");
  } else {
    for (auto i = scope->ts_begin(); i != scope->ts_end(); i++) {
      node->addTypeSymbol(i->second);
    }
  }
  node->add("sub scope");
  if (scope->subscope_empty()) {
    node->add("empty");
  } else {
    for (auto i = scope->subscope_begin(); i != scope->subscope_end(); i++) {
      node->addScope(i->second);
    }
  }
  g->nodes.insert(std::make_pair(node->id(), node));
  Cowstr eid = AstToScopeEdge::ident(ast, scope);
  LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
             "eid {} not exist, ast {} has no link to Scope {}", eid,
             ast->name(), scope->name());
  AstToScopeEdge *e = new AstToScopeEdge(ast, scope);
  LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
  g->edges.insert(std::make_pair(e->id(), e));
}

struct Context : public VisitorContext {
  Context() : g(nullptr) {}
  Graph *g;
};

DECL1(Nil)
DECL1(Void)
DECL1(Break)
DECL1(Continue)

DECL2(Integer)
DECL2(Float)
DECL2(Boolean)
DECL2(Character)
DECL2(String)

struct VarId : public Visitor {
  VarId() : Visitor("Graph::VarId::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;

    // create ast gnode
    AstNode *node = new AstNode(ast);
    node->add("literal", ast->name());
    Symbol *sym = static_cast<A_VarId *>(ast)->symbol;
    TypeSymbol *tsym = static_cast<A_VarId *>(ast)->typeSymbol;
    node->add("symbol",
              sym ? fmt::format("{}:{}", sym->name(), sym->identifier())
                  : "null");
    node->add("type symbol",
              tsym ? fmt::format("{}:{}", tsym->name(), tsym->identifier())
                   : "null");
    g->nodes.insert(std::make_pair(node->id(), node));

    // create symbol/typeSymbol/scope gnode
    link(static_cast<A_VarId *>(ast), g);
  }
  void link(A_VarId *varId, Graph *g) {
    if (varId->symbol) {
      switch (varId->symbol->kind()) {
      case SymbolKind::Var: {
        if (defineSymbol(varId)) {
          linkAstToVar(varId, g);
        }
        break;
      }
      case SymbolKind::Param: {
        if (defineSymbol(varId)) {
          linkAstToParam(varId, g);
        }
        break;
      }
      case SymbolKind::Func: {
        if (defineSymbol(varId)) {
          linkAstToFunction(varId, g);
        }
        break;
      }
      default:
        LOG_ASSERT(false, "invalid symbol {} kind: {}", varId->symbol->name(),
                   varId->symbol->kind()._to_string());
      }
    } else {
      LOG_ASSERT(
          varId->typeSymbol,
          "varId->typeSymbol is null while varId->symbol already null:{}",
          varId->name());
      switch (varId->typeSymbol->kind()) {
      case TypeSymbolKind::Class: {
        if (defineTypeSymbol(varId)) {
          linkAstToClass(varId, g);
        }
        break;
      }
      default:
        // TypeSymbolKind::Plain
        // TypeSymbolKind::Func
        LOG_ASSERT(false, "invalid type symbol {} kind: {}",
                   varId->typeSymbol->name(),
                   varId->typeSymbol->kind()._to_string());
      }
    }
  }
  bool defineSymbol(A_VarId *varId) {
    return varId->symbol && varId->symbol->ast() == varId;
  }
  bool defineTypeSymbol(A_VarId *varId) {
    return varId->typeSymbol && varId->typeSymbol->ast() == varId;
  }
};

DECL3(PlainType, A_PlainType, token)

DECL4(Throw, A_Throw, expr)
DECL4(Return, A_Return, expr)
DECL4(Yield, A_Yield, expr)

struct Block : public Visitor {
  Block() : Visitor("Graph::Block::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;
    AstNode *node = new AstNode(ast);
    g->nodes.insert(std::make_pair(node->id(), node));

    // create local scope
    A_Block *block = static_cast<A_Block *>(ast);
    LOG_ASSERT(block->localScope, "block {} localScope must not null",
               block->name());
    linkAstToScope(block, block->localScope, g);
  }
  virtual void postVisit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;
    linkAstToAst(ast, static_cast<A_Block *>(ast)->blockStats,
                 BOOST_PP_STRINGIZE(blockStats), g);
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("Graph::CompileUnit::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;
    AstNode *node = new AstNode(ast);
    g->nodes.insert(std::make_pair(node->id(), node));

    // create global scope
    A_CompileUnit *compileUnit = static_cast<A_CompileUnit *>(ast);
    LOG_ASSERT(compileUnit->globalScope,
               "compile unit {} globalScope must not null",
               compileUnit->name());
    linkAstToScope(compileUnit, compileUnit->globalScope, g);
  }
  virtual void postVisit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;
    linkAstToAst(ast, static_cast<A_CompileUnit *>(ast)->topStats,
                 BOOST_PP_STRINGIZE(topStats), g);
  }
};

DECL5(PostfixExpr, A_PostfixExpr, postfixOp, expr)
DECL5(PrefixExpr, A_PrefixExpr, prefixOp, expr)

DECL6(Call, A_Call, id, args)
DECL6(Exprs, A_Exprs, expr, next)

struct Loop : public Visitor {
  Loop() : Visitor("Graph::Loop::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;
    AstNode *node = new AstNode(ast);
    g->nodes.insert(std::make_pair(node->id(), node));

    // create local scope
    A_Loop *loop = static_cast<A_Loop *>(ast);
    LOG_ASSERT(loop->localScope, "loop {} localScope must not null",
               loop->name());
    linkAstToScope(loop, loop->localScope, g);
  }
  virtual void postVisit(Ast *ast, VisitorContext *context) {
    Graph *g = static_cast<Context *>(context)->g;
    linkAstToAst(ast, static_cast<A_Loop *>(ast)->condition,
                 BOOST_PP_STRINGIZE(condition), g);
    linkAstToAst(ast, static_cast<A_Loop *>(ast)->body,
                 BOOST_PP_STRINGIZE(body), g);
  }
};

DECL6(DoWhile, A_DoWhile, body, condition)
DECL6(BlockStats, A_BlockStats, blockStat, next)
DECL6(FuncSign, A_FuncSign, id, params)
DECL6(Params, A_Params, param, next)
DECL6(Param, A_Param, id, type)
DECL6(TopStats, A_TopStats, topStat, next)

DECL7(Assign, A_Assign, assignOp, assignee, assignor)
DECL7(InfixExpr, A_InfixExpr, infixOp, left, right)

DECL8(If, A_If, condition, thenp, elsep)
DECL8(LoopCondition, A_LoopCondition, init, condition, update)
DECL8(LoopEnumerator, A_LoopEnumerator, id, type, expr)
DECL8(Try, A_Try, tryp, catchp, finallyp)
DECL8(FuncDef, A_FuncDef, funcSign, resultType, body)
DECL8(VarDef, A_VarDef, id, type, expr)

} // namespace graph

} // namespace detail

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::graph::x();                                       \
    binder_.bind((+AstKind::x)._to_integral(), v);                             \
    visitors_.push_back(v);                                                    \
  } while (0)

Drawer::Drawer(const Cowstr &output)
    : Phase("Drawer"), fileName_(output),
      context_(new detail::graph::Context()), binder_(context_) {
  BIND(Nil);
  BIND(Void);
  BIND(Break);
  BIND(Continue);

  BIND(Integer);
  BIND(Float);
  BIND(Boolean);
  BIND(Character);
  BIND(String);

  BIND(VarId);

  BIND(PlainType);

  BIND(Throw);
  BIND(Return);
  BIND(Yield);
  BIND(Block);
  BIND(CompileUnit);

  BIND(PostfixExpr);
  BIND(PrefixExpr);

  BIND(Call);
  BIND(Exprs);

  BIND(Loop);
  BIND(DoWhile);
  BIND(BlockStats);
  BIND(FuncSign);
  BIND(Params);
  BIND(Param);
  BIND(TopStats);

  BIND(Assign);
  BIND(InfixExpr);

  BIND(If);
  BIND(LoopCondition);
  BIND(LoopEnumerator);
  BIND(Try);
  BIND(FuncDef);
  BIND(VarDef);
}

Drawer::~Drawer() {
  delete context_;
  context_ = nullptr;
  for (int i = 0; i < (int)visitors_.size(); i++) {
    LOG_ASSERT(visitors_[i], "visitors_[{}] must not null", i);
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
  visitors_.clear();
}

void Drawer::run(Ast *ast) {
  detail::graph::Graph g;
  static_cast<detail::graph::Context *>(context_)->g = &g;
  Visitor::traverse(&binder_, ast);
  g.draw(fileName_);
}