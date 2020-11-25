// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "NameGenerator.h"
#include "Symbol.h"
#include "Token.h"
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
    add(fmt::format("{}@{}", ast->kind()._to_string(), ast->identifier()));
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
    add(fmt::format("{}@{}", scope->name(), scope->identifier()));
    switch (scope->sc_kind()) {
    case ScopeKind::Symbol:
      add("symbol", dynamic_cast<Symbol *>(scope)->kind()._to_string());
      break;
    case ScopeKind::TypeSymbol:
      add("type symbol",
          dynamic_cast<TypeSymbol *>(scope)->kind()._to_string());
      break;
    default:
      add("scope", scope->sc_kind()._to_string());
      break;
    }
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
    GCell c1(fmt::format("{}@{}", sym->name(), sym->identifier()));
    GCell c2(sym->type()->name());
    GCell c3(sym->kind()._to_string());
    GCell c4(sym->location().str());
    GLine line({c1, c2, c3, c4});
    lines.push_back(line);
  }
  virtual void addTypeSymbol(TypeSymbol *tsym) {
    GCell c1(fmt::format("{}@{}", tsym->name(), tsym->identifier()));
    GCell c2(" ");
    GCell c3(tsym->kind()._to_string());
    GCell c4(tsym->location().str());
    GLine line({c1, c2, c3, c4});
    lines.push_back(line);
  }
  virtual void addScope(Scope *scope) {
    GCell c1(fmt::format("{}@{}", scope->name(), scope->identifier()));
    GCell c2(scope->location().str());
    GLine line({c1, c2});
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
              GEdgeKey(identify(to->owner()), identify(to)), "dashed") {}
};

struct AstToTypeSymbolEdge : public GEdge {
  AstToTypeSymbolEdge(Ast *from, TypeSymbol *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(identify(to->owner()), identify(to)), "dashed") {}
};

struct AstToScopeEdge : public GEdge {
  AstToScopeEdge(Ast *from, ScopeNode *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(to->id(), to->lines[0].cells[0].id), "dashed") {}
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

static void linkAstToClass(A_VarId *varId, Graph *g) {
  LOG_ASSERT(false, "not implemented!");
}

static void linkAstToScope(Ast *varId, Scope *scope, Graph *g) {
  ScopeNode *node = new ScopeNode(scope);
  if (scope->s_empty() && scope->ts_empty() && scope->sc_empty()) {
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
    if (!scope->sc_empty()) {
      node->add("sub_scope");
      for (auto i = scope->sc_begin(); i != scope->sc_end(); i++) {
        node->addScope(i->second);
      }
    }
  }
  g->nodes.insert(node->id(), node);
  AstToScopeEdge *edge = new AstToScopeEdge(varId, node);
  g->edges.insert(edge->id(), edge);
}

// symbol and ast links }

} // namespace drawer

} // namespace detail

Drawer::Drawer(const Cowstr &fileName)
    : Phase("Drawer"), fileName_(fileName), g_(new detail::drawer::Graph()) {}

void Drawer::run(Ast *ast) {
  LOG_ASSERT(!fileName_.empty(), "fileName_ must not empty");
  ast->accept(this);
  g_->draw(fileName_);
}

const Cowstr &Drawer::fileName() const { return fileName_; }

Cowstr &Drawer::fileName() { return fileName_; }

static void emptyImpl(Ast *ast, detail::drawer::Graph *g) {
  detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);
  g->nodes.insert(node->id(), node);
}

static void literalImpl(Ast *ast, detail::drawer::Graph *g) {
  detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);
  node->add("literal", ast->name());
  g->nodes.insert(node->id(), node);
}

#define VISIT_TOKEN(ast, g, token)                                             \
  do {                                                                         \
    detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);          \
    node->add(BOOST_PP_STRINGIZE(token), tokenName(ast->token));               \
    g->nodes.insert(node->id(), node);                                         \
  } while (0)

#define VISIT_CHILD1(ast, g, child1)                                           \
  do {                                                                         \
    detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);          \
    g->nodes.insert(node->id(), node);                                         \
    if (ast->child1) {                                                         \
      ast->child1->accept(this);                                               \
    }                                                                          \
    detail::drawer::linkAstToAst(ast, ast->child1,                             \
                                 BOOST_PP_STRINGIZE(child1), g);               \
  } while (0)

#define VISIT_TOKEN_CHILD1(ast, g, token, child1)                              \
  do {                                                                         \
    detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);          \
    node->add(BOOST_PP_STRINGIZE(token), tokenName(ast->token));               \
    g->nodes.insert(node->id(), node);                                         \
    if (ast->child1) {                                                         \
      ast->child1->accept(this);                                               \
    }                                                                          \
    detail::drawer::linkAstToAst(ast, ast->child1,                             \
                                 BOOST_PP_STRINGIZE(child1), g);               \
  } while (0)

