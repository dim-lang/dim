// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Counter.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace detail {

static CounterNameGenerator CNG;

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
  GCell(const Cowstr &a_id, const Cowstr &a_value, int a_width)
      : id(a_id), value(TRANSLATE(a_value)), width(a_width) {}
  ~GCell() = default;

  Cowstr id; // PORT="id"
  Cowstr value;
  int width; // COLSPAN="width"

  Cowstr str() {
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
  ~GLine() = default;

  std::vector<GCell> cells;

  Cowstr str() {
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
  GNode(const Cowstr &a_id) : id(a_id) {}
  virtual ~GNode() = default;

  Cowstr id;
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
      if (lines[i].cells.size() < maxWidth) {
        lines[i].cells[0].width = maxWidth - lines[i].cells.size() + 1;
      }
    }
  }

  virtual Cowstr str() {
    adjust();
    const std::unordered_map<Cowstr, Cowstr> &attributes = {
        {"BORDER", "\"0\""},
        {"CELLBORDER", "\"1\""},
        {"CELLSPACING", "\"0\""},
    };
    std::stringstream ss;
    ss << id << " [label=<<TABLE";
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
  virtual Cowstr id() { return id; }
};

struct NilNode : public GNode {
  NilNode() : GNode(CNG.generate("nilnode")) {
    GCell c(CNG.generate("cell"), "nil", 0);
    GLine l({c});
    lines.push_back(l);
  }
};

struct AstNode : public GNode {
  AstGNode(Ast *ast) : GNode(identify(ast)) {
    add(ast->kind()._to_string());
    add("id", Cowstr::from(ast->identifier()));
    add("location", ast->location().str());
  }

  static Cowstr identify(Ast *ast) {
    return Cowstr("astnode") + Cowstr::from(ast->identifier());
  }
  void add(const Cowstr &a) {
    GCell c(CNG.generate("cell"), a, 0);
    GLine l({c});
    lines.push_back(l);
  }
  void add(const Cowstr &a, const Cowstr &b) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GLine l({c1, c2});
    lines.push_back(l);
  }
};

struct ScopeNode : public GNode {
  ScopeNode(Scope *scope) : GNode(identify(scope)) {
    add(scope->name());
    add("id", Cowstr::from(ast->identifier()));
    add("location", ast->location().str());
  }

  static Cowstr identify(Scope *scope) {
    return Cowstr("scopenode") + Cowstr::from(scope->identifier());
  }
  static Cowstr identifySymbol(Symbol *sym) {
    return Cowstr("symbolcell") + Cowstr::from(sym->identifier());
  }
  static Cowstr identifyTypeSymbol(TypeSymbol *tsym) {
    return Cowstr("typesymbolcell") + Cowstr::from(tsym->identifier());
  }
  static Cowstr identifyScope(TypeSymbol *tsym) {
    return Cowstr("scopecell") + Cowstr::from(tsym->identifier());
  }
  void add(const Cowstr &a) {
    GCell c(CNG.generate("cell"), a, 0);
    GLine line({c});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GLine line({c1, c2});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b, const Cowstr &c) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GCell c3(CNG.generate("cell"), c, 0);
    GLine line({c1, c2, c3});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b, const Cowstr &c, const Cowstr &d) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GCell c3(CNG.generate("cell"), c, 0);
    GCell c4(CNG.generate("cell"), d, 0);
    GLine line({c1, c2, c3, c4});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b, const Cowstr &c, const Cowstr &d,
           const Cowstr &e) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GCell c3(CNG.generate("cell"), c, 0);
    GCell c4(CNG.generate("cell"), d, 0);
    GCell c5(CNG.generate("cell"), e, 0);
    GLine line({c1, c2, c3, c4, c5});
    node.lines.push_back(line);
  }
  void addSymbol(Symbol *sym) {
    GCell c1(identifySymbol(sym), Cowstr::from(sym->identifier()), 0);
    GCell c2(CNG.generate("cell"), sym->name(), 0);
    GCell c3(CNG.generate("cell"), sym->type()->name(), 0);
    GCell c4(CNG.generate("cell"), sym->kind()._to_string(), 0);
    GCell c5(CNG.generate("cell"), sym->location().str(), 0);
    GLine line({c1, c2, c3, c4, c5});
    node.lines.push_back(line);
  }
  void addTypeSymbol(TypeSymbol *tsym) {
    GCell c1(identifyTypeSymbol(tsym), Cowstr::from(tsym->identifier()), 0);
    GCell c2(CNG.generate("cell"), tsym->name(), 0);
    GCell c3(CNG.generate("cell"), tsym->kind()._to_string(), 0);
    GCell c4(CNG.generate("cell"), tsym->location().str(), 0);
    GLine line({c1, c2, c3, c4});
    node.lines.push_back(line);
  }
  void addScope(Scope *scope) {
    GCell c1(identifyScope(scope), Cowstr::from(scope->identifier()), 0);
    GCell c2(CNG.generate("cell"), scope->name(), 0);
    GCell c3(CNG.generate("cell"), scope->kind()._to_string(), 0);
    GCell c4(CNG.generate("cell"), scope->location().str(), 0);
    GLine line({c1, c2, c3, c4});
    node.lines.push_back(line);
  }
};

