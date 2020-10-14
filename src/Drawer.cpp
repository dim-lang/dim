// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Counter.h"
#include "Symbol.h"
#include <algorithm>

namespace detail {

static CounterNameGenerator CNG;

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

/**
 * <TD PORT="id"> value </TD>
 * <TD PORT="id" COLSPAN="width"> value </TD>
 */
struct GCell {
  GCell(const Cowstr &a_id, const Cowstr &a_value, int a_width)
      : id(a_id), value(TRANSLATE(a_value)), width(a_width) {}
  ~GCell() = default;

  Cowstr id; // PORT="id"
  Cowstr value;
  int width; // COLSPAN="width"

  Cowstr str() {
    return width ? fmt::format("<TD PORT=\"{}\" COLSPAN=\"{}\">{}</TD>", id,
                               width, value)
                 : fmt::format("<TD PORT=\"{}\">{}</TD>", id, value);
  }
};

/**
 * <TR> GCell+ </TR>
 */
struct GLine {
  GLine(const std::vector<GCell> &a_cells) : cells(a_cells) {}
  ~GLine() = default;

  std::vector<GCell> cells;

  Cowstr str() {
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
 * (node)id [label=<<TABLE attributes> GLine+ </TABLE>>]
 */
struct GNode {
  GNode(const Cowstr &a_id) : id(a_id) {}
  ~GNode() = default;

  Cowstr id;
  std::vector<GLine> lines;

  void adjust() {
    int maxWidth = 0;
    for (int i = 0; i < (int)lines.size(); i++) {
      maxWidth = std::max<int>(lines[i].cells.size(), maxWidth);
    }
    for (int i = 0; i < (int)lines.size(); i++) {
      if (lines[i].cells.size() < maxWidth) {
        lines[i].cells[0].width = maxWidth - lines[i].cells.size() + 1;
      }
    }
  }

  Cowstr str() {
    adjust();
    const std::unordered_map<Cowstr, Cowstr> &attributes = {
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
    for (int i = 0; i < (int)lines.size(); i++) {
      ss << lines[i].str();
      if (i < (int)lines.size() - 1) {
        ss << "\n";
      }
    }
    ss << "</TABLE>>]";
    return ss.str();
  }
};

struct NilNode {
  NilNode() : node(CNG.generate("nilnode")) {
    GCell cell(CNG.generate("cell"), "nil", 0);
    GLine line({cell});
    node.lines.push_back(line);
  }

  GNode node;

  Cowstr id() { return node.id; }
  Cowstr str() { return node.str(); }
};

struct AstNode {
  AstGNode(Ast *ast) : node(generateName(ast)) {
    add(ast->kind()._to_string());
    add("id", Cowstr::from(ast->identifier()));
    add("location", ast->location().str());
  }

  GNode node;

  static Cowstr generateName(Ast *ast) {
    return Cowstr("astnode") + Cowstr::from(ast->identifier());
  }
  void add(const Cowstr &a) {
    GCell c(CNG.generate("cell"), a, 0);
    GLine line({c});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GLine line({c1, c2});
    node.lines.push_back(line);
  }
  Cowstr id() { return node.id; }
  Cowstr str() { return node.str(); }
};

struct ScopeNode {
  ScopeNode(Scope *scope) : node(generateName(scope)) {
    add(scope->name());
    add("id", Cowstr::from(ast->identifier()));
    add("location", ast->location().str());
  }

  GNode node;

  static Cowstr generateName(Scope *scope) {
    return Cowstr("scopenode") + Cowstr::from(ast->identifier());
  }
  void emptify(const Cowstr &area) {
    add(area);
    add("empty");
  }
  void add(const Cowstr &a) {
    GCell c(CNG.generate("cell"), a, 0);
    GLine line({c});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GLine line({c1, c2});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b, const Cowstr &c) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GCell c3(CNG.generate("cell"), c, 0);
    GLine line({c1, c2, c3});
    node.lines.push_back(line);
  }
  void add(const Cowstr &a, const Cowstr &b, const Cowstr &c, const Cowstr &d) {
    GCell c1(CNG.generate("cell"), a, 0);
    GCell c2(CNG.generate("cell"), b, 0);
    GCell c3(CNG.generate("cell"), c, 0);
    GCell c4(CNG.generate("cell"), d, 0);
    GLine line({c1, c2, c3, c4});
    node.lines.push_back(line);
  }
  void add(Symbol *sym) {
    GCell c1(CNG.generate("cell"), Cowstr::from(sym->identifier()), 0);
    GCell c2(CNG.generate("cell"), sym->name(), 0);
    GCell c3(CNG.generate("cell"), sym->type()->name(), 0);
    GCell c4(CNG.generate("cell"), sym->kind()._to_string(), 0);
    GCell c5(CNG.generate("cell"), sym->location(), 0);
    GLine line({c1, c2, c3, c4, c5});
    node.lines.push_back(line);
  }
  void add(TypeSymbol *tsym) {
    GCell c1(CNG.generate("cell"), Cowstr::from(tsym->identifier()), 0);
    GCell c2(CNG.generate("cell"), tsym->name(), 0);
    GCell c3(CNG.generate("cell"), tsym->kind()._to_string(), 0);
    GCell c4(CNG.generate("cell"), tsym->location(), 0);
    GLine line({c1, c2, c3, c4});
    node.lines.push_back(line);
  }
  void add(Scope *scope) {
    GCell c1(CNG.generate("cell"), Cowstr::from(tsym->identifier()), 0);
    GCell c2(CNG.generate("cell"), tsym->name(), 0);
    GCell c3(CNG.generate("cell"), tsym->kind()._to_string(), 0);
    GCell c4(CNG.generate("cell"), tsym->location(), 0);
    GLine line({c1, c2, c3, c4});
    node.lines.push_back(line);
  }
  Cowstr id() { return node.id; }
  Cowstr str() { return node.str(); }
};

struct GEdgeID {
  GEdgeID(const Cowstr &a_id1) : id1(a_id1) {}
  GEdgeID(const Cowstr &a_id1, const Cowstr &a_id2) : id1(a_id1), id2(a_id2) {}

  Cowstr id1;
  Cowstr id2;

  Cowstr str() { return id2.empty() ? id1 : (id1 + ":" + id2); }
};

struct GEdge {};

} // namespace detail

int Drawer::draw(Ast *ast, const Cowstr &output) { return 0; }
