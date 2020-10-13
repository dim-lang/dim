// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "enum.h"
#include <unordered_map>
#include <vector>

class Ast;
class Visitor;
class VisitorBinder;

BETTER_ENUM(VisitorKind, int,
            // literal
            Integer = 5000, Float, Boolean, Character, String, Nil, Void,
            // id
            VarId,
            // expr without block
            Throw, Return, Break, Continue, Assign, PostfixExpr, PrefixExpr,
            InfixExpr, Call, Exprs,
            // expr with block
            If, Loop, Yield, LoopCondition, LoopEnumerator, DoWhile, Try, Block,
            BlockStats,
            // type
            PlainType,
            // declaration and definition
            FuncDef, FuncSign, Params, Param, VarDef,
            // compile unit
            CompileUnit, TopStats)

// get visitor for anode
class VisitorBinder {
public:
  VisitorBinder(const std::vector<Visitor *> &visitors = {});
  virtual ~VisitorBinder();
  virtual int bind(Visitor *visitor);
  virtual Visitor *get(Ast *anode) const;

protected:
  std::unordered_map<int, Visitor *> visitors_;
};

/**
 * For any tree node which contains N (N >= 0) child node:
 *
 *         anode
 *        / |...\
 *       c1 c2   cN
 *
 * visitor traverse an entire tree in preorder with:
 *  - visit
 *  - post visit
 *
 * For example traverse tree node below:
 *
 *        anode
 *       /  |  \
 *      a   b   c
 *
 * Fristly visitor arrive anode, do the `visit` action. Then visit child
 * node a, b and c seperately, Finally visitor do `post-visit` action before
 * leaves anode.
 * If anode doesn't have any child node, `visit` `post-visit` action will still
 * been execute as well.
 */
class Visitor {
public:
  virtual ~Visitor() = default;
  virtual VisitorKind kind() const = 0;

  // do nothing by default
  virtual void visit(VisitorBinder *binder, Ast *anode);
  virtual void postVisit(VisitorBinder *binder, Ast *anode);

  static void traverse(VisitorBinder *binder, Ast *anode);
};
