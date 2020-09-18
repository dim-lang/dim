// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Log.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"

static const std::unordered_map<char, Cowstr> HtmlTranslator = {
    {'\\', "\\\\"}, {'\n', "\\n"},  {'\t', "\\t"},  {'\r', "\\r"},
    {'\?', "\\?"},  {'\a', "\\a"},  {'\b', "\\b"},  {'\f', "\\f"},
    {'\v', "\\v"},  {'\'', "\\\'"}, {'\"', "\\\""}, {'<', "&lt;"},
    {'>', "&gt;"},  {'|', "&#124;"}};

#define TRANSLATE(x) (Cowstr(x).replace(HtmlTranslator))

static Cowstr nameGenerate(const Cowstr &name, const Cowstr &delimiter = "") {
  static Counter counter_;
  std::stringstream ss;
  ss << name << delimiter << counter_.next();
  return ss.str();
}

GraphCell::GraphCell(const Cowstr &value)
    : id_(nameGenerate("cell")), value_(TRANSLATE(value)) {}

Cowstr GraphCell::str() const {
  return fmt::format("<TD PORT=\"{}\">{}</TD>", id_, value_);
}

GraphLine::GraphLine(const std::vector<GraphCell> &cells) : cells_(cells) {}

void GraphLine::add(const GraphCell &cell) { cells_.push_back(cell); }

Cowstr GraphLine::str() const {
  std::stringstream ss;
  ss << "<TR> ";
  for (int i = 0; i < (int)cells_.size(); i++) {
    ss << cells_[i].str();
  }
  ss << " </TR>";
  return ss.str();
}

GraphLabel::GraphLabel(const std::vector<GraphLine> &table,
                       const std::unordered_map<Cowstr, Cowstr> &attributes)
    : table_(table) {
  for (auto i = attributes.begin(); i != attributes.end(); i++) {
    attributes_.insert(
        std::make_pair(TRANSLATE(i->first), TRANSLATE(i->second)));
  }
}

void GraphLabel::add(const GraphLine &line) { table_.push_back(line); }

Cowstr GraphLabel::str() const {
  std::stringstream ss;
  ss << "<TABLE ";
  for (auto i = attributes_.begin(); i != attributes_.end(); i++) {
    ss << " " << i->first << "=" << i->second;
  }
  ss << ">";
  for (int i = 0; i < (int)table_.size(); i++) {
    ss << table_[i].str();
    if (i < (int)table_.size() - 1) {
      ss << "\n";
    }
  }
  ss << "</TABLE>";
  return ss.str();
}

GraphNode::GraphNode(std::shared_ptr<GraphLabel> label)
    : id_(nameGenerate("node")), label_(label) {}

GraphNode::GraphNode(const std::vector<GraphLine> &table,
                     const std::unordered_map<Cowstr, Cowstr> &attributes)
    : id_(nameGenerate("node")), label_(new GraphLabel(table, attributes)) {}

Cowstr GraphNode::str() const {
  std::stringstream ss;
  ss << id_.str();
  if (label_) {
    ss << " [label=<";
    ss << label_->str();
    ss << ">]";
  }
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
        &attributes)
    : fileName_(fileName) {
  for (auto i = attributes.begin(); i != attributes.end(); i++) {
    std::unordered_map<Cowstr, Cowstr> temp;
    for (auto j = i->second.begin(); j != i->second.end(); j++) {
      temp.insert(std::make_pair(TRANSLATE(j->first), TRANSLATE(j->second)));
    }
    attributes_.insert(std::make_pair(TRANSLATE(i->first), temp));
  }
}

void Graph::addNode(std::shared_ptr<GraphNode> node) { nodes_.push_back(node); }

void Graph::addEdge(std::shared_ptr<GraphEdge> edge) { edges_.push_back(edge); }

