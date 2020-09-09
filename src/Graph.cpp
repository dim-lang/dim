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
      : name(getName("node")), header(a_header), label() {}
  virtual ~AstDotNode() = default;

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

static const std::unordered_map<char, std::string> HtmlTranslator = {
    {'\\', "\\\\"}, {'\n', "\\n"}, {'\t', "\\t"}, {'\r', "\\r"}, {'\?', "\\?"},
    {'\a', "\\a"},  {'\b', "\\b"}, {'\f', "\\f"}, {'\v', "\\v"}, {'\'', "\\\'"},
    {'\"', "\\\""}, {'<', "&lt;"}, {'>', "&gt;"},
};

#define LITERAL_ATTR(header)                                                   \
  do {                                                                         \
    adnsp u(new adn(header));                                                  \
    u->label.push_back(                                                        \
        std::make_pair(Strings::dup("literal"),                                \
                       Strings::replace(ast->name().raw(), HtmlTranslator)));  \
    u->label.push_back(                                                        \
        std::make_pair(Strings::dup("location"),                               \
                       Strings::dup(ast->location().toString().c_str())));     \
    u->label.push_back(std::make_pair(Strings::dup("symbolName"),              \
                                      (char *)ast->name().toSymbolName()));    \
    u->label.push_back(std::make_pair(Strings::dup("llvmName"),                \
                                      (char *)ast->name().toLLVMName()));      \
    nodes.push_back(u);                                                        \
    return u;                                                                  \
  } while (0)

struct AstDot {
  std::list<adnsp> nodes;
  std::list<adesp> edges;

  AstDot() : nodes(), edges() {}
  virtual ~AstDot() = default;

  virtual int draw(std::shared_ptr<Ast> ast, const std::string &output) {
    FileWriter fwriter(output);
    drawImpl(ast);
    fwriter.write("digraph {\n");
    fwriter.write("    node [shape=record]\n\n");
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      fwriter.write(fmt::format("    {}\n", (*i)->toString()));
    }
    fwriter.write("\n");
    for (auto i = edges.begin(); i != edges.end(); i++) {
      fwriter.write(fmt::format("    {}\n", (*i)->toString()));
    }
    fwriter.write("}\n");
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
        LITERAL_ATTR("integer_literal");
      }
      case AstCategory::Float: {
        LITERAL_ATTR("float_literal");
      }
      case AstCategory::Boolean: {
        LITERAL_ATTR("boolean_literal");
      }
      case AstCategory::Character: {
        LITERAL_ATTR("character_literal");
      }
      case AstCategory::String: {
        LITERAL_ATTR("string_literal");
      }
      case AstCategory::Nil: {
        LITERAL_ATTR("nil");
      }
      case AstCategory::Void: {
        LITERAL_ATTR("void");
      }
      default:
        LOG_ASSERT(false, "invalid literal type: {}",
                   ast->category()._to_string());
      }
    }
    switch (ast->category()) {
    case AstCategory::VarId: {
      adnsp u(new adn("var_id"));
      u->label.push_back(
          std::make_pair(Strings::dup("literal"),
                         Strings::replace(ast->name().raw(), HtmlTranslator)));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      u->label.push_back(std::make_pair(Strings::dup("symbolName"),
                                        (char *)ast->name().toSymbolName()));
      u->label.push_back(std::make_pair(Strings::dup("llvmName"),
                                        (char *)ast->name().toLLVMName()));
      nodes.push_back(u);
      return u;
    }
    case AstCategory::Break: {
      adnsp u(new adn("break"));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      nodes.push_back(u);
      return u;
    }
    case AstCategory::Continue: {
      adnsp u(new adn("continue"));
      u->label.push_back(
          std::make_pair(Strings::dup("location"),
                         Strings::dup(ast->location().toString().c_str())));
      nodes.push_back(u);
      return u;
    }
    case AstCategory::Throw: {
      adnsp u(new adn("throw"));
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
      adnsp u(new adn("return"));
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
      adnsp u(new adn("assign"));
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
      adnsp u(new adn("postfix"));
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
      adnsp u(new adn("infix"));
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
      adnsp u(new adn("prefix"));
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
      adnsp u(new adn("call"));
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
      adnsp u(new adn("exprs"));
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
      adnsp u(new adn("if"));
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
      adnsp u(new adn("loop"));
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
      adnsp u(new adn("yield"));
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
      adnsp u(new adn("loop_condition"));
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
      adnsp u(new adn("loop_enumerator"));
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
      adnsp u(new adn("do_while"));
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
      adnsp u(new adn("try"));
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
      adnsp u(new adn("block"));
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
      adnsp u(new adn("block_stats"));
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
      adnsp u(new adn("plain_type"));
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
      adnsp u(new adn("func_def"));
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
      adnsp u(new adn("func_sign"));
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
      adnsp u(new adn("params"));
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
      adnsp u(new adn("param"));
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
      adnsp u(new adn("var_def"));
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
      adnsp u(new adn("top_stats"));
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
      adnsp u(new adn("compile_unit"));
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
