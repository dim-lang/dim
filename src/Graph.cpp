// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Counter.h"
#include "Files.h"
#include "LinkedHashMap.hpp"
#include "Log.h"
#include "Name.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"

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

// static Cowstr nameGenerate(const Cowstr &name, const Cowstr &delimiter = "")
// {
//   static Counter counter_;
//   std::stringstream ss;
//   ss << name << delimiter << counter_.next();
//   return ss.str();
// }

namespace detail {

static CounterNameGenerator namegen;

/**
 * <TD>xxx</TD>
 * <TD PORT="id">xxx</TD>
 */
class GCell {
public:
  GCell(const Cowstr &a_value, int a_width = 0)
      : id(namegen.generate("cell")), value(TRANSLATE(a_value)),
        width(a_width) {}
  virtual ~GCell() = default;

  Cowstr id; // PORT="id"
  Cowstr value;
  int width; // COLSPAN="width"

  virtual Cowstr str() {
    return width ? fmt::format("<TD PORT=\"{}\" COLSPAN=\"{}\">{}</TD>", id,
                               width, value)
                 : fmt::format("<TD PORT=\"{}\">{}</TD>", id, value);
  }
};

/**
 * <TR>xxx</TR>
 */
class GLine {
public:
  GLine(const std::vector<GCell> &a_cells = {}) : cells(a_cells) {}
  virtual ~GLine() = default;

  std::vector<GCell> cells;

  virtual void add(const Cowstr &cell) {
    GCell c(cell);
    cells.push_back(c);
  }