int Graph::draw() {
  FileWriter fwriter(fileName_);
  fwriter.writeln("digraph {");
  for (auto i = attributes_.begin(); i != attributes_.end(); i++) {
    fwriter.write(fmt::format("    {} [", i->first));
    int c = 0;
    for (auto j = i->second.begin(); j != i->second.end(); j++, c++) {
      fwriter.write(fmt::format("{}={}", j->first, j->second));
      if (c < (int)i->second.size() - 1) {
        fwriter.write(", ");
      }
    }
    fwriter.writeln("    ]");
  }
  fwriter.writeln();
  for (auto i = nodes_.begin(); i != nodes_.end(); i++) {
    fwriter.writeln(fmt::format("    {}", (*i)->str()));
  }
  fwriter.write("\n");
  for (auto i = edges_.begin(); i != edges_.end(); i++) {
    fwriter.writeln(fmt::format("    {}", (*i)->str()));
  }
  fwriter.writeln("}");
  fwriter.flush();
  return 0;
}

using attrmap = std::unordered_map<Cowstr, Cowstr>;
using gnode = GraphNode;
using gnodesp = std::shared_ptr<gnode>;
using gedge = GraphEdge;
using gedgesp = std::shared_ptr<GraphEdge>;
using glabel = GraphLabel;
using glabelsp = std::shared_ptr<glabel>;
using gline = GraphLine;
using glinesp = std::shared_ptr<gline>;
using gcell = GraphCell;
using gcellsp = std::shared_ptr<gcell>;

const static attrmap node_attr = {
    {"shape", "record"},
    {"fontname", "Courier New, Courier"},
    {"fontsize", "12"},
};
const static attrmap edge_attr = {
    {"fontname", "Courier New, Courier"},
    {"fontsize", "12"},
};
const static attrmap graph_attr = {
    {"fontname", "Courier New, Courier"},
    {"fontsize", "12"},
};

#define glinevec std::vector<GraphLine>()

static GraphLine fromCell(const Cowstr &a) {
  GraphLine gl;
  gl.add(GraphCell(a));
  return gl;
}

static GraphLine fromCell(const Cowstr &a, const Cowstr &b) {
  GraphLine gl;
  gl.add(GraphCell(a));
  gl.add(GraphCell(b));
  return gl;
}

#define AG_NIL                                                                 \
  do {                                                                         \
    gnodesp u(new gnode({fromCell("nil")}));                                   \
    g.addNode(u);                                                              \
    return u;                                                                  \
  } while (0)

#define AG_LITERAL(ast, atype)                                                 \
  do {                                                                         \
    gnodesp u(                                                                 \
        new gnode({fromCell("literal", static_cast<atype *>(ast)->literal()),  \
                   fromCell("location", ast->location().str())}));             \
    g.addNode(u);                                                              \
    return u;                                                                  \
  } while (0)

#define AG_LOCATION(ast)                                                       \
  do {                                                                         \
    gnodesp u(new gnode({fromCell("location", ast->location().str())}));       \
    g.addNode(u);                                                              \
    return u;                                                                  \
  } while (0)

#define AGOP(ast, atype, op)                                                   \
  do {                                                                         \
    gnodesp u(new gnode({fromCell(BOOST_PP_STRINGIZE(op),                      \
                                  tokenName(static_cast<atype *>(ast)->op)),   \
                         fromCell("location", ast->location().str())}));       \
    g.addNode(u);                                                              \
    return u;                                                                  \
  } while (0)

#define AG1(ast, atype, a)                                                     \
  do {                                                                         \
    gnodesp u(new gnode({fromCell("location", ast->location().str())}));       \
    gnodesp v = astDrawImpl(static_cast<atype *>(ast)->a, g);                  \
    gedgesp e(new gedge(u->id(), v->id()));                                    \
    g.addNode(u);                                                              \
    g.addEdge(e);                                                              \
    return u;                                                                  \
  } while (0)

#define AGOP1(ast, atype, a, op)                                               \
  do {                                                                         \
    gnodesp u(new gnode({fromCell(BOOST_PP_STRINGIZE(op),                      \
                                  tokenName(static_cast<atype *>(ast)->op)),   \
                         fromCell("location", ast->location().str())}));       \
    gnodesp v = astDrawImpl(static_cast<atype *>(ast)->a, g);                  \
    gedgesp e(                                                                 \
        new gedge(u->id(), v->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(a))}))));   \
    g.addNode(u);                                                              \
    g.addEdge(e);                                                              \
    return u;                                                                  \
  } while (0)

