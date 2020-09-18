// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Log.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include "container/LinkedHashMap.hpp"

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

static Cowstr nameGenerate(const Cowstr &name, const Cowstr &delimiter = "") {
  static Counter counter_;
  std::stringstream ss;
  ss << name << delimiter << counter_.next();
  return ss.str();
}

GraphLine::GraphLine(const std::vector<Cowstr> &a_cells)
    : id(nameGenerate("cell")) {
  for (int i = 0; i < (int)a_cells.size(); i++) {
    cells.push_back(TRANSLATE(a_cells[i]));
  }
}

void GraphLine::add(const Cowstr &cell) { cells.push_back(TRANSLATE(cell)); }

Cowstr GraphLine::str() const {
  std::stringstream ss;
  ss << "        <TR><TD PORT=\"" << id << "\">";
  if (cells.size() != 0) {
    ss << cells[0];
    if (cells.size() > 1) {
      ss << ": ";
      for (int i = 1; i < (int)cells.size(); i++) {
        ss << cells[i];
        if (i < (int)cells.size() - 1) {
          ss << ", ";
        }
      }
    }
  }
  ss << "</TD></TR>";
  return ss.str();
}

GraphLabel::GraphLabel(const std::vector<GraphLine> &a_lines,
                       const std::unordered_map<Cowstr, Cowstr> &a_attributes)
    : lines(a_lines), attributes(a_attributes) {}

const Cowstr &GraphLabel::id() const {
  LOG_ASSERT(!lines.empty(), "lines must not empty");
  return lines[0].id;
}

Cowstr GraphLabel::str() const {
  std::stringstream ss;
  ss << "<TABLE";
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
  ss << "</TABLE>";
  return ss.str();
}

GraphNode::GraphNode(const std::vector<GraphLine> &a_lines,
                     const std::unordered_map<Cowstr, Cowstr> &a_attributes)
    : id(nameGenerate("node")), label(new GraphLabel(a_lines, a_attributes)) {}

Cowstr GraphNode::str() const {
  std::stringstream ss;
  ss << id.str();
  ss << " [label=<";
  ss << label->str();
  ss << ">]";
  return ss.str();
}

GraphEdge::GraphEdge(const Cowstr &from, const Cowstr &to,
                     std::shared_ptr<GraphLabel> label)
    : from_(from), to_(to), label_(label) {}

GraphEdge::GraphEdge(const Cowstr &fromNode, const Cowstr &fromField,
                     const Cowstr &toNode, const Cowstr &toField,
                     std::shared_ptr<GraphLabel> label)
    : from_(fmt::format("{}:{}", fromNode, fromField)),
      to_(fmt::format("{}:{}", toNode, toField)), label_(label) {}

Cowstr GraphEdge::str() const {
  std::stringstream ss;
  ss << from_ << " -> " << to_;
  if (label_) {
    ss << " [label=<";
    ss << label_->str();
    ss << " >]";
  }
  return ss.str();
}

Graph::Graph(
    const Cowstr &fileName,
    const std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>>
        &a_attributes)
    : attributes(a_attributes), fileName_(fileName) {}

int Graph::draw() {
  FileWriter fwriter(fileName_);
  fwriter.writeln("digraph {");
  for (auto i = attributes.begin(); i != attributes.end(); i++) {
    fwriter.write(fmt::format("    {} [", i->first));
    int c = 0;
    for (auto j = i->second.begin(); j != i->second.end(); j++, c++) {
      fwriter.write(fmt::format("{}={}", j->first, j->second));
      if (c < (int)i->second.size() - 1) {
        fwriter.write(", ");
      }
    }
    fwriter.writeln("]");
  }
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

using attrmap = std::unordered_map<Cowstr, Cowstr>;

using gn = GraphNode;
using gnsp = std::shared_ptr<gn>;
using ge = GraphEdge;
using gesp = std::shared_ptr<GraphEdge>;
using glabel = GraphLabel;
using glabelsp = std::shared_ptr<glabel>;
using gline = GraphLine;
using glinesp = std::shared_ptr<gline>;

const static attrmap gnode_attr = {
    {"shape", "record"},
    {"fontname", "\"Courier New, Courier\""},
    {"fontsize", "12"},
};

const static attrmap gedge_attr = {
    {"fontname", "\"Courier New, Courier\""},
    {"fontsize", "12"},
};

const static attrmap ggraph_attr = {
    {"fontname", "\"Courier New, Courier\""},
    {"fontsize", "12"},
};

const static attrmap agelabel_attr = {
    {"BORDER", "\"0\""},
    {"CELLBORDER", "\"0\""},
    {"CELLSPACING", "\"0\""},
};

static gline fromCell(const Cowstr &a) {
  gline l;
  l.add(a);
  return l;
}

static gline fromCell(const Cowstr &a, const Cowstr &b) {
  gline l;
  l.add(a);
  l.add(b);
  return l;
}

#define AE_LABEL(x)                                                            \
  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(x))}, agelabel_attr))