  virtual Cowstr str() {
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
 * <TABLE attributes> xxx </TABLE>
 */
class GLabel {
public:
  GLabel(const std::vector<GLine> &a_lines,
         const std::unordered_map<Cowstr, Cowstr> &a_attributes)
      : lines(a_lines), attributes(a_attributes) {}
  virtual ~GLabel() = default;

  std::vector<GLine> lines;
  std::unordered_map<Cowstr, Cowstr> attributes;

  virtual const Cowstr &id() const { return lines[0].cells[0].id; }
  virtual Cowstr str() {
    int maxWidth = 0;
    for (int i = 0; i < (int)lines.size(); i++) {
      maxWidth = std::max<int>(lines[i].cells.size(), maxWidth);
    }
    for (int i = 0; i < (int)lines.size(); i++) {
      if (lines[i].cells.size() < maxWidth) {
        lines[i].cells[0].width = maxWidth - lines[i].cells.size() + 1;
      }
    }

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
};

/**
 * node
 * node [label=< xxx >]
 */
class GNode {
public:
  GNode(std::shared_ptr<GLabel> a_label)
      : id(namegen.generate("node")), label(a_label) {}
  virtual ~GNode() = default;

  Cowstr id;
  std::shared_ptr<GLabel> label;

  virtual Cowstr str() {
    return fmt::format("{} [label=<{}>]", id, label->str());
  }
};

/**
 * from -> to
 * from -> to [label=< xxx >]
 */
class GEdge {
public:
  GEdge(std::shared_ptr<GNode> a_from, std::shared_ptr<GNode> a_to,
        std::shared_ptr<GLabel> a_label)
      : fromNode(a_from), fromCell(nullptr), toNode(a_to), toCell(nullptr),
        label(a_label) {}
  GEdge(std::shared_ptr<GNode> a_from, std::shared_ptr<GCell> a_fromCell,
        std::shared_ptr<GNode> a_to, std::shared_ptr<GCell> a_toCell,
        std::shared_ptr<GLabel> a_label)
      : fromNode(a_from), fromCell(a_fromCell), toNode(a_to), toCell(a_toCell),
        label(a_label) {}
  virtual ~GEdge() = default;

  std::shared_ptr<GNode> fromNode;
  std::shared_ptr<GCell> fromCell;
  std::shared_ptr<GNode> toNode;
  std::shared_ptr<GCell> toCell;
  std::shared_ptr<GLabel> label;

  virtual void setFromCell(const GCell &c) {
    fromCell = std::shared_ptr<GCell>(new GCell(c));
  }
  virtual void setToCell(const GCell &c) {
    toCell = std::shared_ptr<GCell>(new GCell(c));
  }
  virtual Cowstr str() {
    std::stringstream ss;
    ss << fromNode->id;
    if (fromCell) {
      ss << ":" << fromCell->id;
    }
    ss << " -> " << toNode->id;
    if (toCell) {
      ss << ":" << toCell->id << ":n";
    }
    if (label) {
      ss << " [label=<" << label->str() << ">]";
    }
    return ss.str();
  }
};

/**
 * digraph nerd {
 *   node [ attributes ]
 *   edge [ attributes ]
 *   graph [ attributes ]
 *
 *   node1 [label=< xxx >]
 *   node2 [label=< xxx >]
 *   ...
 *
 *   node1 -> node2 [label=< xxx >]
 *   node3 -> node3 [label=< xxx >]
 *   ...
 * }
 */
class Graph {
public:
  Graph(const Cowstr &a_fileName,
        const std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>>
            &a_attributes)
      : fileName(a_fileName), attributes(a_attributes) {}
  virtual ~Graph() = default;

  Cowstr fileName;
  std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>> attributes;
  std::vector<std::shared_ptr<GNode>> nodes;
  std::vector<std::shared_ptr<GEdge>> edges;

  virtual int draw() {
    FileWriter fwriter(fileName);
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
};

} // namespace detail

using attrmap = std::unordered_map<Cowstr, Cowstr>;
using gattrmap = std::unordered_map<Cowstr, attrmap>;
using gc = detail::GCell;
using gcp = std::shared_ptr<gc>;
using gli = detail::GLine;
using glip = std::shared_ptr<gli>;
using gla = detail::GLabel;
using glap = std::shared_ptr<gla>;
using gn = detail::GNode;
using gnp = std::shared_ptr<gn>;
using ge = detail::GEdge;
using gep = std::shared_ptr<ge>;

const static attrmap edge_label_attr = {
    {"BORDER", "\"0\""},
    {"CELLBORDER", "\"0\""},
    {"CELLSPACING", "\"0\""},
};

const static attrmap node_label_attr = {
    {"BORDER", "\"0\""},
    {"CELLBORDER", "\"1\""},
    {"CELLSPACING", "\"0\""},
};

const static gattrmap graph_attr = {
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

static gli LINE(const Cowstr &a) {
  gli line;
  line.add(a);
  return line;
}

static gli LINE(const Cowstr &a, const Cowstr &b) {
  gli line;
  line.add(a);
  line.add(b);
  return line;
}

static gli LINE(const Cowstr &a, const Cowstr &b, const Cowstr &c,
                const Cowstr &d) {
  gli line;
  line.add(a);
  line.add(b);
  line.add(c);
  line.add(d);
  return line;
}

static gnp NODE(const gli &a) {
  std::vector<gli> lines;
  lines.push_back(a);
  glap label(new gla(lines, node_label_attr));
  gnp node(new gn(label));
  return node;
}

static gnp NODE(const gli &a, const gli &b) {
  std::vector<gli> lines;
  lines.push_back(a);
  lines.push_back(b);
  glap label(new gla(lines, node_label_attr));
  gnp node(new gn(label));
  return node;
}

static gnp NODE(const gli &a, const gli &b, const gli &c) {
  std::vector<gli> lines;
  lines.push_back(a);
  lines.push_back(b);
  lines.push_back(c);
  glap label(new gla(lines, node_label_attr));
  gnp node(new gn(label));
  return node;
}

static glap LABEL(const gli &a) {
  std::vector<gli> lines;
  lines.push_back(a);
  glap label(new gla(lines, edge_label_attr));
  return label;
}

static gep EDGE(gnp a, gnp b, glap c = nullptr) { return gep(new ge(a, b, c)); }

static gep EDGE(gnp a, const gc &ac, gnp b, const gc &bc, glap c = nullptr) {
  return gep(new ge(a, gcp(new gc(ac)), b, gcp(new gc(bc)), c));
}

#define AG_NIL                                                                 \
  do {                                                                         \
    gnp u = NODE(LINE("nil"));                                                 \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AG_LITERAL(ast, atype)                                                 \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()), LINE("literal", ast->name()), \
                 LINE("location", ast->location().str()));                     \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AG_LOCATION(ast)                                                       \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE("location", ast->location().str()));                     \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AGOP(ast, atype, op)                                                   \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE(BOOST_PP_STRINGIZE(op),                                  \
                      tokenName(static_cast<atype *>(ast)->op)),               \
                 LINE("location", ast->location().str()));                     \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define AG1(ast, atype, a)                                                     \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE("location", ast->location().str()));                     \
    gnp v = drawAst(static_cast<atype *>(ast)->a, g);                          \
    gep e = EDGE(u, v);                                                        \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define AGOP1(ast, atype, a, op)                                               \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE(BOOST_PP_STRINGIZE(op),                                  \
                      tokenName(static_cast<atype *>(ast)->op)),               \
                 LINE("location", ast->location().str()));                     \
    gnp v = drawAst(static_cast<atype *>(ast)->a, g);                          \
    gep e = EDGE(u, v, LABEL(LINE(BOOST_PP_STRINGIZE(a))));                    \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define AG2(ast, atype, a, b)                                                  \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE("location", ast->location().str()));                     \
    gnp p = drawAst(static_cast<atype *>(ast)->a, g);                          \
    gnp q = drawAst(static_cast<atype *>(ast)->b, g);                          \
    gep ep = EDGE(u, p, LABEL(LINE(BOOST_PP_STRINGIZE(a))));                   \
    gep eq = EDGE(u, q, LABEL(LINE(BOOST_PP_STRINGIZE(b))));                   \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define AGOP2(ast, atype, a, b, op)                                            \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE(BOOST_PP_STRINGIZE(op),                                  \
                      tokenName(static_cast<atype *>(ast)->op)),               \
                 LINE("location", ast->location().str()));                     \
    gnp p = drawAst(static_cast<atype *>(ast)->a, g);                          \
    gnp q = drawAst(static_cast<atype *>(ast)->b, g);                          \
    gep ep = EDGE(u, p, LABEL(LINE(BOOST_PP_STRINGIZE(a))));                   \
    gep eq = EDGE(u, q, LABEL(LINE(BOOST_PP_STRINGIZE(b))));                   \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define AG3(ast, atype, a, b, c)                                               \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE("location", ast->location().str()));                     \
    gnp p = drawAst(static_cast<atype *>(ast)->a, g);                          \
    gnp q = drawAst(static_cast<atype *>(ast)->b, g);                          \
    gnp v = drawAst(static_cast<atype *>(ast)->c, g);                          \
    gep ep = EDGE(u, p, LABEL(LINE(BOOST_PP_STRINGIZE(a))));                   \
    gep eq = EDGE(u, q, LABEL(LINE(BOOST_PP_STRINGIZE(b))));                   \
    gep ev = EDGE(u, v, LABEL(LINE(BOOST_PP_STRINGIZE(c))));                   \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