struct GEdgeID {
  GEdgeID(const Cowstr &a_id1) : id1(a_id1) {}
  GEdgeID(const Cowstr &a_id1, const Cowstr &a_id2) : id1(a_id1), id2(a_id2) {}

  Cowstr id1;
  Cowstr id2;

  Cowstr str() { return id2.empty() ? id1 : (id1 + ":" + id2); }
};

struct GEdge {
  GEdge(GEdgeID a_from, GEdgeID a_to, bool a_dotted)
      : from(a_from), to(a_to), dotted(a_dotted) {}
  virtual ~GEdge() = default;

  GEdgeID from;
  GEdgeID to;
  std::vector<GLine> lines;
  bool dotted;

  static Cowstr identify(GEdgeID from, GEdgeID to) {
    return from.str() + "-" + to.str();
  }
  virtual Cowstr id() { return identify(from, to); }
  virtual Cowstr str() {
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
      : GEdge(GEdgeID(AstNode::identify(from)), GEdgeID(to), false) {}
};

struct AstToAstEdge : public GEdge {
  AstToAstEdge(Ast *from, Ast *to, const Cowstr &name)
      : GEdge(GEdgeID(AstNode::identify(from)), GEdgeID(AstNode::identify(to)),
              false) {
    add(name);
  }

  static Cowstr identify(Ast *from, Ast *to) {
    GEdgeID a(AstNode::identify(from));
    GEdgeID b(AstNode::identify(to));
    return GEdge::identify(a, b);
  }
  void add(const Cowstr &a) {
    GCell c(CNG.generate("edge"), a, 0);
    GLine line({c});
    edge.lines.push_back(line);
  }
};

struct AstToScopeEdge : public GEdge {
  AstToScopeEdge(Ast *from, Scope *to)
      : GEdge(GEdgeID(AstNode::identify(from)),
              GEdgeID(ScopeNode::identify(to)), true) {}

  static Cowstr identify(Ast *from, Scope *to) {
    GEdgeID a(AstNode::identify(from));
    GEdgeID b(ScopeNode::identify(to));
    return GEdge::identify(a, b);
  }
};

struct AstToSymbolEdge : public GEdge {
  AstToSymbolEdge(Ast *from, Symbol *to)
      : edge(GEdgeID(AstNode::identify(from)),
             GEdgeID(ScopeNode::identify(to->owner()),
                     ScopeNode::identifySymbol(to)),
             true) {}
};

struct AstToTypeSymbolEdge : public GEdge {
  AstToTypeSymbolEdge(Ast *from, TypeSymbol *to)
      : GEdge(GEdgeID(AstNode::identify(from)),
              GEdgeID(ScopeNode::identify(to->owner()),
                      ScopeNode::identifyTypeSymbol(to)),
              true) {}
};

struct SymbolToAstEdge : public GEdge {
  SymbolToAstEdge(Symbol *from, Ast *to)
      : GEdge(GEdgeID(AstNode::identify(from)),
              GEdgeID(ScopeNode::identify(to->owner()),
                      ScopeNode::identifySymbol(to)),
              false) {}
};

struct TypeSymbolToAstEdge : public GEdge {
  TypeSymbolToAstEdge(TypeSymbol *from, Ast *to)
      : GEdge(GEdgeID(ScopeNode::identify(from->owner()),
                      ScopeNode::identifyTypeSymbol(from)),
              GEdgeID(AstNode::identify(to)), false) {}
};

struct Graph {
  Graph(Ast *a_ast) : ast(a_ast) {}
  void draw(const Cowstr &fileName) {
    const static std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>>
        attributes = {
            {
                "node",
                {
                    {"shape", "none"}, {"fontname", "\"Courier New, Courier\""},
                    // {"fontsize", "12"},
                },
            },
            {
                "edge",
                {
                    {"fontname", "\"Courier New, Courier\""},
                    // {"fontsize", "12"},
                },
            },
            {
                "graph",
                {
                    {"fontname", "\"Courier New, Courier\""},
                    // {"fontsize", "12"},
                },
            },
        };
  }