#define VISIT_CHILD2(ast, g, child1, child2)                                   \
  do {                                                                         \
    detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);          \
    g->nodes.insert(node->id(), node);                                         \
    if (ast->child1) {                                                         \
      ast->child1->accept(this);                                               \
    }                                                                          \
    if (ast->child2) {                                                         \
      ast->child2->accept(this);                                               \
    }                                                                          \
    detail::drawer::GNode *a1 = detail::drawer::linkAstToAst(                  \
        ast, ast->child1, BOOST_PP_STRINGIZE(child1), g);                      \
    detail::drawer::GNode *a2 = detail::drawer::linkAstToAst(                  \
        ast, ast->child2, BOOST_PP_STRINGIZE(child2), g);                      \
    detail::drawer::rankAstNode(a1, a2, g_);                                   \
  } while (0)

#define VISIT_TOKEN_CHILD2(ast, g, token, child1, child2)                      \
  do {                                                                         \
    detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);          \
    node->add(BOOST_PP_STRINGIZE(token), tokenName(ast->token));               \
    g->nodes.insert(node->id(), node);                                         \
    if (ast->child1) {                                                         \
      ast->child1->accept(this);                                               \
    }                                                                          \
    if (ast->child2) {                                                         \
      ast->child2->accept(this);                                               \
    }                                                                          \
    detail::drawer::GNode *a1 = detail::drawer::linkAstToAst(                  \
        ast, ast->child1, BOOST_PP_STRINGIZE(child1), g);                      \
    detail::drawer::GNode *a2 = detail::drawer::linkAstToAst(                  \
        ast, ast->child2, BOOST_PP_STRINGIZE(child2), g);                      \
    detail::drawer::rankAstNode(a1, a2, g_);                                   \
  } while (0)

#define VISIT_CHILD3(ast, g, child1, child2, child3)                           \
  do {                                                                         \
    detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);          \
    g->nodes.insert(node->id(), node);                                         \
    if (ast->child1) {                                                         \
      ast->child1->accept(this);                                               \
    }                                                                          \
    if (ast->child2) {                                                         \
      ast->child2->accept(this);                                               \
    }                                                                          \
    if (ast->child3) {                                                         \
      ast->child3->accept(this);                                               \
    }                                                                          \
    detail::drawer::GNode *a1 = detail::drawer::linkAstToAst(                  \
        ast, ast->child1, BOOST_PP_STRINGIZE(child1), g);                      \
    detail::drawer::GNode *a2 = detail::drawer::linkAstToAst(                  \
        ast, ast->child2, BOOST_PP_STRINGIZE(child2), g);                      \
    detail::drawer::GNode *a3 = detail::drawer::linkAstToAst(                  \
        ast, ast->child3, BOOST_PP_STRINGIZE(child3), g);                      \
    detail::drawer::rankAstNode(a1, a2, a3, g);                                \
  } while (0)

void Drawer::visitInteger(A_Integer *ast) { literalImpl(ast, g_); }
void Drawer::visitFloat(A_Float *ast) { literalImpl(ast, g_); }
void Drawer::visitBoolean(A_Boolean *ast) { literalImpl(ast, g_); }
void Drawer::visitCharacter(A_Character *ast) { literalImpl(ast, g_); }
void Drawer::visitString(A_String *ast) { literalImpl(ast, g_); }

void Drawer::visitNil(A_Nil *ast) { emptyImpl(ast, g_); }
void Drawer::visitVoid(A_Void *ast) { emptyImpl(ast, g_); }
void Drawer::visitBreak(A_Break *ast) { emptyImpl(ast, g_); }
void Drawer::visitContinue(A_Continue *ast) { emptyImpl(ast, g_); }

void Drawer::visitVarId(A_VarId *ast) {
  // create ast gnode
  detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);
  node->add("literal", ast->name());
  node->add("symbol", ast->symbol()
                          ? fmt::format("{}@{}", ast->symbol()->name(),
                                        ast->symbol()->identifier())
                          : "null");
  node->add("type symbol", ast->typeSymbol()
                               ? fmt::format("{}@{}", ast->typeSymbol()->name(),
                                             ast->typeSymbol()->identifier())
                               : "null");
  g_->nodes.insert(node->id(), node);

  // link symbol gnode
  if (ast->symbol()) {
    switch (ast->symbol()->kind()) {
    case SymbolKind::Func: {
      if (Symbol::isDefined(ast)) {
        linkAstToScope(ast, static_cast<S_Func *>(ast->symbol()), g_);
      }
      break;
    }
    default:
      break;
    }
  } else if (ast->typeSymbol()) {
    switch (ast->typeSymbol()->kind()) {
    case TypeSymbolKind::Class: {
      if (TypeSymbol::isDefined(ast)) {
        linkAstToClass(ast, g_);
      }
      break;
    }
    default:
      break;
    }
  } else {
    LOG_ASSERT(false, "invalid ast {}:{}", ast->name(), ast->location());
  }
}