#define AG_NIL                                                                 \
  do {                                                                         \
    gnsp u(new gn({fromCell("nil")}));                                         \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AG_LITERAL(ast, atype)                                                 \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell("literal", static_cast<atype *>(ast)->literal()),  \
                   fromCell("location", ast->location().str())}));             \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AG_LOCATION(ast)                                                       \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell("location", ast->location().str())}));             \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AGOP(ast, atype, op)                                                   \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell(BOOST_PP_STRINGIZE(op),                            \
                            tokenName(static_cast<atype *>(ast)->op)),         \
                   fromCell("location", ast->location().str())}));             \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AG1(ast, atype, a)                                                     \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell("location", ast->location().str())}));             \
    gnsp v = astDrawImpl(static_cast<atype *>(ast)->a, g);                     \
    gesp e(new ge(u->id, v->id));                                              \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define AGOP1(ast, atype, a, op)                                               \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell(BOOST_PP_STRINGIZE(op),                            \
                            tokenName(static_cast<atype *>(ast)->op)),         \
                   fromCell("location", ast->location().str())}));             \
    gnsp v = astDrawImpl(static_cast<atype *>(ast)->a, g);                     \
    gesp e(new ge(u->id, v->id, AE_LABEL(a)));                                 \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define AG2(ast, atype, a, b)                                                  \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell("location", ast->location().str())}));             \
    gnsp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                     \
    gnsp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                     \
    gesp ep(new ge(u->id, p->id, AE_LABEL(a)));                                \
    gesp eq(new ge(u->id, q->id, AE_LABEL(b)));                                \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define AGOP2(ast, atype, a, b, op)                                            \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell(BOOST_PP_STRINGIZE(op),                            \
                            tokenName(static_cast<atype *>(ast)->op)),         \
                   fromCell("location", ast->location().str())}));             \
    gnsp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                     \
    gnsp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                     \
    gesp ep(new ge(u->id, p->id, AE_LABEL(a)));                                \
    gesp eq(new ge(u->id, q->id, AE_LABEL(b)));                                \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define AG3(ast, atype, a, b, c)                                               \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell("location", ast->location().str())}));             \
    gnsp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                     \
    gnsp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                     \
    gnsp v = astDrawImpl(static_cast<atype *>(ast)->c, g);                     \
    gesp ep(new ge(u->id, p->id, AE_LABEL(a)));                                \
    gesp eq(new ge(u->id, q->id, AE_LABEL(b)));                                \
    gesp ev(new ge(u->id, v->id, AE_LABEL(c)));                                \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

