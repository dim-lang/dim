// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "NameGenerator.h"
#include "Symbol.h"
#include "Token.h"
#include "Visitor.h"
#include "boost/preprocessor/stringize.hpp"
#include "infra/Counter.h"
#include "infra/Files.h"
#include "infra/LinkedHashMap.h"
#include "infra/LinkedHashMap.hpp"
#include "infra/Log.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace detail {

namespace drawer {

// Graph {

static CounterNameGenerator CounterNG;

static Cowstr identify(Identifiable *identifier) {
  return Cowstr("id") + identifier->decIdentifier();
}

/**
 * <TD PORT="id"> value </TD>
 * <TD PORT="id" COLSPAN="width" ALIGN="LEFT" BALIGN="LEFT"> value </TD>
 */
struct GCell {
  Cowstr id; // PORT="id"
  Cowstr value;
  int width; // COLSPAN="width"
  Cowstr align;

  GCell(const Cowstr &a_value)
      : id(CounterNG.generate("cell")), value(translate(a_value)), width(0),
        align("") {}
  GCell(const Cowstr &a_id, const Cowstr &a_value)
      : id(a_id), value(translate(a_value)), width(0), align("") {}

  Cowstr str() const {
    std::stringstream ss;
    ss << "<TD PORT=\"" << id << "\"";
    if (width) {
      ss << " COLSPAN=\"" << width << "\"";
    }
    if (!align.empty()) {
      ss << " ALIGN=\"" << align << "\" BALIGN=\"" << align << "\"";
    }
    ss << ">" << value << "</TD>";
    return ss.str();
  }

  static Cowstr translate(const Cowstr &value) {
    static const std::unordered_map<char, Cowstr> htmlTranslator = {
        {'\\', "\\\\"},
        {'\n', "<BR ALIGN=\"LEFT\"/>"},
        {'\t', "&#9;"},
        {'\r', "\\r"},
        // {'\?', "\\?"},
        {'\a', "\\a"},
        {'\b', "\\b"},
        {'\f', "\\f"},
        {'\v', "\\v"},
        {'&', "&#38;"},
        // {'\'', "\\'"},
        // {'\"', "\\\""},
        {'<', "&lt;"},
        {'>', "&gt;"},
        {'|', "&#124;"},
    };
    return Cowstr(value).replace(htmlTranslator);
  }
};

/**
 * <TR> GCell+ </TR>
 */
struct GLine {
  std::vector<GCell> cells;

  GLine(const std::vector<GCell> &a_cells) : cells(a_cells) {}

  Cowstr str() const {
    std::stringstream ss;
    ss << "<TR>";
    for (int i = 0; i < (int)cells.size(); i++) {
      ss << cells[i].str();
    }
    ss << "</TR>";
    return ss.str();
  }
};

/**
 * (node)id [label=<<TABLE> GLine+ </TABLE>>]
 */
struct GNode {
  Cowstr ident;
  std::vector<GLine> lines;

  GNode() : ident(CounterNG.generate("node")) {}
  GNode(const Cowstr &a_ident) : ident(a_ident) {}
  virtual ~GNode() = default;

  virtual Cowstr str() const {
    std::stringstream ss;
    ss << ident
       << " [label=<<TABLE CELLBORDER=\"1\" CELLSPACING=\"0\" BORDER=\"0\">\n";
    for (int i = 0; i < (int)lines.size(); i++) {
      ss << "        " << lines[i].str() << "\n";
    }
    ss << "    </TABLE>>]";
    return ss.str();
  }

  virtual Cowstr id() const { return ident; }

  virtual void adjust() {
    if (lines.empty()) {
      return;
    }
    // adjust for max width
    int maxWidth = 0;
    for (int i = 0; i < (int)lines.size(); i++) {
      maxWidth = std::max<int>(lines[i].cells.size(), maxWidth);
    }
    for (int i = 0; i < (int)lines.size(); i++) {
      if (lines[i].cells.size() > 0 && lines[i].cells.size() < maxWidth) {
        lines[i].cells[0].width = maxWidth - lines[i].cells.size() + 1;
      }
    }
    // adjust for string literal left align
    for (int i = 0; i < (int)lines.size(); i++) {
      for (int j = 0; j < (int)lines[i].cells.size(); j++) {
        if (lines[i].cells[j].value.startWith('"')) {
          lines[i].cells[j].align = "LEFT";
        }
      }
    }
  }
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
    GCell c3(" ");
    GCell c4(tsym->kind()._to_string());
    GCell c5(tsym->location().str());
    GLine line({c1, c2, c3, c4, c5});
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
  Cowstr key1;
  Cowstr key2;

