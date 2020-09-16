// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Files.h"
#include "Log.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include "container/LinkedHashMap.hpp"
#include <sstream>
#include <utility>
#include <vector>

namespace detail {

static const std::unordered_map<char, Cowstr> HtmlTranslator = {
    {'\\', "\\\\"}, {'\n', "\\n"},  {'\t', "\\t"},  {'\r', "\\r"},
    {'\?', "\\?"},  {'\a', "\\a"},  {'\b', "\\b"},  {'\f', "\\f"},
    {'\v', "\\v"},  {'\'', "\\\'"}, {'\"', "\\\""}, {'<', "&lt;"},
    {'>', "&gt;"},  {'|', "&#124;"}};

static AstGraphNameGenerator nameGenerator;

struct AstDotNodeLabel {
  Cowstr title;
  Cowstr value;

  AstDotNodeLabel(const Cowstr &a_title, const Cowstr &a_value)
      : title(Cowstr(a_title).replace(HtmlTranslator)),
        value(Cowstr(a_value).replace(HtmlTranslator)) {}
  virtual ~AstDotNodeLabel() = default;
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << " + " << title << ": " << value << " \\l ";
    return ss.str();
  }
};

struct AstDotNode {
  Cowstr name;
  Cowstr header;
  std::vector<AstDotNodeLabel> labels;

  AstDotNode(const Cowstr &a_name, const Cowstr &a_header,
             const std::vector<AstDotNodeLabel> &a_labels = {})
      : name(Cowstr(a_name).replace(HtmlTranslator)),
        header(Cowstr(a_header).replace(HtmlTranslator)), labels(a_labels) {}
  virtual ~AstDotNode() = default;
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << name << " [label=\"{ " << header;
    for (auto i = labels.begin(); i != labels.end(); i++) {
      ss << " | " << i->str();
    }
    ss << " }\"]";
    return ss.str();
  }
};

struct AstDotEdge {
  Cowstr label;
  std::shared_ptr<AstDotNode> head;
  std::shared_ptr<AstDotNode> tail;

  AstDotEdge(const Cowstr &a_label, std::shared_ptr<AstDotNode> a_head,
             std::shared_ptr<AstDotNode> a_tail)
      : label(a_label), head(a_head), tail(a_tail) {}
  virtual ~AstDotEdge() = default;
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << head->name << " -> " << tail->name << " [label=\"" << label << "\"]";
    return ss.str();
  }
};

template <typename _Node, typename _Edge> struct DotGraph {
  std::vector<std::shared_ptr<_Node>> nodes;
  std::vector<std::shared_ptr<_Edge>> edges;
  std::vector<Cowstr> fontnames;
  int fontsize;
  Cowstr shape;
  Cowstr rankdir;

  DotGraph(const Cowstr &a_rankdir = "TB",
           const std::vector<Cowstr> &a_fontnames = {"Courier New", "Courier"},
           int a_fontsize = 12, const Cowstr &a_shape = "record")
      : fontnames(a_fontnames), fontsize(a_fontsize), shape(a_shape),
        rankdir(a_rankdir) {}

  virtual ~DotGraph() = default;

  virtual int draw(const Cowstr &output) {
    FileWriter fwriter(output);
    fwriter.writeln("digraph {");
    std::stringstream fontname_ss;
    for (int i = 0; i < (int)fontnames.size(); i++) {
      fontname_ss << fontnames[i];
      if (i < (int)fontnames.size() - 1) {
        fontname_ss << ", ";
      }
    }
    Cowstr fontnames_str = fontname_ss.str();
    fwriter.writeln(
        fmt::format("    graph [fontname=\"{}\", fontsize={}, rankdir={}]",
                    fontnames_str, fontsize, rankdir));
    fwriter.writeln(
        fmt::format("    node [shape={}, fontname=\"{}\", fontsize={}]", shape,
                    fontnames_str, fontsize));
    fwriter.writeln(fmt::format("    edge [fontname=\"{}\", fontsize={}]",
                                fontnames_str, fontsize));
    fwriter.writeln();
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      fwriter.writeln(fmt::format("    {}", (*i)->str()));
    }
    fwriter.write("\n");
    for (auto i = edges.begin(); i != edges.end(); i++) {
      fwriter.writeln(fmt::format("    {}", (*i)->str()));
    }
    fwriter.writeln("}");
    fwriter.flush();
    return 0;
  }
};

using adnl = AstDotNodeLabel;
using adn = AstDotNode;
using ade = AstDotEdge;
using adnsp = std::shared_ptr<AstDotNode>;
using adesp = std::shared_ptr<AstDotEdge>;