#define AG2(ast, atype, a, b)                                                  \
  do {                                                                         \
    gnodesp u(new gnode({fromCell("location", ast->location().str())}));       \
    gnodesp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                  \
    gnodesp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                  \
    gedgesp ep(                                                                \
        new gedge(u->id(), p->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(a))}))));   \
    gedgesp eq(                                                                \
        new gedge(u->id(), q->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(b))}))));   \
    g.addNode(u);                                                              \
    g.addEdge(ep);                                                             \
    g.addEdge(eq);                                                             \
    return u;                                                                  \
  } while (0)

#define AGOP2(ast, atype, a, b, op)                                            \
  do {                                                                         \
    gnodesp u(new gnode({fromCell(BOOST_PP_STRINGIZE(op),                      \
                                  tokenName(static_cast<atype *>(ast)->op)),   \
                         fromCell("location", ast->location().str())}));       \
    gnodesp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                  \
    gnodesp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                  \
    gedgesp ep(                                                                \
        new gedge(u->id(), p->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(a))}))));   \
    gedgesp eq(                                                                \
        new gedge(u->id(), q->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(b))}))));   \
    g.addNode(u);                                                              \
    g.addEdge(ep);                                                             \
    g.addEdge(eq);                                                             \
    return u;                                                                  \
  } while (0)

#define AG3(ast, atype, a, b, c)                                               \
  do {                                                                         \
    gnodesp u(new gnode({fromCell("location", ast->location().str())}));       \
    gnodesp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                  \
    gnodesp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                  \
    gnodesp v = astDrawImpl(static_cast<atype *>(ast)->c, g);                  \
    gedgesp ep(                                                                \
        new gedge(u->id(), p->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(a))}))));   \
    gedgesp eq(                                                                \
        new gedge(u->id(), q->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(b))}))));   \
    gedgesp ev(                                                                \
        new gedge(u->id(), v->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(c))}))));   \
    g.addNode(u);                                                              \
    g.addEdge(ep);                                                             \
    g.addEdge(eq);                                                             \
    g.addEdge(ev);                                                             \
    return u;                                                                  \
  } while (0)

#define AGOP3(ast, atype, a, b, c, op)                                         \
  do {                                                                         \
    gnodesp u(new gnode({fromCell(BOOST_PP_STRINGIZE(op),                      \
                                  tokenName(static_cast<atype *>(ast)->op)),   \
                         fromCell("location", ast->location().str())}));       \
    gnodesp p = astDrawImpl(static_cast<atype *>(ast)->a, g);                  \
    gnodesp q = astDrawImpl(static_cast<atype *>(ast)->b, g);                  \
    gnodesp v = astDrawImpl(static_cast<atype *>(ast)->c, g);                  \
    gedgesp ep(                                                                \
        new gedge(u->id(), p->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(a))}))));   \
    gedgesp eq(                                                                \
        new gedge(u->id(), q->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(b))}))));   \
    gedgesp ev(                                                                \
        new gedge(u->id(), v->id(),                                            \
                  glabelsp(new glabel({fromCell(BOOST_PP_STRINGIZE(c))}))));   \
    g.addNode(u);                                                              \
    g.addEdge(ep);                                                             \
    g.addEdge(eq);                                                             \
    g.addEdge(ev);                                                             \
    return u;                                                                  \
  } while (0)

static gnodesp astDrawImpl(Ast *ast, Graph &g) {
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
      {"node", node_attr},
      {"edge", edge_attr},
      {"graph", graph_attr},
  };
  Graph g(output, gattr);
  astDrawImpl(ast_, g);
  return g.draw();
}

SymbolGraph::SymbolGraph(std::shared_ptr<Scope> scope) : scope_(scope) {}

int SymbolGraph::draw(const Cowstr &output) { return 0; }