  Ast *ast;
  std::unordered_map<Cowstr, GNode *> nodes;
  std::unordered_map<Cowstr, GEdge *> edges;

  static void linkAstToAst(Ast *a, Ast *b, VisitorContext *context,
                           const Cowstr &name) {
    LOG_ASSERT(a, "Ast a is null");
    Graph *g = cast(Context, context)->g;
    auto u = g->nodes.find(AstNode::identify(a));
    LOG_ASSERT(u != g->nodes.end() && cast(GNode, u->second),
               "Ast a {} don't has related GNode {} in graph", a->name(),
               AstNode::identify(a));
    if (b) {
      auto v = g->nodes.find(AstNode::identify(b));
      LOG_ASSERT(v != g->nodes.end() && cast(GNode, v->second),
                 "Ast b {} don't has related GNode {} in graph", b->name(),
                 AstNode::identify(b));
      Cowstr eid = AstToAstEdge::identify(a, b);
      LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
                 "eid {} not exist, Ast a {} hasn't link to Ast b {}", eid,
                 a->name(), b->name());
      AstToAstEdge *e = new AstToAstEdge(a, b, name);
      LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
      g->edges.insert(std::make_pair(e->id(), e));
    } else {
      NilNode *ndoe = new NilNode();
      g->nodes.insert(std::make_pair(node->id(), node));
      AstToNilEdge *e = new AstToNilEdge(a, node->id(), name);
      g->edges.insert(std::make_pair(e->id(), e));
    }
  }

  struct Context : public VisitorContext {
    Graph *g;
  };

  // kind, id, location
#define DECL1(x)                                                               \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
  };

  // kind, id, location, literal
#define DECL2(x)                                                               \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      node->add("literal", ast->name());                                       \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
  };

  // kind, id, location, token
#define DECL3(x, astype, tok)                                                  \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(cast(astype, ast)->tok));   \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
  };

  // kind, id, location, child1
#define DECL4(x, astype, child1)                                               \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      linkAstToAst(ast, cast(astype, ast)->child1,                             \
                   BOOST_PP_STRINGIZE(child1));                                \
    }                                                                          \
  };

  // kind, id, location, token, child1
#define DECL5(x, astype, tok, child1)                                          \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(cast(astype, ast)->tok));   \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      linkAstToAst(ast, cast(astype, ast)->child1,                             \
                   BOOST_PP_STRINGIZE(child1));                                \
    }                                                                          \
  };

  // kind, id, location, child1, child2
#define DECL6(x, astype, child1, child2)                                       \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      linkAstToAst(ast, cast(astype, ast)->child1,                             \
                   BOOST_PP_STRINGIZE(child1));                                \
      linkAstToAst(ast, cast(astype, ast)->child2,                             \
                   BOOST_PP_STRINGIZE(child2));                                \
    }                                                                          \
  };

  // kind, id, location, token, child1, child2
#define DECL7(x, astype, tok, child1, child2)                                  \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      node->add(BOOST_PP_STRINGIZE(tok), tokenName(cast(astype, ast)->tok));   \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      linkAstToAst(ast, cast(astype, ast)->child1,                             \
                   BOOST_PP_STRINGIZE(child1));                                \
      linkAstToAst(ast, cast(astype, ast)->child2,                             \
                   BOOST_PP_STRINGIZE(child2));                                \
    }                                                                          \
  };

  // kind, id, location, child1, child2, child3