#define ADN_NIL                                                                \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), "nil"));                    \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_LITERAL(ast, atype)                                                \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), (ast)->name()));            \
    u->labels.push_back(                                                       \
        adnl("literal", static_cast<atype *>(ast)->literal()));                \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_LOCATION(ast)                                                      \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), (ast)->name()));            \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_ONE(ast, atype, a)                                                 \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), (ast)->name()));            \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp v = drawImpl(static_cast<atype *>(ast)->a);                          \
    adesp e(new ade(BOOST_PP_STRINGIZE(a), u, v));                             \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_ONE_OP(ast, atype, op, a)                                          \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), ast->name()));              \
    u->labels.push_back(adnl(BOOST_PP_STRINGIZE(op),                           \
                             tokenName(static_cast<atype *>(ast)->op)));       \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp v = drawImpl(static_cast<atype *>(ast)->a);                          \
    adesp e(new ade(BOOST_PP_STRINGIZE(a), u, v));                             \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_TWO_OP(ast, atype, op, a, b)                                       \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), ast->name()));              \
    u->labels.push_back(adnl(BOOST_PP_STRINGIZE(op),                           \
                             tokenName(static_cast<atype *>(ast)->op)));       \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp p = drawImpl(static_cast<atype *>(ast)->a);                          \
    adnsp q = drawImpl(static_cast<atype *>(ast)->b);                          \
    adesp ep(new ade(BOOST_PP_STRINGIZE(a), u, p));                            \
    adesp eq(new ade(BOOST_PP_STRINGIZE(b), u, q));                            \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define ADN_TWO(ast, atype, a, b)                                              \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), ast->name()));              \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp p = drawImpl(static_cast<atype *>(ast)->a);                          \
    adnsp q = drawImpl(static_cast<atype *>(ast)->b);                          \
    adesp ep(new ade(BOOST_PP_STRINGIZE(a), u, p));                            \
    adesp eq(new ade(BOOST_PP_STRINGIZE(b), u, q));                            \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define ADN_THREE(ast, atype, a, b, c)                                         \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), ast->name()));              \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp p = drawImpl(static_cast<atype *>(ast)->a);                          \
    adnsp q = drawImpl(static_cast<atype *>(ast)->b);                          \
    adnsp v = drawImpl(static_cast<atype *>(ast)->c);                          \
    adesp ep(new ade(BOOST_PP_STRINGIZE(a), u, p));                            \
    adesp eq(new ade(BOOST_PP_STRINGIZE(b), u, q));                            \
    adesp ev(new ade(BOOST_PP_STRINGIZE(c), u, v));                            \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

#define ADN_OP(ast, atype, op)                                                 \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"), ast->name()));              \
    u->labels.push_back(adnl(BOOST_PP_STRINGIZE(op),                           \
                             tokenName(static_cast<atype *>(ast)->op)));       \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

struct AstDotGraph {
  DotGraph<AstDotNode, AstDotEdge> g;

  AstDotGraph() {}
  virtual ~AstDotGraph() = default;

  virtual int draw(Ast *ast, const Cowstr &output) {
    drawImpl(ast);
    return g.draw(output);
  }