  GEdgeKey(const Cowstr &a_key1) : key1(a_key1) {}
  GEdgeKey(const Cowstr &a_key1, const Cowstr &a_key2)
      : key1(a_key1), key2(a_key2) {}

  static Cowstr ident(const GEdgeKey &from, const GEdgeKey &to) {
    return from.str() + "-" + to.str();
  }

  bool hasCell() const { return !key2.empty(); }

  Cowstr str() const { return key2.empty() ? key1 : (key1 + ":" + key2); }
};

struct GEdge {
  GEdgeKey from;
  GEdgeKey to;
  std::vector<GLine> lines;
  Cowstr style;
  Cowstr direction;

  GEdge(GEdgeKey a_from, GEdgeKey a_to, const Cowstr &a_style = "",
        const Cowstr &a_direction = "")
      : from(a_from), to(a_to), style(a_style), direction(a_direction) {}
  virtual ~GEdge() = default;

  virtual Cowstr id() const { return GEdgeKey::ident(from, to); }
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << from.str() << "->" << to.str();
    if (!direction.empty()) {
      ss << direction;
    }
    if (style.empty() && lines.empty()) {
      return ss.str();
    }
    ss << " [";
    if (!style.empty()) {
      ss << "style=" << style;
    }
    if (lines.size() > 0) {
      ss << " label=<<TABLE CELLBORDER=\"0\" CELLSPACING=\"0\" BORDER=\"0\">";
      for (int i = 0; i < (int)lines.size(); i++) {
        ss << lines[i].str();
      }
      ss << "</TABLE>>";
    }
    ss << "]";
    return ss.str();
  }
  virtual void add(const Cowstr &a) {
    GCell c(a);
    GLine line({c});
    lines.push_back(line);
  }
};

struct AstToNilEdge : public GEdge {
  AstToNilEdge(Ast *from, const Cowstr &to, const Cowstr &name)
      : GEdge(GEdgeKey(identify(from)), GEdgeKey(to)) {
    add(name);
  }
};

struct AstToAstEdge : public GEdge {
  AstToAstEdge(Ast *from, Ast *to, const Cowstr &name)
      : GEdge(GEdgeKey(identify(from)), GEdgeKey(identify(to))) {
    add(name);
  }

  static Cowstr ident(Ast *from, Ast *to) {
    GEdgeKey a(identify(from));
    GEdgeKey b(identify(to));
    return GEdgeKey::ident(a, b);
  }
};

// define edge

struct AstToSymbolEdge : public GEdge {
  AstToSymbolEdge(Ast *from, Symbol *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(identify(to->owner()), identify(to)), "dashed") {
    add("#");
  }
};

struct AstToTypeSymbolEdge : public GEdge {
  AstToTypeSymbolEdge(Ast *from, TypeSymbol *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(identify(to->owner()), identify(to)), "dashed") {
    add("#");
  }
};

struct AstToScopeEdge : public GEdge {
  AstToScopeEdge(Ast *from, ScopeNode *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(to->id(), to->lines[0].cells[0].id), "dashed") {
    add("#");
  }
};

// resolve edge

struct SymbolToAstEdge : public GEdge {
  SymbolToAstEdge(Ast *from, Symbol *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(identify(to->owner()), identify(to)), "dotted") {
    add("&");
  }
};

struct TypeSymbolToAstEdge : public GEdge {
  TypeSymbolToAstEdge(TypeSymbol *from, Ast *to)
      : GEdge(GEdgeKey(identify(from->owner()), identify(from)),
              GEdgeKey(identify(to)), "dotted") {
    add("&");
  }
};

struct ScopeToAstEdge : public GEdge {
  ScopeToAstEdge(Ast *from, ScopeNode *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(to->id(), to->lines[0].cells[0].id), "dotted") {
    add("&");
  }
};

struct GEdgeRank {
  Cowstr rank;

