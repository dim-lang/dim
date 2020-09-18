// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Files.h"
#include "container/Cowstr.h"
#include <memory>
#include <unordered_map>
class Ast;
class Scope;

/**
 * <TR><TD PORT="id"> xxx - xxx - xxx </TD></TR>
 */
class GraphLine {
public:
  GraphLine(const std::vector<Cowstr> &a_cells = {});
  virtual ~GraphLine() = default;

  Cowstr id; // PORT="id"
  std::vector<Cowstr> cells;

  virtual void add(const Cowstr &cell);
  virtual Cowstr str() const;
};

/**
 * <TABLE attributes> xxx </TABLE>
 */
class GraphLabel {
public:
  GraphLabel(const std::vector<GraphLine> &a_lines = {},
             const std::unordered_map<Cowstr, Cowstr> &a_attributes = {
                 {"BORDER", "\"0\""},
                 {"CELLBORDER", "\"1\""},
                 {"CELLSPACING", "\"0\""},
             });
  virtual ~GraphLabel() = default;

  std::vector<GraphLine> lines;
  std::unordered_map<Cowstr, Cowstr> attributes;

  virtual const Cowstr &id() const;
  virtual Cowstr str() const;
};

/**
 * node
 * node [label=< xxx >]
 */
class GraphNode {
public:
  GraphNode(const std::vector<GraphLine> &a_lines = {},
            const std::unordered_map<Cowstr, Cowstr> &a_attributes = {
                {"BORDER", "\"0\""},
                {"CELLBORDER", "\"1\""},
                {"CELLSPACING", "\"0\""},
            });
  virtual ~GraphNode() = default;

  Cowstr id;
  std::shared_ptr<GraphLabel> label;

  virtual Cowstr str() const;
};

/**
 * from -> to
 * fromNode:fromField -> toNode:toField
 * fromNode:fromField -> toNode:toField [label=< xxx >]
 */
class GraphEdge {
public:
  GraphEdge(const Cowstr &from, const Cowstr &to,
            std::shared_ptr<GraphLabel> label = nullptr);
  GraphEdge(const Cowstr &fromNode, const Cowstr &fromField,
            const Cowstr &toNode, const Cowstr &toField,
            std::shared_ptr<GraphLabel> label = nullptr);
  virtual ~GraphEdge() = default;
  virtual Cowstr str() const;

protected:
  Cowstr from_;
  Cowstr to_;
  std::shared_ptr<GraphLabel> label_;
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
 *   node1:id1 -> node2:id2 [label=< xxx >]
 *   node3:id3 -> node3:id3 [label=< xxx >]
 *   ...
 * }
 */
class Graph {
public:
  Graph(const Cowstr &fileName,
        const std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>>
            &a_attributes = {});
  virtual ~Graph() = default;

  std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>> attributes;
  std::vector<std::shared_ptr<GraphNode>> nodes;
  std::vector<std::shared_ptr<GraphEdge>> edges;

  virtual int draw();

protected:
  Cowstr fileName_;
};

class AstGraph {
public:
  AstGraph(Ast *ast);
  virtual ~AstGraph() = default;
  virtual int draw(const Cowstr &output);

private:
  Ast *ast_;
};

class SymbolGraph {
public:
  SymbolGraph(std::shared_ptr<Scope> scope);
  virtual ~SymbolGraph() = default;
  virtual int draw(const Cowstr &output);

private:
  std::shared_ptr<Scope> scope_;
};
