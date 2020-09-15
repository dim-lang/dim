// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Files.h"
#include "Log.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include <sstream>
#include <utility>
#include <vector>

#define ASP(x) (static_cast<x *>(ast))

namespace detail {

static const std::unordered_map<char, Cowstr> HtmlTranslator = {
    {'\\', "\\\\"}, {'\n', "\\n"},  {'\t', "\\t"},  {'\r', "\\r"},
    {'\?', "\\?"},  {'\a', "\\a"},  {'\b', "\\b"},  {'\f', "\\f"},
    {'\v', "\\v"},  {'\'', "\\\'"}, {'\"', "\\\""}, {'<', "&lt;"},
    {'>', "&gt;"},  {'|', "&#124;"}};

static AstGraphNameGenerator nameGenerator;

#if 0
struct AstDotNode;
struct AstDotEdge;
struct AstDot;

using adn = AstDotNode;
using ade = AstDotEdge;
using adnsp = std::shared_ptr<AstDotNode>;
using adesp = std::shared_ptr<AstDotEdge>;

struct AstDotNode {
  Cowstr name;
  Cowstr header;
  std::vector<std::pair<Cowstr, Cowstr>> label;

  AstDotNode(const Cowstr &a_header)
      : name(nameGenerator.from("node")),
        header(Cowstr(a_header).replace(HtmlTranslator)), label() {}
  virtual ~AstDotNode() = default;

  virtual AstDotNode &add(const Cowstr &a, const Cowstr &b) {
    label.push_back(std::make_pair(Cowstr(a).replace(HtmlTranslator),
                                   Cowstr(b).replace(HtmlTranslator)));
    return *this;
  }
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << name << " [label=\"{ " << header;
    for (auto i = label.begin(); i != label.end(); i++) {
      ss << " | + " << i->first << ": " << i->second << "\\l ";
    }
    ss << " }\"]";
    return ss.str();
  }
};

struct AstDotEdge {
  Cowstr label;
  adnsp head;
  adnsp tail;

  AstDotEdge(const Cowstr &a_label, adnsp a_head, adnsp a_tail)
      : label(a_label), head(a_head), tail(a_tail) {}
  virtual ~AstDotEdge() = default;

  virtual Cowstr str() const {
    std::stringstream ss;
    ss << head->name << " -> " << tail->name << " [label=\"" << label << "\"]";
    return ss.str();
  }
};

