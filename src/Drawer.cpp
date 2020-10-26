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
#include <vector>

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
    // {'\'', "\\'"},
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
      : id(CNG.generate("cell")), value(TRANSLATE(a_value)), width(0),
        align("") {}
  GCell(const Cowstr &a_id, const Cowstr &a_value)
      : id(a_id), value(TRANSLATE(a_value)), width(0), align("") {}

  Cowstr id; // PORT="id"
  Cowstr value;
  int width; // COLSPAN="width"
  Cowstr align;

  Cowstr str() const {
    std::stringstream ss;
    ss << "<TD PORT=\"" << id << "\"";
    if (width) {
      ss << " COLSPAN=\"" << width << "\"";
    }
    if (!align.empty()) {
      ss << " ALIGN=\"" << align << "\"";
    }
    ss << ">" << value << "</TD>";
    return ss.str();
    // return width ? fmt::format("<TD PORT=\"{}\" COLSPAN=\"{}\">{}</TD>", id,
    //                            width, value)
    //              : fmt::format("<TD PORT=\"{}\">{}</TD>", id, value);
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
 * (node)id [label=<<TABLE> GLine+ </TABLE>>]
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

  virtual Cowstr str() const {
    std::stringstream ss;
    ss << ident
       << " [label=<<TABLE CELLBORDER=\"1\" CELLSPACING=\"0\" BORDER=\"0\">\n";
    for (int i = 0; i < (int)lines.size(); i++) {
      ss << lines[i].str();
      if (i < lines.size() - 1) {
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
  GEdgeKey(const Cowstr &a_key1) : key1(a_key1) {}
  GEdgeKey(const Cowstr &a_key1, const Cowstr &a_key2)
      : key1(a_key1), key2(a_key2) {}

  Cowstr key1;
  Cowstr key2;

  static Cowstr ident(const GEdgeKey &from, const GEdgeKey &to) {
    return from.str() + "-" + to.str();
  }
  bool hasCell() const { return !key2.empty(); }
  Cowstr str() const { return key2.empty() ? key1 : (key1 + ":" + key2); }
};

struct GEdge {
  GEdge(GEdgeKey a_from, GEdgeKey a_to, const Cowstr &a_style = "")
      : from(a_from), to(a_to), style(a_style) {}
  virtual ~GEdge() = default;

  GEdgeKey from;
  GEdgeKey to;
  std::vector<GLine> lines;
  Cowstr style;

  virtual Cowstr id() const { return GEdgeKey::ident(from, to); }
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << from.str() << "->" << to.str();
    if (from.hasCell() || to.hasCell()) {
      ss << ":w";
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
        if (i < (int)lines.size() - 1) {
          ss << "\n";
        }
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

struct AstToScopeEdge : public GEdge {
  AstToScopeEdge(Ast *from, ScopeNode *to)
      : GEdge(GEdgeKey(identify(from)),
              GEdgeKey(to->id(), to->lines[0].cells[0].id), "dashed") {}
};

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

struct SymbolToAstEdge : public GEdge {
  SymbolToAstEdge(Symbol *from, Ast *to)
      : GEdge(GEdgeKey(identify(from->owner()), identify(from)),
              GEdgeKey(identify(to)), "dotted") {}
};

struct TypeSymbolToAstEdge : public GEdge {
  TypeSymbolToAstEdge(TypeSymbol *from, Ast *to)
      : GEdge(GEdgeKey(identify(from->owner()), identify(from)),
              GEdgeKey(identify(to)), "dotted") {}
};

struct ScopeToAstEdge : public GEdge {
  ScopeToAstEdge(ScopeNode *from, Ast *to)
      : GEdge(GEdgeKey(from->id(), from->lines[0].cells[0].id),
              GEdgeKey(identify(to)), "dotted") {}
};

struct Graph {
  std::vector<GNode *> nodes;
  std::vector<GEdge *> edges;

  void draw(const Cowstr &fileName) {
    FileWriter fwriter(fileName);
    fwriter.writeln("digraph {");
    // fontsize: 12
    fwriter.writeln("    node [shape=none, fontname=\"Courier New, Courier\"]");
    fwriter.writeln("    edge [fontname=\"Courier New, Courier\"]");
    fwriter.writeln("    graph [fontname=\"Courier New, Courier\"]");
    fwriter.writeln();
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      (*i)->adjust();
    }
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      fwriter.writeln(fmt::format("    {}", (*i)->str()));
    }
    fwriter.writeln();
    for (auto i = edges.begin(); i != edges.end(); i++) {
      fwriter.writeln(fmt::format("    {}", (*i)->str()));
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
      g->nodes.push_back(node);                                                \
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
      g->nodes.push_back(node);                                                \
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
      g->nodes.push_back(node);                                                \
    }                                                                          \
  };

// kind, id, location, child1
#define DECL4(x, astype, child1)                                               \
  struct x : public Visitor {                                                  \
    x() : Visitor("Graph::" BOOST_PP_STRINGIZE(x) "::Visitor") {}              \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = static_cast<Context *>(context)->g;                           \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.push_back(node);                                                \
    }                                                                          \
    virtual void finishVisit(Ast *ast, VisitorContext *context) {              \
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
      g->nodes.push_back(node);                                                \
    }                                                                          \
    virtual void finishVisit(Ast *ast, VisitorContext *context) {              \
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
      g->nodes.push_back(node);                                                \
    }                                                                          \
    virtual void finishVisit(Ast *ast, VisitorContext *context) {              \
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
      g->nodes.push_back(node);                                                \
    }                                                                          \
    virtual void finishVisit(Ast *ast, VisitorContext *context) {              \
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
      g->nodes.push_back(node);                                                \
    }                                                                          \
    virtual void finishVisit(Ast *ast, VisitorContext *context) {              \
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
  if (b) {
    AstToAstEdge *edge = new AstToAstEdge(a, b, name);
    g->edges.push_back(edge);
  } else {
    NilNode *node = new NilNode();
    g->nodes.push_back(node);
    AstToNilEdge *edge = new AstToNilEdge(a, node->id(), name);
    g->edges.push_back(edge);
  }
}

static void linkAstToVar(A_VarId *varId, Graph *g) {
  S_Var *variableSymbol = static_cast<S_Var *>(varId->symbol);
  AstToSymbolEdge *edge = new AstToSymbolEdge(varId, variableSymbol);
  g->edges.push_back(edge);
}

static void linkAstToParam(A_VarId *varId, Graph *g) {
  S_Param *parameterSymbol = static_cast<S_Param *>(varId->symbol);
  AstToSymbolEdge *edge = new AstToSymbolEdge(varId, parameterSymbol);
  g->edges.push_back(edge);
}

static void linkAstToClass(A_VarId *varId, Graph *g) {
  LOG_ASSERT(false, "not implemented!");
}

static void linkAstToScope(Ast *ast, Scope *scope, Graph *g) {
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
  g->nodes.push_back(node);
  AstToScopeEdge *edge = new AstToScopeEdge(ast, node);
  g->edges.push_back(edge);
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
    g->nodes.push_back(node);

    // create symbol/typeSymbol/scope gnode
    makeLink(static_cast<A_VarId *>(ast), g);
  }
  void makeLink(A_VarId *varId, Graph *g) {
    if (varId->symbol) {
      switch (varId->symbol->kind()) {
      case SymbolKind::Var: {
        if (isSymbolDefined(varId)) {
          linkAstToVar(varId, g);
        }
        break;
      }
      case SymbolKind::Param: {
        if (isSymbolDefined(varId)) {
          linkAstToParam(varId, g);
        }
        break;
      }
      case SymbolKind::Func: {
        if (isSymbolDefined(varId)) {
          linkAstToScope(varId, static_cast<S_Func *>(varId->symbol), g);
        }
        break;
      }
      default:
        LOG_ASSERT(false, "invalid symbol {} kind: {}", varId->symbol->name(),
                   varId->symbol->kind()._to_string());
      }
    } else if (varId->typeSymbol) {
      switch (varId->typeSymbol->kind()) {
      case TypeSymbolKind::Class: {
        if (isTypeSymbolDefined(varId)) {
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
  bool isSymbolDefined(A_VarId *varId) {
    return varId->symbol && varId->symbol->ast() == varId;
  }
  bool isTypeSymbolDefined(A_VarId *varId) {
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
    g->nodes.push_back(node);

    // create local scope
    A_Block *block = static_cast<A_Block *>(ast);
    LOG_ASSERT(block->localScope, "block {} localScope must not null",
               block->name());
    linkAstToScope(block, block->localScope, g);
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
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
    g->nodes.push_back(node);

    // create global scope
    A_CompileUnit *compileUnit = static_cast<A_CompileUnit *>(ast);
    LOG_ASSERT(compileUnit->globalScope,
               "compile unit {} globalScope must not null",
               compileUnit->name());
    linkAstToScope(compileUnit, compileUnit->globalScope, g);
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
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
    g->nodes.push_back(node);

    // create local scope
    A_Loop *loop = static_cast<A_Loop *>(ast);
    LOG_ASSERT(loop->localScope, "loop {} localScope must not null",
               loop->name());
    linkAstToScope(loop, loop->localScope, g);
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
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

const Cowstr &Drawer::fileName() const { return fileName_; }

Cowstr &Drawer::fileName() { return fileName_; }
