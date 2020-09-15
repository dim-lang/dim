// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Files.h"
#include "Log.h"
#include "Token.h"
#include <list>
#include <sstream>
#include <utility>

#define ASP(x) (static_cast<x *>(ast))

namespace detail {

static const std::unordered_map<char, Cowstr> HtmlTranslator = {
    {'\\', "\\\\"}, {'\n', "\\n"}, {'\t', "\\t"}, {'\r', "\\r"}, {'\?', "\\?"},
    {'\a', "\\a"},  {'\b', "\\b"}, {'\f', "\\f"}, {'\v', "\\v"}, {'\'', "\\\'"},
    {'\"', "\\\""}, {'<', "&lt;"}, {'>', "&gt;"},
};

static AstGraphNameGenerator nameGenerator;

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
  std::list<std::pair<Cowstr, Cowstr>> label;

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
    nodes.push_back(u);                                                        \
    return u;                                                                  \
  } while (0)

#define NODE                                                                   \
  do {                                                                         \
    adnsp u(new adn(ast->name().replace(HtmlTranslator)));                     \
    u->add("location", ast->location().str());                                 \
    nodes.push_back(u);                                                        \
    return u;                                                                  \
  } while (0)

#define S_NODE                                                                 \
  adnsp u(new adn(ast->name().replace(HtmlTranslator)));                       \
  u->add("location", ast->location().str())

struct AstDot {
  std::list<adnsp> nodes;
  std::list<adesp> edges;

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
      nodes.push_back(u);
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
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstKind::Return: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Return)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::PostfixExpr: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "postfixOp", Cowstr(tokenName(ASP(A_PostfixExpr)->postfixOp))
                           .replace(HtmlTranslator)));
      adnsp v = drawImpl(ASP(A_PostfixExpr)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::PrefixExpr: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "prefixOp", Cowstr(tokenName(ASP(A_PrefixExpr)->prefixOp))
                          .replace(HtmlTranslator)));
      adnsp v = drawImpl(ASP(A_PrefixExpr)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstKind::Call: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Call)->id);
      adnsp q = drawImpl(ASP(A_Call)->args);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("args", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::Exprs: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Exprs)->expr);
      adnsp q = drawImpl(ASP(A_Exprs)->next);
      adesp ep(new ade("expr", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      edges.push_back(ev);
      return u;
    }
    case AstKind::Loop: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Loop)->condition);
      adnsp q = drawImpl(ASP(A_Loop)->body);
      adesp ep(new ade("condition", u, p));
      adesp eq(new ade("body", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::Yield: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Yield)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      edges.push_back(ev);
      return u;
    }
    case AstKind::LoopEnumerator: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_LoopEnumerator)->id);
      adnsp q = drawImpl(ASP(A_LoopEnumerator)->expr);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("expr", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::DoWhile: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_DoWhile)->body);
      adnsp q = drawImpl(ASP(A_DoWhile)->condition);
      adesp ep(new ade("body", u, p));
      adesp eq(new ade("condition", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      edges.push_back(ev);
      return u;
    }
    case AstKind::Block: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_Block)->blockStats);
      adesp e(new ade("blockStats", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstKind::BlockStats: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_BlockStats)->blockStat);
      adnsp q = drawImpl(ASP(A_BlockStats)->next);
      adesp ep(new ade("blockStat", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::PlainType: {
      S_NODE;
      u->label.push_back(std::make_pair(
          "token",
          Cowstr(tokenName(ASP(A_PlainType)->token)).replace(HtmlTranslator)));
      nodes.push_back(u);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      edges.push_back(ev);
      return u;
    }
    case AstKind::FuncSign: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_FuncSign)->id);
      adnsp q = drawImpl(ASP(A_FuncSign)->params);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("params", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::Params: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Params)->param);
      adnsp q = drawImpl(ASP(A_Params)->next);
      adesp ep(new ade("param", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::Param: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_Param)->id);
      adnsp q = drawImpl(ASP(A_Param)->type);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("type", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
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
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      edges.push_back(ev);
      return u;
    }
    case AstKind::TopStats: {
      S_NODE;
      adnsp p = drawImpl(ASP(A_TopStats)->topStat);
      adnsp q = drawImpl(ASP(A_TopStats)->next);
      adesp ep(new ade("topStat", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstKind::CompileUnit: {
      S_NODE;
      adnsp v = drawImpl(ASP(A_CompileUnit)->topStats);
      adesp e(new ade("topStats", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    default:
      LOG_ASSERT(false, "invalid ast kind: {}", ast->kind()._to_string());
    }
  }
};

} // namespace detail

int Graph::drawAst(Ast *ast, const Cowstr &output) {
  detail::AstDot g;
  return g.draw(ast, output);
}
