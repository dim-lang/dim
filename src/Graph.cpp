// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Files.h"
#include "Log.h"
#include "Strings.h"
#include "Token.h"
#include <list>
#include <sstream>
#include <utility>

#define ASP(x) (std::static_pointer_cast<x>(ast))

namespace detail {

static const std::unordered_map<char, std::string> HtmlTranslator = {
    {'\\', "\\\\"}, {'\n', "\\n"}, {'\t', "\\t"}, {'\r', "\\r"}, {'\?', "\\?"},
    {'\a', "\\a"},  {'\b', "\\b"}, {'\f', "\\f"}, {'\v', "\\v"}, {'\'', "\\\'"},
    {'\"', "\\\""}, {'<', "&lt;"}, {'>', "&gt;"},
};

static char *getName(const std::string &name) {
  std::stringstream ss;
  ss << name << Counter::get();
  return Strings::dup(ss.str().c_str());
}

struct AstDotNode;
struct AstDotEdge;
struct AstDot;

using adn = AstDotNode;
using ade = AstDotEdge;
using adnsp = std::shared_ptr<AstDotNode>;
using adesp = std::shared_ptr<AstDotEdge>;

struct AstDotNode {
  std::string name;
  std::string header;
  std::list<std::pair<char *, char *>> label;

  AstDotNode(const std::string &a_header)
      : name(getName("node")),
        header(Strings::replace(a_header, HtmlTranslator)), label() {}
  virtual ~AstDotNode() = default;

  virtual AstDotNode &add(char *a, char *b) {
    label.push_back(std::make_pair(Strings::replace(a, HtmlTranslator),
                                   Strings::replace(b, HtmlTranslator)));
    return *this;
  }
  virtual AstDotNode &add(const std::string &a, const std::string &b) {
    label.push_back(std::make_pair(Strings::replace(a, HtmlTranslator),
                                   Strings::replace(b, HtmlTranslator)));
    return *this;
  }
  virtual std::string toString() const {
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
  std::string label;
  adnsp head;
  adnsp tail;

  AstDotEdge(const std::string &a_label, adnsp a_head, adnsp a_tail)
      : label(a_label), head(a_head), tail(a_tail) {}
  virtual ~AstDotEdge() = default;

  virtual std::string toString() const {
    std::stringstream ss;
    ss << head->name << " -> " << tail->name << " [label=\"" << label << "\"]";
    return ss.str();
  }
};

#define LITERAL_NODE(ast_type)                                                 \
  do {                                                                         \
    adnsp u(new adn(ast->name().symbolName()));                                \
    u->add("literal", ASP(ast_type)->literal());                               \
    u->add("location", ast->location().toString());                            \
    u->add("llvmName", ast->name().llvmName());                                \
    nodes.push_back(u);                                                        \
    return u;                                                                  \
  } while (0)

#define NODE                                                                   \
  do {                                                                         \
    adnsp u(new adn(ast->name().symbolName()));                                \
    u->add("location", ast->location().toString());                            \
    nodes.push_back(u);                                                        \
    return u;                                                                  \
  } while (0)

#define SYM_NODE adnsp u(new adn(ast->name().symbolName()))

struct AstDot {
  std::list<adnsp> nodes;
  std::list<adesp> edges;

  AstDot() : nodes(), edges() {}
  virtual ~AstDot() = default;