  GEdgeRank(GNode *a, GNode *b) {
    rank =
        fmt::format("{{rank=same {} -> {} [style=invis]}}", a->id(), b->id());
  }
  GEdgeRank(GNode *a, GNode *b, GNode *c) {
    rank = fmt::format("{{rank=same {} -> {} -> {} [style=invis]}}", a->id(),
                       b->id(), c->id());
  }
  virtual ~GEdgeRank() = default;

  virtual Cowstr str() const { return rank; }
};

struct Graph {
  LinkedHashMap<Cowstr, GNode *> nodes;
  LinkedHashMap<Cowstr, GEdge *> edges;
  std::vector<GEdgeRank *> ranks;

  Graph() {}
  ~Graph() {
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      delete i->second;
      i->second = nullptr;
    }
    nodes.clear();
    for (auto i = edges.begin(); i != edges.end(); i++) {
      delete i->second;
      i->second = nullptr;
    }
    edges.clear();
    for (int i = 0; i < (int)ranks.size(); i++) {
      delete ranks[i];
      ranks[i] = nullptr;
    }
    ranks.clear();
  }

  void draw(const Cowstr &fileName) {
    FileWriter fwriter(fileName);
    fwriter.writeln("digraph {");
    fwriter.writeln("    node [shape=none, fontname=\"Courier New, Courier\"]");
    fwriter.writeln("    edge [fontname=\"Courier New, Courier\"]");
    fwriter.writeln("    graph [fontname=\"Courier New, Courier\"]");
    fwriter.writeln();
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      LOG_ASSERT(i->second, "nodes i->second must not null");
      i->second->adjust();
    }
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      LOG_ASSERT(i->second, "nodes i->second must not null");
      fwriter.writeln(fmt::format("    {}", i->second->str()));
    }
    fwriter.writeln();
    for (auto i = edges.begin(); i != edges.end(); i++) {
      LOG_ASSERT(i->second, "edges i->second must not null");
      fwriter.writeln(fmt::format("    {}", i->second->str()));
    }
    fwriter.writeln();
    for (int i = 0; i < (int)ranks.size(); i++) {
      LOG_ASSERT(ranks[i], "ranks[{}] must not null", i);
      fwriter.writeln(fmt::format("    {}", ranks[i]->str()));
    }
    fwriter.writeln("}");
    fwriter.flush();
  }
};

// Graph }

// Visitor {

struct Context : public VisitorContext {
  Context() : g(nullptr) {}
  Graph *g;
};

// ast to ast links {

static GNode *linkAstToAst(Ast *a, Ast *b, const Cowstr &name, Graph *g) {
  LOG_ASSERT(a, "a must not null");
  if (b) {
    AstToAstEdge *edge = new AstToAstEdge(a, b, name);
    g->edges.insert(edge->id(), edge);
    LinkedHashMap<Cowstr, GNode *>::iterator it = g->nodes.find(identify(b));
    LOG_ASSERT(it != g->nodes.end(), "b {} cannot find in nodes", b->name());
    LOG_ASSERT(it->second, "b {} related GNode cannot find in nodes",
               b->name());
    return it->second;
  } else {
    NilNode *node = new NilNode();
    g->nodes.insert(node->id(), node);
    AstToNilEdge *edge = new AstToNilEdge(a, node->id(), name);
    g->edges.insert(edge->id(), edge);
    return node;
  }
}

static void rankAstNode(GNode *a, GNode *b, Graph *g) {
  LOG_ASSERT(a, "a must not null");
  LOG_ASSERT(b, "b must not null");
  LOG_ASSERT(g, "g must not null");
  GEdgeRank *r = new GEdgeRank(a, b);
  g->ranks.push_back(r);
}

static void rankAstNode(GNode *a, GNode *b, GNode *c, Graph *g) {
  LOG_ASSERT(a, "a must not null");
  LOG_ASSERT(b, "b must not null");
  LOG_ASSERT(c, "c must not null");
  LOG_ASSERT(g, "g must not null");
  GEdgeRank *r = new GEdgeRank(a, b, c);
  g->ranks.push_back(r);
}

// ast to ast links }

// ast and symbol links {

static void defineAstToVar(A_VarId *varId, Graph *g) {
  S_Var *s_var = static_cast<S_Var *>(varId->symbol());
  AstToSymbolEdge *edge = new AstToSymbolEdge(varId, s_var);
  g->edges.insert(edge->id(), edge);
}