#define DECL8(x, astype, child1, child2, child3)                               \
  struct x : public Visitor {                                                  \
    virtual void visit(Ast *ast, VisitorContext *context) {                    \
      Graph *g = cast(Context, context)->g;                                    \
      AstNode *node = new AstNode(ast);                                        \
      g->nodes.insert(std::make_pair(node->id(), node));                       \
    }                                                                          \
    virtual void postVisit(Ast *ast, VisitorContext *context) {                \
      linkAstToAst(ast, cast(astype, ast)->child1,                             \
                   BOOST_PP_STRINGIZE(child1));                                \
      linkAstToAst(ast, cast(astype, ast)->child2,                             \
                   BOOST_PP_STRINGIZE(child2));                                \
      linkAstToAst(ast, cast(astype, ast)->child3,                             \
                   BOOST_PP_STRINGIZE(child3));                                \
    }                                                                          \
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

  DECL2(VarId)

  struct VarId : public Visitor {
    virtual void visit(Ast *ast, VisitorContext *context) {
      Graph *g = cast(Context, context)->g;

      // create ast gnode
      AstNode *node = new AstNode(ast);
      node->add("literal", ast->name());
      Symbol *sym = cast(A_VarId, ast)->symbol;
      TypeSymbol *tsym = cast(A_VarId, ast)->typeSymbol;
      node->add("symbol",
                sym ? fmt::format("{}:{}", sym->name(), sym->identifier())
                    : "null");
      node->add("type symbol",
                tsym ? fmt::format("{}:{}", tsym->name(), tsym->identifier())
                     : "null");
      g->nodes.insert(std::make_pair(node->id(), node));

      // create symbol/typeSymbol/scope gnode
      link(cast(A_VarId, ast));
    }
    virtual void postVisit(Ast *ast, VisitorContext *context) {}

    void link(A_VarId *varId) {
      if (varId->symbol) {
        switch (varId->symbol->kind()) {
        case SymbolKind::Var: {
          if (defineSymbol(varId)) {
            linkAstToVar(varId);
          }
          break;
        }
        case SymbolKind::Param: {
          if (defineSymbol(varId)) {
            linkAstToParam(varId);
          }
          break;
        }
        case SymbolKind::Func: {
          if (defineSymbol(varId)) {
            linkAstToFunction(varId);
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
            linkClassToAst(varId);
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

    void linkAstToVar(A_VarId *varId) {}

    void linkAstToFunction(A_VarId *varId) {
      S_Func functionSymbol = cast(S_Func, varId->symbol);
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
      if (functionSymbol->sc_empty()) {
        node->add("empty");
      } else {
        for (auto i = functionSymbol->sc_begin(); i != functionSymbol->sc_end();
             i++) {
          node->addScope(i->second);
        }
      }
      g->nodes.insert(std::make_pair(node->id(), node));
      Cowstr eid = AstToScopeEdge::identify(varId, functionSymbol);
      LOG_ASSERT(g->edges.find(eid) == g->edges.end(),
                 "eid {} not exist, varId {} has no link to Scope {}", eid,
                 varId->name(), functionSymbol->name());
      AstToScopeEdge *e = new AstToScopeEdge(varId, functionSymbol);
      LOG_ASSERT(e->id() == eid, "e->id {} != eid {}", e->id(), eid);
      g->edges.insert(std::make_pair(e->id(), e));
    }

    void linkAstToParam(A_VarId *varId) {}
  };

  DECL3(PlainType, A_PlainType, token)

  DECL4(Throw, A_Throw, expr)
  DECL4(Return, A_Return, expr)
  DECL4(Yield, A_Yield, expr)
  DECL4(Block, A_Block, blockStats)
  DECL4(CompileUnit, A_CompileUnit, topStats)

  DECL5(PostfixExpr, A_PostfixExpr, postfixOp, expr)
  DECL5(PrefixExpr, A_PrefixExpr, prefixOp, expr)

  DECL6(Call, A_Call, id, args)
  DECL6(Exprs, A_Exprs, expr, next)
  DECL6(Loop, A_Loop, condition, body)
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
  DECL2(VarDef, A_VarDef, id, type, expr)
};

} // namespace detail

int Drawer::draw(Ast *ast, const Cowstr &output) { return 0; }
