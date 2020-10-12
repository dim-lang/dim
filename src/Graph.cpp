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
#include <unordered_map>

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

namespace detail {

static CounterNameGenerator namegen;

/**
 * <TD>xxx</TD>
 * <TD PORT="id">xxx</TD>
 */
class GCell {
public:
  GCell(const Cowstr &a_id, const Cowstr &a_value, int a_width)
      : id(a_id), value(TRANSLATE(a_value)), width(a_width) {}
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
  GLine(const std::vector<GCell> &a_cells) : cells(a_cells) {}
  virtual ~GLine() = default;

  std::vector<GCell> cells;

  virtual void add(const Cowstr &id, const Cowstr &cell, int width) {
    GCell c(id, cell, width);
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
 * <TR> xxx </TR>
 * <TR> xxx </TR>
 * <TR> xxx </TR>
 */
class GLabel {
public:
  GLabel(const std::vector<GLine> &a_lines) : lines(a_lines) {}
  virtual ~GLabel() = default;

  std::vector<GLine> lines;

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
    for (int i = 0; i < (int)lines.size(); i++) {
      ss << lines[i].str();
      if (i < (int)lines.size() - 1) {
        ss << "\n";
      }
    }
    return ss.str();
  }
};

/**
 * node [label=<<TABLE attributes> xxx </TABLE>>]
 */
class GNode {
public:
  GNode(const Cowstr &a_id, std::shared_ptr<GLabel> a_label)
      : id(a_id), label(a_label) {}
  virtual ~GNode() = default;

  Cowstr id;
  std::shared_ptr<GLabel> label;

  virtual Cowstr str() {
    std::unordered_map<Cowstr, Cowstr> &attributes = {
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
    ss << label->str();
    ss << "</TABLE>>]";
    return ss.str();
  }
};

/**
 * from -> to [label=<<TABLE attributes> xxx </TABLE>>]
 * from -> to [style="dotted" label=<<TABLE attributes> xxx </TABLE>>]
 * from:fromcell -> to:tocell [label=<<TABLE attributes> xxx </TABLE>>]
 * from:fromcell -> to:tocell [style="dotted" label=<<TABLE attributes> xxx
 * </TABLE>>]
 */
class GEdge {
public:
  GEdge(std::shared_ptr<GNode> a_from, std::shared_ptr<GNode> a_to,
        std::shared_ptr<GLabel> a_label, const Cowstr &a_style = "")
      : fromNode(a_from), fromCell(nullptr), toNode(a_to), toCell(nullptr),
        label(a_label), style(a_style) {}
  GEdge(std::shared_ptr<GNode> a_from, std::shared_ptr<GCell> a_fromCell,
        std::shared_ptr<GNode> a_to, std::shared_ptr<GCell> a_toCell,
        std::shared_ptr<GLabel> a_label, const Cowstr &a_style = "")
      : fromNode(a_from), fromCell(a_fromCell), toNode(a_to), toCell(a_toCell),
        label(a_label), style(a_style) {}
  virtual ~GEdge() = default;

  std::shared_ptr<GNode> fromNode;
  std::shared_ptr<GCell> fromCell;
  std::shared_ptr<GNode> toNode;
  std::shared_ptr<GCell> toCell;
  std::shared_ptr<GLabel> label;
  Cowstr style;

  virtual Cowstr str() {
    const std::unordered_map<Cowstr, Cowstr> &attributes = {
        {"BORDER", "\"0\""},
        {"CELLBORDER", "\"0\""},
        {"CELLSPACING", "\"0\""},
    };

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
      ss << " [";
      if (!style.empty()) {
        ss << "style=\"" << style << "\" ";
      }
      ss << "label=<<TABLE";
      for (auto i = attributes.begin(); i != attributes.end(); i++) {
        ss << " " << i->first << "=" << i->second;
      }
      ss << ">" << label->str() << "</TABLE>>]";
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
 *   node2:cell4 -> node3:cell5 [label=< xxx >]
 *   ...
 * }
 */
class Graph {
public:
  Graph(const Cowstr &a_fileName) : fileName(a_fileName) {}
  virtual ~Graph() = default;