static void resolveAstToVar(A_VarId *varId, Graph *g) {
  S_Var *s_var = static_cast<S_Var *>(varId->symbol());
  SymbolToAstEdge *edge = new SymbolToAstEdge(varId, s_var);
  g->edges.insert(edge->id(), edge);
}

static void defineAstToParam(A_VarId *varId, Graph *g) {
  S_Param *s_param = static_cast<S_Param *>(varId->symbol());
  AstToSymbolEdge *edge = new AstToSymbolEdge(varId, s_param);
  g->edges.insert(edge->id(), edge);
}

static void resolveAstToParam(A_VarId *varId, Graph *g) {
  S_Param *s_param = static_cast<S_Param *>(varId->symbol());
  SymbolToAstEdge *edge = new SymbolToAstEdge(varId, s_param);
  g->edges.insert(edge->id(), edge);
}

static void defineAstToClass(A_VarId *varId, Graph *g) {
  LOG_ASSERT(false, "not implemented!");
}

static void resolveAstToClass(A_VarId *varId, Graph *g) {
  LOG_ASSERT(false, "not implemented!");
}

static void defineAstToScope(Ast *varId, Scope *scope, Graph *g) {
  ScopeNode *node = new ScopeNode(scope);
  if (scope->s_empty() && scope->ts_empty() && scope->subscope_empty()) {
    node->add("empty");
  } else {
    if (!scope->s_empty()) {
      node->add("symbol");
      for (auto i = scope->s_begin(); i != scope->s_end(); i++) {
        node->addSymbol(i->second);
      }
    }
    if (!scope->ts_empty()) {
      node->add("type_symbol");
      for (auto i = scope->ts_begin(); i != scope->ts_end(); i++) {
        node->addTypeSymbol(i->second);
      }
    }
    if (!scope->subscope_empty()) {
      node->add("sub_scope");
      for (auto i = scope->subscope_begin(); i != scope->subscope_end(); i++) {
        node->addScope(i->second);
      }
    }
  }
  g->nodes.insert(node->id(), node);
  AstToScopeEdge *edge = new AstToScopeEdge(varId, node);
  g->edges.insert(edge->id(), edge);
}

static void resolveAstToScope(Ast *varId, Scope *scope, Graph *g) {
  auto it = g->nodes.find(identify(scope));
  LOG_ASSERT(it != g->nodes.end(), "scope {} related ScopeNode not exist",
             scope->name());
  LOG_ASSERT(it->second, "scope {} related ScopeNode must not null",
             scope->name());
  GNode *node = it->second;
  ScopeToAstEdge *edge =
      new ScopeToAstEdge(varId, static_cast<ScopeNode *>(node));
  g->edges.insert(edge->id(), edge);
}

// symbol and ast links }

// kind, id, location
#define DECL1(x)                                                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
  }

// kind, id, location, literal
#define DECL2(x)                                                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)                                                                 \
    () : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}                 \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      node->add("literal", ast->name());                                       \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
  }

// kind, id, location, token
#define DECL3(x, astype, tok)                                                  \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(static_cast<astype *>(ast)->tok));                   \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
  }

// kind, id, location, child1
#define DECL4(x, astype, child1)                                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
    virtual void finishVisit(Ast *ast) {                                       \
      Graph *g = static_cast<Context *>(context())->g;                         \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
    }                                                                          \
  }

// kind, id, location, token, child1
#define DECL5(x, astype, tok, child1)                                          \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(static_cast<astype *>(ast)->tok));                   \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
    virtual void finishVisit(Ast *ast) {                                       \
      Graph *g = static_cast<Context *>(context())->g;                         \
      linkAstToAst(ast, static_cast<astype *>(ast)->child1,                    \
                   BOOST_PP_STRINGIZE(child1), g);                             \
    }                                                                          \
  }

// kind, id, location, child1, child2
#define DECL6(x, astype, child1, child2)                                       \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
    virtual void finishVisit(Ast *ast) {                                       \
      Graph *g = static_cast<Context *>(context())->g;                         \
      GNode *a1 = linkAstToAst(ast, static_cast<astype *>(ast)->child1,        \
                               BOOST_PP_STRINGIZE(child1), g);                 \
      GNode *a2 = linkAstToAst(ast, static_cast<astype *>(ast)->child2,        \
                               BOOST_PP_STRINGIZE(child2), g);                 \
      rankAstNode(a1, a2, g);                                                  \
    }                                                                          \
  }