  virtual std::shared_ptr<AstDotNode> drawImpl(Ast *ast) {
    if (!ast) {
      ADN_NIL;
    }
    if (Ast::isLiteral(ast)) {
      switch (ast->kind()) {
      case AstKind::Integer: {
        ADN_LITERAL(ast, A_Integer);
      }
      case AstKind::Float: {
        ADN_LITERAL(ast, A_Float);
      }
      case AstKind::Boolean: {
        ADN_LITERAL(ast, A_Boolean);
      }
      case AstKind::Character: {
        ADN_LITERAL(ast, A_Character);
      }
      case AstKind::String: {
        ADN_LITERAL(ast, A_String);
      }
      case AstKind::Nil: {
        ADN_LOCATION(ast);
      }
      case AstKind::Void: {
        ADN_LOCATION(ast);
      }
      default:
        LOG_ASSERT(false, "invalid literal type: {}", ast->kind()._to_string());
      }
    }
    switch (ast->kind()) {
    case AstKind::VarId: {
      ADN_LITERAL(ast, A_VarId);
    }
    case AstKind::Break: {
      ADN_LOCATION(ast);
    }
    case AstKind::Continue: {
      ADN_LOCATION(ast);
    }
    case AstKind::Throw: {
      ADN_ONE(ast, A_Throw, expr);
    }
    case AstKind::Return: {
      ADN_ONE(ast, A_Return, expr);
    }
    case AstKind::Assign: {
      ADN_TWO_OP(ast, A_Assign, assignOp, assignee, assignor);
    }
    case AstKind::PostfixExpr: {
      ADN_ONE_OP(ast, A_PostfixExpr, postfixOp, expr);
    }
    case AstKind::InfixExpr: {
      ADN_TWO_OP(ast, A_InfixExpr, infixOp, left, right);
    }
    case AstKind::PrefixExpr: {
      ADN_ONE_OP(ast, A_PrefixExpr, prefixOp, expr);
    }
    case AstKind::Call: {
      ADN_TWO(ast, A_Call, id, args);
    }
    case AstKind::Exprs: {
      ADN_TWO(ast, A_Exprs, expr, next);
    }
    case AstKind::If: {
      ADN_THREE(ast, A_If, condition, thenp, elsep);
    }
    case AstKind::Loop: {
      ADN_TWO(ast, A_Loop, condition, body);
    }
    case AstKind::Yield: {
      ADN_ONE(ast, A_Yield, expr);
    }
    case AstKind::LoopCondition: {
      ADN_THREE(ast, A_LoopCondition, init, condition, update);
    }
    case AstKind::LoopEnumerator: {
      ADN_TWO(ast, A_LoopEnumerator, id, expr);
    }
    case AstKind::DoWhile: {
      ADN_TWO(ast, A_DoWhile, body, condition);
    }
    case AstKind::Try: {
      ADN_THREE(ast, A_Try, tryp, catchp, finallyp);
    }
    case AstKind::Block: {
      ADN_ONE(ast, A_Block, blockStats);
    }
    case AstKind::BlockStats: {
      ADN_TWO(ast, A_BlockStats, blockStat, next);
    }
    case AstKind::PlainType: {
      ADN_OP(ast, A_PlainType, token);
    }
    case AstKind::FuncDef: {
      ADN_THREE(ast, A_FuncDef, funcSign, resultType, body);
    }
    case AstKind::FuncSign: {
      ADN_TWO(ast, A_FuncSign, id, params);
    }
    case AstKind::Params: {
      ADN_TWO(ast, A_Params, param, next);
    }
    case AstKind::Param: {
      ADN_TWO(ast, A_Param, id, type);
    }
    case AstKind::VarDef: {
      ADN_THREE(ast, A_VarDef, id, type, expr);
    }
    case AstKind::TopStats: {
      ADN_TWO(ast, A_TopStats, topStat, next);
    }
    case AstKind::CompileUnit: {
      ADN_ONE(ast, A_CompileUnit, topStats);
    }
    default:
      LOG_ASSERT(false, "invalid ast kind: {}", ast->kind()._to_string());
    }
  }
};

/**
 * { <field2> title: value | [ title: value ]* }
 *     |       |      |
 *     id     title  value
 */
struct SymbolDotNodeLabel {
  Cowstr id;
  std::vector<std::pair<Cowstr, Cowstr>> attributes;

  SymbolDotNodeLabel(const Cowstr &a_id)
      : id(Cowstr(a_id).replace(HtmlTranslator)) {}
  virtual ~SymbolDotNodeLabel() = default;
  virtual void add(const Cowstr &title, const Cowstr &value) {
    attributes.push_back(std::make_pair(Cowstr(title).replace(HtmlTranslator),
                                        Cowstr(value).replace(HtmlTranslator)));
  }
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << "{ <" << id << "> ";
    for (int i = 0; i < (int)attributes.size(); i++) {
      ss << fmt::format("{}: {}", attributes[i].first, attributes[i].second);
      if (i < (int)attributes.size() - 1) {
        ss << " | ";
      }
    }
    ss << " }";
    return ss.str();
  }
};

/**
 * SymbolNode1[label="<field2>        name      | labels "]
 *     |                 |             |            |
 *     id              headLabelId    headLabel   labels
 */
struct SymbolDotNode {
  Cowstr id;
  Cowstr headLabelId;
  Cowstr headLabel;
  std::vector<std::shared_ptr<SymbolDotNodeLabel>> labels;

  SymbolDotNode(const Cowstr &a_id, const Cowstr &a_headLabelId,
                const Cowstr &a_headLabel)
      : id(Cowstr(a_id).replace(HtmlTranslator)),
        headLabelId(Cowstr(a_headLabelId).replace(HtmlTranslator)),
        headLabel(Cowstr(a_headLabel).replace(HtmlTranslator)) {}
  virtual ~SymbolDotNode() = default;
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << id << " [label=\"<" << headLabelId << "> " << headLabel;
    for (auto i = labels.begin(); i != labels.end(); i++) {
      ss << " | " << (*i)->str();
    }
    ss << "\"]";
    return ss.str();
  }
};