#define AGOP3(ast, atype, a, b, c, op)                                         \
  do {                                                                         \
    gnsp u(new gn({fromCell(ast->name()),                                      \
                   fromCell(BOOST_PP_STRINGIZE(op),                            \
                            tokenName(static_cast<atype *>(ast)->op)),         \
                   fromCell("location", ast->location().str())}));             \
    gnsp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                     \
    gnsp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                     \
    gnsp v = astDrawImpl(static_cast<atype *>(ast)->c, g);                     \
    gesp ep(new ge(u->id, p->id, AE_LABEL(a)));                                \
    gesp eq(new ge(u->id, q->id, AE_LABEL(b)));                                \
    gesp ev(new ge(u->id, v->id, AE_LABEL(c)));                                \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

static gnsp astDrawImpl(Ast *ast, Graph &g) {
  if (!ast) {
    AG_NIL;
  }
  switch (ast->kind()) {
  case AstKind::Integer: {
    AG_LITERAL(ast, A_Integer);
  }
  case AstKind::Float: {
    AG_LITERAL(ast, A_Float);
  }
  case AstKind::Boolean: {
    AG_LITERAL(ast, A_Boolean);
  }
  case AstKind::Character: {
    AG_LITERAL(ast, A_Character);
  }
  case AstKind::String: {
    AG_LITERAL(ast, A_String);
  }
  case AstKind::Nil: {
    AG_LOCATION(ast);
  }
  case AstKind::Void: {
    AG_LOCATION(ast);
  }
  case AstKind::VarId: {
    AG_LITERAL(ast, A_VarId);
  }
  case AstKind::Break: {
    AG_LOCATION(ast);
  }
  case AstKind::Continue: {
    AG_LOCATION(ast);
  }
  case AstKind::Throw: {
    AG1(ast, A_Throw, expr);
  }
  case AstKind::Return: {
    AG1(ast, A_Return, expr);
  }
  case AstKind::Assign: {
    AGOP2(ast, A_Assign, assignee, assignor, assignOp);
  }
  case AstKind::PostfixExpr: {
    AGOP1(ast, A_PostfixExpr, expr, postfixOp);
  }
  case AstKind::InfixExpr: {
    AGOP2(ast, A_InfixExpr, left, right, infixOp);
  }
  case AstKind::PrefixExpr: {
    AGOP1(ast, A_PrefixExpr, expr, prefixOp);
  }
  case AstKind::Call: {
    AG2(ast, A_Call, id, args);
  }
  case AstKind::Exprs: {
    AG2(ast, A_Exprs, expr, next);
  }
  case AstKind::If: {
    AG3(ast, A_If, condition, thenp, elsep);
  }
  case AstKind::Loop: {
    AG2(ast, A_Loop, condition, body);
  }
  case AstKind::Yield: {
    AG1(ast, A_Yield, expr);
  }
  case AstKind::LoopCondition: {
    AG3(ast, A_LoopCondition, init, condition, update);
  }
  case AstKind::LoopEnumerator: {
    AG2(ast, A_LoopEnumerator, id, expr);
  }
  case AstKind::DoWhile: {
    AG2(ast, A_DoWhile, body, condition);
  }
  case AstKind::Try: {
    AG3(ast, A_Try, tryp, catchp, finallyp);
  }
  case AstKind::Block: {
    AG1(ast, A_Block, blockStats);
  }
  case AstKind::BlockStats: {
    AG2(ast, A_BlockStats, blockStat, next);
  }
  case AstKind::PlainType: {
    AGOP(ast, A_PlainType, token);
  }
  case AstKind::FuncDef: {
    AG3(ast, A_FuncDef, funcSign, resultType, body);
  }
  case AstKind::FuncSign: {
    AG2(ast, A_FuncSign, id, params);
  }
  case AstKind::Params: {
    AG2(ast, A_Params, param, next);
  }
  case AstKind::Param: {
    AG2(ast, A_Param, id, type);
  }
  case AstKind::VarDef: {
    AG3(ast, A_VarDef, id, type, expr);
  }
  case AstKind::TopStats: {
    AG2(ast, A_TopStats, topStat, next);
  }
  case AstKind::CompileUnit: {
    AG1(ast, A_CompileUnit, topStats);
  }
  default:
    LOG_ASSERT(false, "invalid ast kind: {}", ast->kind()._to_string());
  }
}

AstGraph::AstGraph(Ast *ast) : ast_(ast) {}

int AstGraph::draw(const Cowstr &output) {
  std::unordered_map<Cowstr, attrmap> gattr = {
      {"node", gnode_attr},
      {"edge", gedge_attr},
      {"graph", ggraph_attr},
  };
  Graph g(output, gattr);
  astDrawImpl(ast_, g);
  return g.draw();
}

using scsp = std::shared_ptr<Scope>;

static GraphLine drawSymbol(std::shared_ptr<Symbol> sym) {
  if (!sym) {
    return fromCell("nil");
  }
  gline l;
  l.add("symbol");
  l.add(fmt::format("name: {}", sym->name()));
  if (sym->owner()) {
    SymbolData sdata = sym->owner()->s_resolve(sym->name());
    l.add(fmt::format("type: {}", sdata.typeSymbol->name()));
  } else {
    l.add("type: void");
  }
  l.add(fmt::format("kind: {}", sym->kind()._to_string()));
  return l;
}

static GraphLine drawTypeSymbol(std::shared_ptr<TypeSymbol> sym) {
  if (!sym) {
    return fromCell("nil");
  }
  gline l;
  l.add(fmt::format("typeSymbol"));
  l.add(fmt::format("name: {}", sym->name()));
  l.add(fmt::format("kind: {}", sym->kind()._to_string()));
  return l;
}

static gnsp symbolDrawImpl(scsp scope, Graph &g) {
  if (!scope) {
    AG_NIL;
  }
  gnsp u(new gn({fromCell(scope->name())}));
  g.nodes.push_back(u);
  // draw symbol
  for (auto i = scope->s_begin(); i != scope->s_end(); i++) {
    SymbolData sdata = i->second;
    LOG_ASSERT(sdata.symbol, "sdata.symbol must not null");
    switch (sdata.symbol->kind()) {
    case SymbolKind::Var:
    case SymbolKind::Param:
    case SymbolKind::Field: {
      u->label->lines.push_back(drawSymbol(sdata.symbol));
      break;
    }
    case SymbolKind::Func:
    case SymbolKind::Method:
    case SymbolKind::Local:
    case SymbolKind::Global: {
      gline l = drawSymbol(sdata.symbol);
      u->label->lines.push_back(l);
      gnsp v =
          symbolDrawImpl(std::dynamic_pointer_cast<Scope>(sdata.symbol), g);
      gesp e(new ge(u->id, l.id, v->id, v->label->id()));
      g.edges.push_back(e);
      break;
    }
    default:
      LOG_ASSERT(false, "invalid sdata.symbol type: {}",
                 sdata.symbol->kind()._to_string());
    }
  }
  // draw type symbol
  for (auto i = scope->ts_begin(); i != scope->ts_end(); i++) {
    TypeSymbolData tsdata = i->second;
    u->label->lines.push_back(drawTypeSymbol(tsdata.typeSymbol));
  }
  return u;
}

SymbolGraph::SymbolGraph(std::shared_ptr<Scope> scope) : scope_(scope) {}

int SymbolGraph::draw(const Cowstr &output) {
  std::unordered_map<Cowstr, attrmap> gattr = {
      {"node", gnode_attr},
      {"edge", gedge_attr},
      {"graph", ggraph_attr},
  };
  Graph g(output, gattr);
  symbolDrawImpl(scope_, g);
  return g.draw();
}