#define L_NODE(ast_type)                                                       \
  do {                                                                         \
    adnsp u(new adn(ast->name().replace(HtmlTranslator)));                     \
    u->add("literal", ASP(ast_type)->literal());                               \
    u->add("location", ast->location().str());                                 \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define NODE                                                                   \
  do {                                                                         \
    adnsp u(new adn(ast->name().replace(HtmlTranslator)));                     \
    u->add("location", ast->location().str());                                 \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define S_NODE                                                                 \
  adnsp u(new adn(ast->name().replace(HtmlTranslator)));                       \
  u->add("location", ast->location().str())

struct AstDot {
  std::vector<adnsp> nodes;
  std::vector<adesp> edges;

  AstDot() : nodes(), edges() {}
  virtual ~AstDot() = default;

  virtual int draw(Ast *ast, const Cowstr &output) {
    FileWriter fwriter(output);
    drawImpl(ast);
    fwriter.writeln("digraph {");
    fwriter.writeln(
        "    graph [fontname=\"Courier New, Courier\", fontsize=12]");
    fwriter.writeln("    node [shape=record, fontname=\"Courier New, "
                    "Courier\", fontsize=12]");
    fwriter.writeln(
        "    edge [fontname=\"Courier New, Courier\", fontsize=12]");
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

private:
  virtual adnsp drawImpl(Ast *ast) {
    if (!ast) {
      adnsp u(new adn("nil"));
      g.nodes.push_back(u);
      return u;
    }
    if (Ast::isLiteral(ast)) {
      switch (ast->kind()) {
      case AstKind::Integer: {
        L_NODE(A_Integer);
      }
      case AstKind::Float: {
        L_NODE(A_Float);
      }
      case AstKind::Boolean: {
        L_NODE(A_Boolean);
      }
      case AstKind::Character: {
        L_NODE(A_Character);
      }
      case AstKind::String: {
        L_NODE(A_String);
      }
      case AstKind::Nil: {
        NODE;
      }
      case AstKind::Void: {
        NODE;
      }
      default:
        LOG_ASSERT(false, "invalid literal type: {}", ast->kind()._to_string());
      }
    }
    switch (ast->kind()) {
    case AstKind::VarId: {
      L_NODE(A_VarId);
    }
    case AstKind::Break: {
      NODE;
    }
    case AstKind::Continue: {
      NODE;
    }
    case AstKind::Throw: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Throw)->expr);
      adesp e(new ade("expr", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    case AstKind::Return: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Return)->expr);
      adesp e(new ade("expr", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    case AstKind::Assign: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "assignOp",
          Cowstr(tokenName(ASP(A_Assign)->assignOp)).replace(HtmlTranslator)));
      adnsp p = drawImpl(ASP(A_Assign)->assignee);
      adnsp q = drawImpl(ASP(A_Assign)->assignor);
      adesp ep(new ade("assignee", u, p));
      adesp eq(new ade("assignor", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::PostfixExpr: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "postfixOp", Cowstr(tokenName(ASP(A_PostfixExpr)->postfixOp))
                           .replace(HtmlTranslator)));
      adnsp v = drawImpl(ASP(A_PostfixExpr)->expr);
      adesp e(new ade("expr", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    case AstKind::InfixExpr: {
      S_NODE;
      u->label.push_back(
          std::make_pair("infixOp", Cowstr(tokenName(ASP(A_InfixExpr)->infixOp))
                                        .replace(HtmlTranslator)));
      adnsp p = drawImpl(ASP(A_InfixExpr)->left);
      adnsp q = drawImpl(ASP(A_InfixExpr)->right);
      adesp ep(new ade("left", u, p));
      adesp eq(new ade("right", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::PrefixExpr: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "prefixOp", Cowstr(tokenName(ASP(A_PrefixExpr)->prefixOp))
                          .replace(HtmlTranslator)));
      adnsp v = drawImpl(ASP(A_PrefixExpr)->expr);
      adesp e(new ade("expr", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    case AstKind::Call: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Call)->id);
      adnsp q = drawImpl(ASP(A_Call)->args);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("args", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::Exprs: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Exprs)->expr);
      adnsp q = drawImpl(ASP(A_Exprs)->next);
      adesp ep(new ade("expr", u, p));
      adesp eq(new ade("next", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::If: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_If)->condition);
      adnsp q = drawImpl(ASP(A_If)->thenp);
      adnsp v = drawImpl(ASP(A_If)->elsep);
      adesp ep(new ade("condition", u, p));
      adesp eq(new ade("then", u, q));
      adesp ev(new ade("else", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      g.edges.push_back(ev);
      return u;
    }
    case AstKind::Loop: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Loop)->condition);
      adnsp q = drawImpl(ASP(A_Loop)->body);
      adesp ep(new ade("condition", u, p));
      adesp eq(new ade("body", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::Yield: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Yield)->expr);
      adesp e(new ade("expr", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    case AstKind::LoopCondition: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_LoopCondition)->init);
      adnsp q = drawImpl(ASP(A_LoopCondition)->condition);
      adnsp v = drawImpl(ASP(A_LoopCondition)->update);
      adesp ep(new ade("init", u, p));
      adesp eq(new ade("condition", u, q));
      adesp ev(new ade("update", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      g.edges.push_back(ev);
      return u;
    }
    case AstKind::LoopEnumerator: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_LoopEnumerator)->id);
      adnsp q = drawImpl(ASP(A_LoopEnumerator)->expr);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("expr", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::DoWhile: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_DoWhile)->body);
      adnsp q = drawImpl(ASP(A_DoWhile)->condition);
      adesp ep(new ade("body", u, p));
      adesp eq(new ade("condition", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::Try: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Try)->tryp);
      adnsp q = drawImpl(ASP(A_Try)->catchp);
      adnsp v = drawImpl(ASP(A_Try)->finallyp);
      adesp ep(new ade("try", u, p));
      adesp eq(new ade("catch", u, q));
      adesp ev(new ade("finally", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      g.edges.push_back(ev);
      return u;
    }
    case AstKind::Block: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Block)->blockStats);
      adesp e(new ade("blockStats", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    case AstKind::BlockStats: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_BlockStats)->blockStat);
      adnsp q = drawImpl(ASP(A_BlockStats)->next);
      adesp ep(new ade("blockStat", u, p));
      adesp eq(new ade("next", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::PlainType: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "token",
          Cowstr(tokenName(ASP(A_PlainType)->token)).replace(HtmlTranslator)));
      g.nodes.push_back(u);
      return u;
    }
    case AstKind::FuncDef: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_FuncDef)->funcSign);
      adnsp q = drawImpl(ASP(A_FuncDef)->resultType);
      adnsp v = drawImpl(ASP(A_FuncDef)->body);
      adesp ep(new ade("funcSign", u, p));
      adesp eq(new ade("resultType", u, q));
      adesp ev(new ade("body", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      g.edges.push_back(ev);
      return u;
    }
    case AstKind::FuncSign: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_FuncSign)->id);
      adnsp q = drawImpl(ASP(A_FuncSign)->params);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("params", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::Params: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Params)->param);
      adnsp q = drawImpl(ASP(A_Params)->next);
      adesp ep(new ade("param", u, p));
      adesp eq(new ade("next", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::Param: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Param)->id);
      adnsp q = drawImpl(ASP(A_Param)->type);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("type", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::VarDef: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_VarDef)->id);
      adnsp q = drawImpl(ASP(A_VarDef)->type);
      adnsp v = drawImpl(ASP(A_VarDef)->expr);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("type", u, q));
      adesp ev(new ade("expr", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      g.edges.push_back(ev);
      return u;
    }
    case AstKind::TopStats: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_TopStats)->topStat);
      adnsp q = drawImpl(ASP(A_TopStats)->next);
      adesp ep(new ade("topStat", u, p));
      adesp eq(new ade("next", u, q));
      g.nodes.push_back(u);
      g.edges.push_back(ep);
      g.edges.push_back(eq);
      return u;
    }
    case AstKind::CompileUnit: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_CompileUnit)->topStats);
      adesp e(new ade("topStats", u, v));
      g.nodes.push_back(u);
      g.edges.push_back(e);
      return u;
    }
    default:
      LOG_ASSERT(false, "invalid ast kind: {}", ast->kind()._to_string());
    }
  }
};
#endif

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

  DotGraph(const std::vector<std::shared_ptr<_Node>> &a_nodes = {},
           const std::vector<std::shared_ptr<_Edge>> &a_edges = {},
           const std::vector<Cowstr> &a_fontnames = {"Courier New", "Courier"},
           int a_fontsize = 12, const Cowstr &a_shape = "record")
      : nodes(a_nodes), edges(a_edges), fontnames(a_fontnames),
        fontsize(a_fontsize), shape(a_shape) {}

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
    fwriter.writeln(fmt::format("    graph [fontname=\"{}\", fontsize={}]",
                                fontnames_str, fontsize));
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
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    (ast)->name().replace(HtmlTranslator)));                   \
    u->labels.push_back(                                                       \
        adnl("literal", static_cast<atype *>(ast)->literal()));                \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_LOCATION(ast)                                                      \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    (ast)->name().replace(HtmlTranslator)));                   \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_ONE(ast, atype, a)                                                 \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    (ast)->name().replace(HtmlTranslator)));                   \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp v = drawImpl(static_cast<atype *>(ast)->a);                          \
    adesp e(new ade(BOOST_PP_STRINGIZE(a), u, v));                             \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_ONE_OP(ast, atype, op, a)                                          \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    ast->name().replace(HtmlTranslator)));                     \
    u->labels.push_back(adnl(BOOST_PP_STRINGIZE(op),                           \
                             Cowstr(tokenName(static_cast<atype *>(ast)->op))  \
                                 .replace(HtmlTranslator)));                   \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    adnsp v = drawImpl(static_cast<atype *>(ast)->a);                          \
    adesp e(new ade(BOOST_PP_STRINGIZE(a), u, v));                             \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define ADN_TWO_OP(ast, atype, op, a, b)                                       \
  do {                                                                         \
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    ast->name().replace(HtmlTranslator)));                     \
    u->labels.push_back(adnl(BOOST_PP_STRINGIZE(op),                           \
                             Cowstr(tokenName(static_cast<atype *>(ast)->op))  \
                                 .replace(HtmlTranslator)));                   \
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
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    ast->name().replace(HtmlTranslator)));                     \
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
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    ast->name().replace(HtmlTranslator)));                     \
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
    adnsp u(new adn(nameGenerator.from("AstNode"),                             \
                    ast->name().replace(HtmlTranslator)));                     \
    u->labels.push_back(adnl(BOOST_PP_STRINGIZE(op),                           \
                             Cowstr(tokenName(static_cast<atype *>(ast)->op))  \
                                 .replace(HtmlTranslator)));                   \
    u->labels.push_back(adnl("location", (ast)->location().str()));            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

struct AstDotGraph {
  DotGraph<AstDotNode, AstDotEdge> g;

  AstDotGraph() : g() {}
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

} // namespace detail

int Graph::drawAst(Ast *ast, const Cowstr &output) {
  detail::AstDotGraph g;
  return g.draw(ast, output);
}

int Graph::drawSymbol(std::shared_ptr<Symbol> symbol, const Cowstr &output) {
  return 0;
}