  Cowstr fileName;
  std::vector<std::shared_ptr<GNode>> nodes;
  std::vector<std::shared_ptr<GEdge>> edges;

  virtual int draw() {
    std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>> &attributes =
        {
            {
                "node",
                {
                    {"shape", "none"},
                    {"fontname", "\"Courier New, Courier\""},
                },
            },
            {
                "edge",
                {
                    {"fontname", "\"Courier New, Courier\""},
                },
            },
            {
                "graph",
                {
                    {"fontname", "\"Courier New, Courier\""},
                },
            },
        };

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

GLine line(const Cowstr &id1, const Cowstr &value1, int width1) {
  GCell cell1(id1, value1, width1);

  std::vector<GCell> cells;
  cells.push_back(cell1);

  return GLine(cells);
}

GLine line(const Cowstr &id1, const Cowstr &value1, int width1,
           const Cowstr &id2, const Cowstr &value2, int width2) {
  GCell cell1(id1, value1, width1);
  GCell cell2(id2, value2, width2);

  std::vector<GCell> cells;
  cells.push_back(cell1);
  cells.push_back(cell2);

  return GLine(cells);
}

GLine line(const Cowstr &id1, const Cowstr &value1, int width1,
           const Cowstr &id2, const Cowstr &value2, int width2,
           const Cowstr &id3, const Cowstr &value3, int width3) {
  GCell cell1(id1, value1, width1);
  GCell cell2(id2, value2, width2);
  GCell cell3(id3, value3, width3);

  std::vector<GCell> cells;
  cells.push_back(cell1);
  cells.push_back(cell2);
  cells.push_back(cell3);

  return GLine(cells);
}

GLine line(const Cowstr &id1, const Cowstr &value1, int width1,
           const Cowstr &id2, const Cowstr &value2, int width2,
           const Cowstr &id3, const Cowstr &value3, int width3,
           const Cowstr &id4, const Cowstr &value4, int width4) {
  GCell cell1(id1, value1, width1);
  GCell cell2(id2, value2, width2);
  GCell cell3(id3, value3, width3);
  GCell cell4(id4, value4, width4);

  std::vector<GCell> cells;
  cells.push_back(cell1);
  cells.push_back(cell2);
  cells.push_back(cell3);
  cells.push_back(cell4);

  return GLine(cells);
}

GLine line(const Cowstr &a) { return line(namegen.generate("cell"), a, 0); }

GLine line(const Cowstr &a, const Cowstr &b) {
  return line(namegen.generate("cell"), a, 0, namegen.generate("cell"), b, 0);
}

GLine line(const Cowstr &a, const Cowstr &b, const Cowstr &c) {
  return line(namegen.generate("cell"), a, 0, namegen.generate("cell"), b, 0,
              namegen.generate("cell"), c, 0);
}

GLine line(const Cowstr &a, const Cowstr &b, const Cowstr &c, const Cowstr &d) {
  return line(namegen.generate("cell"), a, 0, namegen.generate("cell"), b, 0,
              namegen.generate("cell"), c, 0, namegen.generate("cell"), d, 0);
}

std::shared_ptr<GNode> node(const Cowstr &id, const GLine &a) {
  std::vector<GLine> lines;
  lines.push_back(a);
  std::shared_ptr<GLabel> label(new GLabel(lines));
  return std::shared_ptr(new GNode(id, label));
}

std::shared_ptr<GNode> node(const Cowstr &id, const GLine &a, const GLine &b) {
  std::vector<GLine> lines;
  lines.push_back(a);
  lines.push_back(b);
  std::shared_ptr<GLabel> label(new GLabel(lines));
  return std::shared_ptr(new GNode(id, label));
}

std::shared_ptr<GNode> node(const Cowstr &id, const GLine &a, const GLine &b,
                            const GLine &c) {
  std::vector<GLine> lines;
  lines.push_back(a);
  lines.push_back(b);
  lines.push_back(c);
  std::shared_ptr<GLabel> label(new GLabel(lines));
  return std::shared_ptr(new GNode(id, label));
}

std::shared_ptr<GNode> node(const GLine &a) {
  return node(namegen.generate("node"), a);
}

std::shared_ptr<GNode> node(const GLine &a, const GLine &b) {
  return node(namegen.generate("node"), a, b);
}

std::shared_ptr<GNode> node(const GLine &a, const GLine &b, const GLine &c) {
  return node(namegen.generate("node"), a, b, c);
}

std::shared_ptr<GLabel> label(const GLine &a) {
  std::vector<GLine> lines;
  lines.push_back(a);
  return std::shared_ptr(new GLabel(lines));
}

std::shared_ptr<GEdge> edge(std::shared_ptr<GNode> a, std::shared_ptr<GNode> b,
                            std::shared_ptr<GLabel> label,
                            bool dotted = false) {
  return std::shared_ptr<GEdge>(new GEdge(a, b, label, dotted ? "dotted" : ""));
}

std::shared_ptr<GEdge> edge(std::shared_ptr<GNode> a, const GCell &acell,
                            std::shared_ptr<GNode> b, const GCell &bcell,
                            std::shared_ptr<GLabel> label,
                            bool dotted = false) {
  return std::shared_ptr<GEdge>(new GEdge(
      a, std::shared_ptr<GCell>(new GCell(acell)) b,
      std::shared_ptr<GCell>(new GCell(bcell)), label, dotted ? "dotted" : ""));
}

std::shared_ptr<GEdge> edge(std::shared_ptr<GNode> a, std::shared_ptr<GNode> b,
                            bool dotted = false) {
  return edge(a, b, nullptr, dotted ? "dotted" : "");
}

std::shared_ptr<GEdge> edge(std::shared_ptr<GNode> a, const GCell &acell,
                            std::shared_ptr<GNode> b, const GCell &bcell,
                            bool dotted = false) {
  return edge(a, acell, b, bcell, nullptr, dotted ? "dotted" : "");
}

using GNodePtr = std::shared_ptr<GNode>;
using GEdgePtr = std::shared_ptr<GEdge>;

#define DA_NIL                                                                 \
  do {                                                                         \
    GNodePtr u = node(line("nil"));                                            \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define DA_LITERAL                                                             \
  do {                                                                         \
    GNodePtr u =                                                               \
        node(line(ast->kind()._to_string()), line("literal", ast->name()),     \
             line("location", ast->location().str()));                         \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define DA_LOCATION                                                            \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line("location", ast->location().str()));                \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define DA_OP(ast, asttype, op)                                                \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line(BOOST_PP_STRINGIZE(op),                             \
                           tokenName(static_cast<asttype *>(ast)->op)),        \
                      line("location", ast->location().str()));                \
    g.nodes.push_back(u);                                                      \
    return u;                                                                  \
  } while (0)

#define DA1(ast, asttype, a)                                                   \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line("location", ast->location().str()));                \
    GNodePtr v = drawAst(static_cast<asttype *>(ast)->a, g);                   \
    GEdgePtr e = edge(u, v);                                                   \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define DA_OP1(ast, asttype, a, op)                                            \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line(BOOST_PP_STRINGIZE(op),                             \
                           tokenName(static_cast<asttype *>(ast)->op)),        \
                      line("location", ast->location().str()));                \
    GNodePtr v = drawAst(static_cast<asttype *>(ast)->a, g);                   \
    GEdgePtr e = edge(u, v, label(line(BOOST_PP_STRINGIZE(a))));               \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(e);                                                      \
    return u;                                                                  \
  } while (0)

#define DA2(ast, asttype, a, b)                                                \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line("location", ast->location().str()));                \
    GNodePtr p = drawAst(static_cast<asttype *>(ast)->a, g);                   \
    GNodePtr q = drawAst(static_cast<asttype *>(ast)->b, g);                   \
    GEdgePtr ep = edge(u, p, label(line(BOOST_PP_STRINGIZE(a))));              \
    GEdgePtr eq = edge(u, q, label(line(BOOST_PP_STRINGIZE(b))));              \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define DA_OP2(ast, asttype, a, b, op)                                         \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line(BOOST_PP_STRINGIZE(op),                             \
                           tokenName(static_cast<asttype *>(ast)->op)),        \
                      line("location", ast->location().str()));                \
    GNodePtr p = drawAst(static_cast<asttype *>(ast)->a, g);                   \
    GNodePtr q = drawAst(static_cast<asttype *>(ast)->b, g);                   \
    GEdgePtr ep = edge(u, p, label(line(BOOST_PP_STRINGIZE(a))));              \
    GEdgePtr eq = edge(u, q, label(line(BOOST_PP_STRINGIZE(b))));              \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    return u;                                                                  \
  } while (0)

#define DA3(ast, asttype, a, b, c)                                             \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line("location", ast->location().str()));                \
    GNodePtr p = drawAst(static_cast<asttype *>(ast)->a, g);                   \
    GNodePtr q = drawAst(static_cast<asttype *>(ast)->b, g);                   \
    GNodePtr v = drawAst(static_cast<asttype *>(ast)->c, g);                   \
    GEdgePtr ep = edge(u, p, label(line(BOOST_PP_STRINGIZE(a))));              \
    GEdgePtr eq = edge(u, q, label(line(BOOST_PP_STRINGIZE(b))));              \
    GEdgePtr ev = edge(u, v, label(line(BOOST_PP_STRINGIZE(c))));              \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

#define DA_OP3(ast, asttype, a, b, c, op)                                      \
  do {                                                                         \
    GNodePtr u = node(line(ast->kind()._to_string()),                          \
                      line(BOOST_PP_STRINGIZE(op),                             \
                           tokenName(static_cast<asttype *>(ast)->op)),        \
                      line("location", ast->location().str()));                \
    GNodePtr p = drawAst(static_cast<asttype *>(ast)->a, g);                   \
    GNodePtr q = drawAst(static_cast<asttype *>(ast)->b, g);                   \
    GNodePtr v = drawAst(static_cast<asttype *>(ast)->c, g);                   \
    GEdgePtr ep = edge(u, p, label(line(BOOST_PP_STRINGIZE(a))));              \
    GEdgePtr eq = edge(u, q, label(line(BOOST_PP_STRINGIZE(b))));              \
    GEdgePtr ev = edge(u, v, label(line(BOOST_PP_STRINGIZE(c))));              \
    g.nodes.push_back(u);                                                      \
    g.edges.push_back(ep);                                                     \
    g.edges.push_back(eq);                                                     \
    g.edges.push_back(ev);                                                     \
    return u;                                                                  \
  } while (0)

static GNodePtr drawSymbol(std::shared_ptr<Symbol> symbol, Graph &g) {}

static GNodePtr drawTypeSymbol(std::shared_ptr<TypeSymbol> typeSymbol,
                               Graph &g) {}

static GNodePtr drawScope(std::shared_ptr<Scope> scope, Graph &g) {}

static GNodePtr drawAst(Ast *ast, Graph &g) {
  if (!ast) {
    DA_NIL;
  }
  switch (ast->kind()) {
  case AstKind::Integer: {
    DA_LITERAL;
  }
  case AstKind::Float: {
    DA_LITERAL;
  }
  case AstKind::Boolean: {
    DA_LITERAL;
  }
  case AstKind::Character: {
    DA_LITERAL;
  }
  case AstKind::String: {
    DA_LITERAL;
  }
  case AstKind::Nil: {
    DA_LOCATION;
  }
  case AstKind::Void: {
    DA_LOCATION;
  }
  case AstKind::VarId: {
    GNodePtr u =
        node(line(ast->kind()._to_string()), line("literal", ast->name()),
             line("location", ast->location().str()));
    g.nodes.push_back(u);
    A_VarId *tmp = static_cast<A_VarId *>(ast);
    if (tmp->symbol) {
      GNodePtr v = drawSymbol(tmp->symbol, g);
      switch (tmp->symbol->kind()) {
      case SymbolKind::Var:
      case SymbolKind::Param:
      case SymbolKind::Field: {
        GEdgePtr ev = edge(u, v, true);
        g.edges.push_back(ev);
        break;
      }
      case SymbolKind::Func:
      case SymbolKind::Method: {
        GEdgePtr ev = edge(u, v);
        g.edges.push_back(ev);
        break;
      }
      }
    } else {
      LOG_ASSERT(tmp->typeSymbol, "tmp {} type symbol must not null",
                 tmp->name());
      // class definition
      GNodePtr v = drawTypeSymbol(tmp->typeSymbol, g);
      switch (tmp->typeSymbol->kind()) {
      case SymbolKind::Plain:
      case SymbolKind::Func: {
        GEdgePtr ev = edge(u, v, true);
        g.edges.push_back(ev);
        break;
      }
      case SymbolKind::Class: {
        GEdgePtr ev = edge(u, v);
        g.edges.push_back(ev);
        break;
      }
      }
    }
    return u;
  }
  case AstKind::Break: {
    DA_LOCATION;
  }
  case AstKind::Continue: {
    DA_LOCATION;
  }
  case AstKind::Throw: {
    DA1(ast, A_Throw, expr);
  }
  case AstKind::Return: {
    DA1(ast, A_Return, expr);
  }
  case AstKind::Assign: {
    DA_OP2(ast, A_Assign, assignee, assignor, assignOp);
  }
  case AstKind::PostfixExpr: {
    DA_OP1(ast, A_PostfixExpr, expr, postfixOp);
  }
  case AstKind::InfixExpr: {
    DA_OP2(ast, A_InfixExpr, left, right, infixOp);
  }
  case AstKind::PrefixExpr: {
    DA_OP1(ast, A_PrefixExpr, expr, prefixOp);
  }
  case AstKind::Call: {
    DA2(ast, A_Call, id, args);
  }
  case AstKind::Exprs: {
    DA2(ast, A_Exprs, expr, next);
  }
  case AstKind::If: {
    DA3(ast, A_If, condition, thenp, elsep);
  }
  case AstKind::Loop: {
    GNodePtr u = node(line(ast->kind()._to_string()),
                      line("location", ast->location().str()));
    GNodePtr p = drawAst(static_cast<A_Loop *>(ast)->condition, g);
    GNodePtr q = drawAst(static_cast<A_Loop *>(ast)->body, g);
    GEdgePtr ep = edge(u, p, label(line("condition")));
    GEdgePtr eq = edge(u, q, label(line("body")));
    g.nodes.push_back(u);
    g.edges.push_back(ep);
    g.edges.push_back(eq);
    A_Loop *tmp = static_cast<A_Loop *>(ast);
    LOG_ASSERT(tmp->localScope, "tmp {} localScope must not null", tmp->name());
    GNodePtr s = drawScope(tmp->localScope, g);
    return u;
  }
  case AstKind::Yield: {
    DA1(ast, A_Yield, expr);
  }
  case AstKind::LoopCondition: {
    DA3(ast, A_LoopCondition, init, condition, update);
  }
  case AstKind::LoopEnumerator: {
    DA2(ast, A_LoopEnumerator, id, expr);
  }
  case AstKind::DoWhile: {
    DA2(ast, A_DoWhile, body, condition);
  }
  case AstKind::Try: {
    DA3(ast, A_Try, tryp, catchp, finallyp);
  }
  case AstKind::Block: {
    DA1(ast, A_Block, blockStats);
  }
  case AstKind::BlockStats: {
    DA2(ast, A_BlockStats, blockStat, next);
  }
  case AstKind::PlainType: {
    DA_OP(ast, A_PlainType, token);
  }
  case AstKind::FuncDef: {
    DA3(ast, A_FuncDef, funcSign, resultType, body);
  }
  case AstKind::FuncSign: {
    DA2(ast, A_FuncSign, id, params);
  }
  case AstKind::Params: {
    DA2(ast, A_Params, param, next);
  }
  case AstKind::Param: {
    DA2(ast, A_Param, id, type);
  }
  case AstKind::VarDef: {
    DA3(ast, A_VarDef, id, type, expr);
  }
  case AstKind::TopStats: {
    DA2(ast, A_TopStats, topStat, next);
  }
  case AstKind::CompileUnit: {
    DA1(ast, A_CompileUnit, topStats);
  }
  default:
    LOG_ASSERT(false, "invalid ast kind: {}", ast->kind()._to_string());
  }
}

} // namespace detail

int Graph::drawAst(Ast *ast, const Cowstr &output) {
  detail::Graph g(output);
  detail::drawAst(ast, g);
  return g.draw();
}