#define AGOP3(ast, atype, a, b, c, op)                                         \
  do {                                                                         \
    gnp u = NODE(LINE(ast->kind()._to_string()),                               \
                 LINE(BOOST_PP_STRINGIZE(op),                                  \
                      tokenName(static_cast<atype *>(ast)->op)),               \
                 LINE("location", ast->location().str()));                     \
    gnp p = drawAst(static_cast<atype *>(ast)->a, g);                          \
    gnp q = drawAst(static_cast<atype *>(ast)->b, g);                          \
    gnp v = drawAst(static_cast<atype *>(ast)->c, g);                          \
    gep ep = EDGE(u, p, LABEL(LINE(BOOST_PP_STRINGIZE(a))));                   \
    gep eq = EDGE(u, q, LABEL(LINE(BOOST_PP_STRINGIZE(b))));                   \
    gep ev = EDGE(u, v, LABEL(LINE(BOOST_PP_STRINGIZE(c))));                   \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

static gnp drawAst(Ast *ast, detail::Graph &g) {
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

int AstGraph::draw(const Cowstr &output) const {
  detail::Graph g(output, graph_attr);
  drawAst(ast_, g);
  return g.draw();
}

using scsp = std::shared_ptr<Scope>;
using symsp = std::shared_ptr<Symbol>;
using tsymsp = std::shared_ptr<TypeSymbol>;

static gli drawSymbol(symsp sym) {
  if (!sym) {
    return LINE("nil");
  }
  gli l;
  l.add("symbol");
  if (sym->owner()) {
    SymbolData sdata = sym->owner()->s_resolve(sym->name());
    l.add(fmt::format("{}: {}", sym->name(), sdata.typeSymbol->name()));
  } else {
    l.add(fmt::format("{}: void", sym->name()));
  }
  l.add(sym->kind()._to_string());
  l.add(sym->location().str());
  return l;
}

static gli drawTypeSymbol(std::shared_ptr<TypeSymbol> sym) {
  if (!sym) {
    return LINE("nil");
  }
  gli l;
  l.add("type symbol");
  l.add(sym->name());
  l.add(sym->kind()._to_string());
  l.add(sym->location().str());
  return l;
}

static gnp drawScope(scsp scope, detail::Graph &g) {
  if (!scope) {
    AG_NIL;
  }
  gnp u = NODE(LINE(scope->name()));
  if (scope->s_empty() && scope->ts_empty()) {
    u->label->lines.push_back(LINE("empty"));
  } else {
    u->label->lines.push_back(LINE(" ", "name (: type)", "kind", "location"));
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
        gli l = drawSymbol(sdata.symbol);
        u->label->lines.push_back(l);
        gnp v = drawScope(std::dynamic_pointer_cast<Scope>(sdata.symbol), g);
        gep e = EDGE(u, l.cells[0], v, v->label->lines[0].cells[0]);
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
  }
  g.nodes.push_back(u);
  return u;
}

SymbolGraph::SymbolGraph(std::shared_ptr<Scope> scope) : scope_(scope) {}

int SymbolGraph::draw(const Cowstr &output) const {
  detail::Graph g(output, graph_attr);
  drawScope(scope_, g);
  return g.draw();
}
