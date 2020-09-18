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
 * <TD PORT="id">value</TD>
 */
class GraphCell {
public:
  GraphCell(const Cowstr &value);
  virtual ~GraphCell() = default;
  virtual const Cowstr &id() const;
  virtual Cowstr str() const;

protected:
  Cowstr id_;    // PORT="id"
  Cowstr value_; // value
};

/**
 * <TR> xxx </TR>
 */
class GraphLine {
public:
  GraphLine(const std::vector<GraphCell> &cells = {});
  virtual ~GraphLine() = default;
  virtual void add(const GraphCell &cell);
  virtual Cowstr str() const;

protected:
  std::vector<GraphCell> cells_;
};

/**
 * <TABLE attributes> xxx </TABLE>
 */
class GraphLabel {
public:
  GraphLabel(const std::vector<GraphLine> &table = {},
             const std::unordered_map<Cowstr, Cowstr> &attributes = {});
  virtual ~GraphLabel() = default;
  virtual void add(const GraphLine &line);
  virtual Cowstr str() const;

protected:
  std::vector<GraphLine> table_;
  std::unordered_map<Cowstr, Cowstr> attributes_;
};

/**
 * node
 * node [label=< xxx >]
 */
class GraphNode {
public:
  GraphNode(std::shared_ptr<GraphLabel> label);
  GraphNode(const std::vector<GraphLine> &table = {},
            const std::unordered_map<Cowstr, Cowstr> &attributes = {});
  virtual ~GraphNode() = default;
  virtual const Cowstr &id() const;
  virtual Cowstr str() const;

protected:
  Cowstr id_;
  std::shared_ptr<GraphLabel> label_;
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
            &attributes = {});
  virtual ~Graph() = default;

  virtual void addNode(std::shared_ptr<GraphNode> node);
  virtual void addEdge(std::shared_ptr<GraphEdge> edge);
  virtual int draw();

protected:
  Cowstr fileName_;

  // graph, node, edge
  std::unordered_map<Cowstr, std::unordered_map<Cowstr, Cowstr>> attributes_;

  std::vector<std::shared_ptr<GraphNode>> nodes_;
  std::vector<std::shared_ptr<GraphEdge>> edges_;
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