  virtual int draw(std::shared_ptr<Ast> ast, const std::string &output) {
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
      fwriter.writeln(fmt::format("    {}", (*i)->toString()));
    }
    fwriter.write("\n");
    for (auto i = edges.begin(); i != edges.end(); i++) {
      fwriter.writeln(fmt::format("    {}", (*i)->toString()));
    }
    fwriter.writeln("}");
    fwriter.flush();
    return 0;
  }

private:
  virtual adnsp drawImpl(std::shared_ptr<Ast> ast) {
    if (!ast) {
      adnsp u(new adn("nil"));
      nodes.push_back(u);
      return u;
    }
    if (Ast::isLiteral(ast)) {
      switch (ast->category()) {
      case AstCategory::Integer: {
        LITERAL_NODE(A_Integer);
      }
      case AstCategory::Float: {
        LITERAL_NODE(A_Float);
      }
      case AstCategory::Boolean: {
        LITERAL_NODE(A_Boolean);
      }
      case AstCategory::Character: {
        LITERAL_NODE(A_Character);
      }
      case AstCategory::String: {
        LITERAL_NODE(A_String);
      }
      case AstCategory::Nil: {
        NODE;
      }
      case AstCategory::Void: {
        NODE;
      }
      default:
        LOG_ASSERT(false, "invalid literal type: {}",
                   ast->category()._to_string());
      }
    }
    switch (ast->category()) {
    case AstCategory::VarId: {
      LITERAL_NODE(A_VarId);
    }
    case AstCategory::Break: {
      NODE;
    }
    case AstCategory::Continue: {
      NODE;
    }
    case AstCategory::Throw: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_Throw)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstCategory::Return: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_Return)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstCategory::Assign: {
      SYM_NODE;
      u->label.push_back(std::make_pair(
          Strings::dup("assignOp"),
          Strings::replace(tokenName(ASP(A_Assign)->assignOp).c_str(),
                           HtmlTranslator)));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_Assign)->assignee);
      adnsp q = drawImpl(ASP(A_Assign)->assignor);
      adesp ep(new ade("assignee", u, p));
      adesp eq(new ade("assignor", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::PostfixExpr: {
      SYM_NODE;
      u->label.push_back(std::make_pair(
          Strings::dup("postfixOp"),
          Strings::replace(tokenName(ASP(A_PostfixExpr)->postfixOp).c_str(),
                           HtmlTranslator)));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_PostfixExpr)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstCategory::InfixExpr: {
      SYM_NODE;
      u->label.push_back(std::make_pair(
          Strings::dup("infixOp"),
          Strings::replace(tokenName(ASP(A_InfixExpr)->infixOp).c_str(),
                           HtmlTranslator)));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_InfixExpr)->left);
      adnsp q = drawImpl(ASP(A_InfixExpr)->right);
      adesp ep(new ade("left", u, p));
      adesp eq(new ade("right", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::PrefixExpr: {
      SYM_NODE;
      u->label.push_back(std::make_pair(
          Strings::dup("prefixOp"),
          Strings::replace(tokenName(ASP(A_PrefixExpr)->prefixOp).c_str(),
                           HtmlTranslator)));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_PrefixExpr)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstCategory::Call: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_Call)->id);
      adnsp q = drawImpl(ASP(A_Call)->args);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("args", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::Exprs: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_Exprs)->expr);
      adnsp q = drawImpl(ASP(A_Exprs)->next);
      adesp ep(new ade("expr", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::If: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
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
    case AstCategory::Loop: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_Loop)->condition);
      adnsp q = drawImpl(ASP(A_Loop)->body);
      adesp ep(new ade("condition", u, p));
      adesp eq(new ade("body", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::Yield: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_Yield)->expr);
      adesp e(new ade("expr", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstCategory::LoopCondition: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
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
    case AstCategory::LoopEnumerator: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_LoopEnumerator)->id);
      adnsp q = drawImpl(ASP(A_LoopEnumerator)->expr);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("expr", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::DoWhile: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_DoWhile)->body);
      adnsp q = drawImpl(ASP(A_DoWhile)->condition);
      adesp ep(new ade("body", u, p));
      adesp eq(new ade("condition", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::Try: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
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
    case AstCategory::Block: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_Block)->blockStats);
      adesp e(new ade("blockStats", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    case AstCategory::BlockStats: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_BlockStats)->blockStat);
      adnsp q = drawImpl(ASP(A_BlockStats)->next);
      adesp ep(new ade("blockStat", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::PlainType: {
      SYM_NODE;
      u->label.push_back(std::make_pair(
          Strings::dup("token"),
          Strings::replace(tokenName(ASP(A_PlainType)->token).c_str(),
                           HtmlTranslator)));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      nodes.push_back(u);
      return u;
    }
    case AstCategory::FuncDef: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
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
    case AstCategory::FuncSign: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_FuncSign)->id);
      adnsp q = drawImpl(ASP(A_FuncSign)->params);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("params", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::Params: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_Params)->param);
      adnsp q = drawImpl(ASP(A_Params)->next);
      adesp ep(new ade("param", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::Param: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_Param)->id);
      adnsp q = drawImpl(ASP(A_Param)->type);
      adesp ep(new ade("id", u, p));
      adesp eq(new ade("type", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::VarDef: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
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
    case AstCategory::TopStats: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp p = drawImpl(ASP(A_TopStats)->topStat);
      adnsp q = drawImpl(ASP(A_TopStats)->next);
      adesp ep(new ade("topStat", u, p));
      adesp eq(new ade("next", u, q));
      nodes.push_back(u);
      edges.push_back(ep);
      edges.push_back(eq);
      return u;
    }
    case AstCategory::CompileUnit: {
      SYM_NODE;
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      adnsp v = drawImpl(ASP(A_CompileUnit)->topStats);
      adesp e(new ade("topStats", u, v));
      nodes.push_back(u);
      edges.push_back(e);
      return u;
    }
    default:
      LOG_ASSERT(false, "invalid ast category: {}",
                 ast->category()._to_string());
    }
  }
};

} // namespace detail

int Graph::drawAst(std::shared_ptr<Ast> ast, const std::string &output) {
  detail::AstDot g;
  return g.draw(ast, output);
}
