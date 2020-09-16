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

struct SymbolDotNodeLabel {
  Cowstr id;
  Cowstr title;
  Cowstr value;

  SymbolDotNodeLabel(const Cowstr &a_id, const Cowstr &a_title,
                     const Cowstr &a_value)
      : id(Cowstr(a_id).replace(HtmlTranslator)),
        title(Cowstr(a_title).replace(HtmlTranslator)),
        value(Cowstr(a_value).replace(HtmlTranslator)) {}
  virtual ~SymbolDotNodeLabel() = default;
  virtual Cowstr str() const {
    return fmt::format("<{}> {}: {}", id, title, value);
  }
};

struct SymbolDotNode {
  Cowstr id;
  Cowstr headerId;
  Cowstr header;
  std::vector<SymbolDotNodeLabel> labels;

  SymbolDotNode(const Cowstr &a_id, const Cowstr &a_headerId,
                const Cowstr &a_header)
      : id(Cowstr(a_id).replace(HtmlTranslator)),
        headerId(Cowstr(a_headerId).replace(HtmlTranslator)),
        header(Cowstr(a_header).replace(HtmlTranslator)) {}
  virtual ~SymbolDotNode() = default;
  virtual Cowstr str() const {
    std::stringstream ss;
    ss << id << " [label=\"<" << headerId << "> " << header;
    for (auto i = labels.begin(); i != labels.end(); i++) {
      ss << " | " << i->str();
    }
    ss << "\"]";
    return ss.str();
  }
};

struct SymbolDotEdge {
  Cowstr headId;
  Cowstr headLabelId;
  Cowstr tailId;
  Cowstr tailLabelId;

  SymbolDotEdge(const Cowstr &a_headId, const Cowstr &a_headLabelId,
                const Cowstr &a_tailId, const Cowstr &a_tailLabelId)
      : headId(a_headId), headLabelId(a_headLabelId), tailId(a_tailId),
        tailLabelId(a_tailLabelId) {}
  virtual ~SymbolDotEdge() = default;
  virtual Cowstr str() const {
    return fmt::format("{}:{} -> {}:{}", headId, headLabelId, tailId,
                       tailLabelId);
  }
};

using sdnl = SymbolDotNodeLabel;
using sdn = SymbolDotNode;
using sde = SymbolDotEdge;
using sdnsp = std::shared_ptr<SymbolDotNode>;
using sdesp = std::shared_ptr<SymbolDotEdge>;

#define SDN_NIL                                                                \
  do {                                                                         \
    sdnsp u(new sdn(nameGenerator.from("SymbolNode"),                          \
                    nameGenerator.from("field"), "nil"));                      \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define SDN_SYMBOL(scope)                                                      \
  do {                                                                         \
    sdnsp u(new sdn(nameGenerator.from("SymbolNode"),                          \
                    nameGenerator.from("field"), (scope)->name()));            \
    u->labels.push_back(sdnl(nameGenerator.from("field"), "kind",              \
                             (scope)->kind()._to_string()));                   \
    u->labels.push_back(                                                       \
        sdnl(nameGenerator.from("field"), "name", (scope)->name()));           \
    if ((scope)->owner()) {                                                    \
      SymbolData sdata = (scope)->owner()->s_resolve((scope)->name());         \
      u->labels.push_back(sdnl(nameGenerator.from("field"), "type",            \
                               sdata.typeSymbol->name()));                     \
    } else {                                                                   \
      u->labels.push_back(sdnl(nameGenerator.from("field"), "type", "nil"));   \
    }                                                                          \
    u->labels.push_back(sdnl(nameGenerator.from("field"), "location",          \
                             (scope)->location().str()));                      \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

struct SymbolDotGraph {
  DotGraph<SymbolDotNode, SymbolDotEdge> g;

  SymbolDotGraph() {}
  virtual ~SymbolDotGraph() = default;

  virtual int draw(std::shared_ptr<Scope> scope, const Cowstr &output) {
    drawImpl(scope);
    return g.draw(output);
  }

  virtual std::shared_ptr<SymbolDotNode>
  drawImpl(std::shared_ptr<Scope> scope) {
    if (!scope) {
      SDN_NIL;
    }
    if (scope->isSymbol()) {
      std::shared_ptr<Symbol> sym = std::dynamic_pointer_cast<Symbol>(scope);
      switch (sym->kind()) {
      case SymbolKind::Var: {
        SDN_SYMBOL(sym);
      }
      case SymbolKind::Param: {
        SDN_SYMBOL(sym);
      }
      case SymbolKind::Field: {
        SDN_SYMBOL(sym);
      }
      case SymbolKind::Func: {
        SDN_SYMBOL(sym);
      }
      case SymbolKind::Method: {
        SDN_SYMBOL(sym);
      }
      case SymbolKind::Local: {
        SDN_SYMBOL(sym);
      }
      case SymbolKind::Global: {
        SDN_SYMBOL(sym);
      }
      default:
        LOG_ASSERT(false, "invalid symbol kind: {}", sym->kind()._to_string());
      }
    } else if (scope->isTypeSymbol()) {
      std::shared_ptr<TypeSymbol> sym =
          std::dynamic_pointer_cast<TypeSymbol>(scope);
      switch (sym->kind()) {
      case TypeSymbolKind::Plain: {
        SDN_SYMBOL(sym);
      }
      case TypeSymbolKind::Func: {
        SDN_SYMBOL(sym);
      }
      case TypeSymbolKind::Class: {
        SDN_SYMBOL(sym);
      }
      default:
        LOG_ASSERT(false, "invalid type symbol kind: {}",
                   sym->kind()._to_string());
      }
    } else {
      LOG_ASSERT(false, "invalid scope type");
    }
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