// kind, id, location, token, child1, child2
#define DECL7(x, astype, tok, child1, child2)                                  \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(static_cast<astype *>(ast)->tok));                   \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
    virtual void finishVisit(Ast *ast) {                                       \
      Graph *g = static_cast<Context *>(context())->g;                         \
      GNode *a1 = linkAstToAst(ast, static_cast<astype *>(ast)->child1,        \
                               BOOST_PP_STRINGIZE(child1), g);                 \
      GNode *a2 = linkAstToAst(ast, static_cast<astype *>(ast)->child2,        \
                               BOOST_PP_STRINGIZE(child2), g);                 \
      rankAstNode(a1, a2, g);                                                  \
    }                                                                          \
  }

// kind, id, location, child1, child2, child3
#define DECL8(x, astype, child1, child2, child3)                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      Graph *g = static_cast<Context *>(context())->g;                         \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(node->id(), node);                                       \
    }                                                                          \
    virtual void finishVisit(Ast *ast) {                                       \
      Graph *g = static_cast<Context *>(context())->g;                         \
      GNode *a1 = linkAstToAst(ast, static_cast<astype *>(ast)->child1,        \
                               BOOST_PP_STRINGIZE(child1), g);                 \
      GNode *a2 = linkAstToAst(ast, static_cast<astype *>(ast)->child2,        \
                               BOOST_PP_STRINGIZE(child2), g);                 \
      GNode *a3 = linkAstToAst(ast, static_cast<astype *>(ast)->child3,        \
                               BOOST_PP_STRINGIZE(child3), g);                 \
      rankAstNode(a1, a2, a3, g);                                              \
    }                                                                          \
  }

DECL1(Nil);
DECL1(Void);
DECL1(Break);
DECL1(Continue);

DECL2(Integer);
DECL2(Float);
DECL2(Boolean);
DECL2(Character);
DECL2(String);

struct VISITOR(VarId) : public Visitor {
  VISITOR(VarId)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(VarId))) {}
  virtual void visit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;

    // create ast gnode
    AstNode *node = new AstNode(ast);
    node->add("literal", ast->name());
    g->nodes.insert(node->id(), node);

    // create symbol gnode
    link(static_cast<A_VarId *>(ast), g);
  }
  void link(A_VarId *ast, Graph *g) {
    if (ast->symbol()) {
      switch (ast->symbol()->kind()) {
      case SymbolKind::Var: {
        if (Symbol::isDefined(ast)) {
          defineAstToVar(ast, g);
        } else {
          resolveAstToVar(ast, g);
        }
        break;
      }
      case SymbolKind::Param: {
        if (Symbol::isDefined(ast)) {
          defineAstToParam(ast, g);
        } else {
          resolveAstToParam(ast, g);
        }
        break;
      }
      case SymbolKind::Func: {
        if (Symbol::isDefined(ast)) {
          defineAstToScope(ast, static_cast<S_Func *>(ast->symbol()), g);
        } else {
          resolveAstToScope(ast, static_cast<S_Func *>(ast->symbol()), g);
        }
        break;
      }
      default:
        LOG_ASSERT(false, "invalid symbol {}:{} kind: {}",
                   ast->symbol()->name(), ast->symbol()->location(),
                   ast->symbol()->kind()._to_string());
      }
    } else if (ast->typeSymbol()) {
      switch (ast->typeSymbol()->kind()) {
      case TypeSymbolKind::Class: {
        if (TypeSymbol::isDefined(ast)) {
          defineAstToClass(ast, g);
        } else {
          resolveAstToClass(ast, g);
        }
        break;
      }
      default:
        // TypeSymbolKind::Plain
        // TypeSymbolKind::Func
        LOG_ASSERT(false, "invalid type symbol {}:{} kind: {}",
                   ast->typeSymbol()->name(), ast->typeSymbol()->location(),
                   ast->typeSymbol()->kind()._to_string());
      }
    } else {
      LOG_ASSERT(false, "invalid ast {}:{}", ast->name(), ast->location());
    }
  }
};

DECL3(PlainType, A_PlainType, token);

DECL4(Throw, A_Throw, expr);
DECL4(Return, A_Return, expr);
DECL4(Yield, A_Yield, expr);