void Drawer::visitPlainType(A_PlainType *ast) { VISIT_TOKEN(ast, g_, token); }

void Drawer::visitThrow(A_Throw *ast) { VISIT_CHILD1(ast, g_, expr); }
void Drawer::visitReturn(A_Return *ast) { VISIT_CHILD1(ast, g_, expr); }
void Drawer::visitYield(A_Yield *ast) { VISIT_CHILD1(ast, g_, expr); }
void Drawer::visitBlock(A_Block *ast) {
  detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);
  g_->nodes.insert(node->id(), node);
  linkAstToScope(ast, ast->scope(), g_);

  if (ast->blockStats) {
    ast->blockStats->accept(this);
  }

  linkAstToAst(ast, ast->blockStats, BOOST_PP_STRINGIZE(blockStats), g_);
}
void Drawer::visitCompileUnit(A_CompileUnit *ast) {
  detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);
  g_->nodes.insert(node->id(), node);
  linkAstToScope(ast, ast->scope(), g_);

  if (ast->topStats) {
    ast->topStats->accept(this);
  }

  linkAstToAst(ast, ast->topStats, BOOST_PP_STRINGIZE(topStats), g_);
}

void Drawer::visitPostfix(A_Postfix *ast) {
  VISIT_TOKEN_CHILD1(ast, g_, postfixOp, expr);
}
void Drawer::visitPrefix(A_Prefix *ast) {
  VISIT_TOKEN_CHILD1(ast, g_, prefixOp, expr);
}

void Drawer::visitCall(A_Call *ast) { VISIT_CHILD2(ast, g_, id, args); }
void Drawer::visitExprs(A_Exprs *ast) { VISIT_CHILD2(ast, g_, expr, next); }
void Drawer::visitDoWhile(A_DoWhile *ast) {
  VISIT_CHILD2(ast, g_, body, condition);
}
void Drawer::visitBlockStats(A_BlockStats *ast) {
  VISIT_CHILD2(ast, g_, blockStat, next);
}
void Drawer::visitFuncSign(A_FuncSign *ast) {
  VISIT_CHILD2(ast, g_, id, params);
}
void Drawer::visitParams(A_Params *ast) { VISIT_CHILD2(ast, g_, param, next); }
void Drawer::visitParam(A_Param *ast) { VISIT_CHILD2(ast, g_, id, type); }
void Drawer::visitTopStats(A_TopStats *ast) {
  VISIT_CHILD2(ast, g_, topStat, next);
}
void Drawer::visitLoop(A_Loop *ast) {
  detail::drawer::AstNode *node = new detail::drawer::AstNode(ast);
  g_->nodes.insert(node->id(), node);
  linkAstToScope(ast, ast->scope(), g_);

  if (ast->condition) {
    ast->condition->accept(this);
  }
  if (ast->body) {
    ast->body->accept(this);
  }

  linkAstToAst(ast, ast->condition, BOOST_PP_STRINGIZE(condition), g_);
  linkAstToAst(ast, ast->body, BOOST_PP_STRINGIZE(body), g_);
}

void Drawer::visitAssign(A_Assign *ast) {
  VISIT_TOKEN_CHILD2(ast, g_, assignOp, assignee, assignor);
}
void Drawer::visitInfix(A_Infix *ast) {
  VISIT_TOKEN_CHILD2(ast, g_, infixOp, left, right);
}

void Drawer::visitIf(A_If *ast) {
  VISIT_CHILD3(ast, g_, condition, thenp, elsep);
}
void Drawer::visitLoopCondition(A_LoopCondition *ast) {
  VISIT_CHILD3(ast, g_, init, condition, update);
}
void Drawer::visitLoopEnumerator(A_LoopEnumerator *ast) {
  VISIT_CHILD3(ast, g_, id, type, expr);
}
void Drawer::visitTry(A_Try *ast) {
  VISIT_CHILD3(ast, g_, tryp, catchp, finallyp);
}
void Drawer::visitFuncDef(A_FuncDef *ast) {
  VISIT_CHILD3(ast, g_, funcSign, resultType, body);
}
void Drawer::visitVarDef(A_VarDef *ast) {
  VISIT_CHILD3(ast, g_, id, type, expr);
}