/**
 * SymbolNode1:field2      -> SymbolNode2:field5
 *     |         |                 |        |
 *   fromId    fromLabelId       toId     toLabelId
 */
struct SymbolDotEdge {
  Cowstr fromId;      // from node id
  Cowstr fromLabelId; // from node label id
  Cowstr toId;        // to node id
  Cowstr toLabelId;   // to node label id

  SymbolDotEdge(const Cowstr &a_fromId, const Cowstr &a_fromLabelId,
                const Cowstr &a_toId, const Cowstr &a_toLabelId)
      : fromId(a_fromId), fromLabelId(a_fromLabelId), toId(a_toId),
        toLabelId(a_toLabelId) {}
  virtual ~SymbolDotEdge() = default;
  virtual Cowstr str() const {
    return fmt::format("{}:{} -> {}:{}", fromId, fromLabelId, toId, toLabelId);
  }
};

using sdnl = SymbolDotNodeLabel;
using sdn = SymbolDotNode;
using sde = SymbolDotEdge;
using sdnlsp = std::shared_ptr<SymbolDotNodeLabel>;
using sdnsp = std::shared_ptr<SymbolDotNode>;
using sdesp = std::shared_ptr<SymbolDotEdge>;

struct SymbolDotGraph {
  DotGraph<SymbolDotNode, SymbolDotEdge> g;

  SymbolDotGraph() : g("LR") {}
  virtual ~SymbolDotGraph() = default;

  virtual int draw(std::shared_ptr<Scope> scope, const Cowstr &output) {
    drawImpl(scope);
    return g.draw(output);
  }

  virtual std::shared_ptr<SymbolDotNode>
  drawImpl(std::shared_ptr<Scope> scope) {
    if (!scope) {
      sdnsp u(new sdn(nameGenerator.from("SymbolNode"),
                      nameGenerator.from("field"), "nil"));
      return u;
    }
    sdnsp u(new sdn(nameGenerator.from("SymbolNode"),
                    nameGenerator.from("field"), scope->name()));
    g.nodes.push_back(u);
    for (auto i = scope->s_begin(); i != scope->s_end(); i++) {
      SymbolData sdata = i->second;
      switch (sdata.symbol->kind()) {
      case SymbolKind::Var:
      case SymbolKind::Param:
      case SymbolKind::Field: {
        u->labels.push_back(drawSymbol(sdata.symbol));
        break;
      }
      case SymbolKind::Func:
      case SymbolKind::Method:
      case SymbolKind::Local:
      case SymbolKind::Global: {
        sdnlsp label = drawSymbol(sdata.symbol);
        u->labels.push_back(label);
        sdnsp v = drawImpl(std::dynamic_pointer_cast<Scope>(sdata.symbol));
        sdesp e(new sde(u->id, label->id, v->id, v->headLabelId));
        g.edges.push_back(e);
        break;
      }
      default:
        LOG_ASSERT(false, "invalid symbol type: {}",
                   sdata.symbol->kind()._to_string());
      }
    }
    for (auto i = scope->ts_begin(); i != scope->ts_end(); i++) {
      TypeSymbolData tsdata = i->second;
      u->labels.push_back(drawTypeSymbol(tsdata.typeSymbol));
    }
    return u;
  }

  virtual sdnlsp drawSymbol(std::shared_ptr<Symbol> sym) {
    if (!sym) {
      return sdnlsp(new sdnl(nameGenerator.from("field")));
    }
    sdnlsp l(new sdnl(nameGenerator.from("field")));
    l->add("name", sym->name());
    if (sym->owner()) {
      SymbolData sdata = sym->owner()->s_resolve(sym->name());
      l->add("type", sdata.typeSymbol->name());
    } else {
      l->add("type", "void");
    }
    l->add("symbol kind", sym->kind()._to_string());
    return l;
  }

  virtual sdnlsp drawTypeSymbol(std::shared_ptr<TypeSymbol> sym) {
    if (!sym) {
      return sdnlsp(new sdnl(nameGenerator.from("field")));
    }
    sdnlsp l(new sdnl(nameGenerator.from("field")));
    l->add("name", sym->name());
    l->add("type symbol kind", sym->kind()._to_string());
    return l;
  }
};

} // namespace detail

int Graph::drawAst(Ast *ast, const Cowstr &output) {
  detail::AstDotGraph g;
  return g.draw(ast, output);
}

int Graph::drawSymbol(std::shared_ptr<Scope> scope, const Cowstr &output) {
  detail::SymbolDotGraph g;
  return g.draw(scope, output);
}