struct VISITOR(Block) : public Visitor {
  VISITOR(Block)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(Block))) {}
  virtual void visit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;
    AstNode *node = new AstNode(ast);
    g->nodes.insert(node->id(), node);

    // create local scope
    A_Block *block = static_cast<A_Block *>(ast);
    defineAstToScope(block, block->scope(), g);
  }
  virtual void finishVisit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;
    linkAstToAst(ast, static_cast<A_Block *>(ast)->blockStats,
                 BOOST_PP_STRINGIZE(blockStats), g);
  }
};

struct VISITOR(CompileUnit) : public Visitor {
  VISITOR(CompileUnit)
  () : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(CompileUnit))) {}
  virtual void visit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;
    AstNode *node = new AstNode(ast);
    g->nodes.insert(node->id(), node);

    // global scope
    A_CompileUnit *compileUnit = static_cast<A_CompileUnit *>(ast);
    defineAstToScope(compileUnit, compileUnit->scope(), g);
  }
  virtual void finishVisit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;
    linkAstToAst(ast, static_cast<A_CompileUnit *>(ast)->topStats,
                 BOOST_PP_STRINGIZE(topStats), g);
  }
};

DECL5(Postfix, A_Postfix, postfixOp, expr);
DECL5(Prefix, A_Prefix, prefixOp, expr);

DECL6(Call, A_Call, id, args);
DECL6(Exprs, A_Exprs, expr, next);
DECL6(DoWhile, A_DoWhile, body, condition);
DECL6(BlockStats, A_BlockStats, blockStat, next);
DECL6(FuncSign, A_FuncSign, id, params);
DECL6(Params, A_Params, param, next);
DECL6(Param, A_Param, id, type);
DECL6(TopStats, A_TopStats, topStat, next);

struct VISITOR(Loop) : public Visitor {
  VISITOR(Loop)() : Visitor("Drawer::" BOOST_PP_STRINGIZE(VISITOR(Loop))) {}
  virtual void visit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;
    AstNode *node = new AstNode(ast);
    g->nodes.insert(node->id(), node);

    // create local scope
    A_Loop *loop = static_cast<A_Loop *>(ast);
    LOG_ASSERT(loop->scope(), "loop's {} scope must not null", loop->name());
    defineAstToScope(loop, loop->scope(), g);
  }
  virtual void finishVisit(Ast *ast) {
    Graph *g = static_cast<Context *>(context())->g;
    linkAstToAst(ast, static_cast<A_Loop *>(ast)->condition,
                 BOOST_PP_STRINGIZE(condition), g);
    linkAstToAst(ast, static_cast<A_Loop *>(ast)->body,
                 BOOST_PP_STRINGIZE(body), g);
  }
};

DECL7(Assign, A_Assign, assignOp, assignee, assignor);
DECL7(Infix, A_Infix, infixOp, left, right);

DECL8(If, A_If, condition, thenp, elsep);
DECL8(LoopCondition, A_LoopCondition, init, condition, update);
DECL8(LoopEnumerator, A_LoopEnumerator, id, type, expr);
DECL8(Try, A_Try, tryp, catchp, finallyp);
DECL8(FuncDef, A_FuncDef, funcSign, resultType, body);
DECL8(VarDef, A_VarDef, id, type, expr);

// Visitor }

} // namespace drawer

} // namespace detail

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::drawer::VISITOR(x)();                             \
    binder_.bind((+AstKind::x), v);                                            \
    visitors_.push_back(v);                                                    \
  } while (0)

Drawer::Drawer(const Cowstr &fileName)
    : Phase("Drawer"), fileName_(fileName),
      context_(new detail::drawer::Context()), binder_(context_) {
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

  BIND(Postfix);
  BIND(Prefix);

  BIND(Call);
  BIND(Exprs);
  BIND(DoWhile);
  BIND(BlockStats);
  BIND(FuncSign);
  BIND(Params);
  BIND(Param);
  BIND(TopStats);

  BIND(Loop);

  BIND(Assign);
  BIND(Infix);

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
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
  visitors_.clear();
}

void Drawer::run(Ast *ast) {
  LOG_ASSERT(!fileName_.empty(), "fileName_ must not empty");
  detail::drawer::Graph g;
  static_cast<detail::drawer::Context *>(context_)->g = &g;
  Visitor::traverse(&binder_, ast);
  g.draw(fileName_);
}

const Cowstr &Drawer::fileName() const { return fileName_; }

Cowstr &Drawer::fileName() { return fileName_; }
