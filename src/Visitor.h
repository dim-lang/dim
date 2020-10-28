// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "iface/Nameable.h"
#include "infra/Cowstr.h"
#include <unordered_map>
#include <vector>

class Ast;
class Visitor;
class VisitorBinder;

class VisitorContext {
public:
  virtual ~VisitorContext() = default;
};

/**
 * For any tree node which contains N (N >= 0) children node:
 *
 *         ast
 *        / |..\
 *       c1 c2  cN
 *
 * visitor traverse an entire tree with actions:
 *  - visit
 *  - visit before child
 *  - visit after child
 *  - finish visit
 *
 * For example traverse tree node below:
 *
 *        ast
 *       / | \
 *      a  b  c
 *
 * 1. Firstly visitor arrive `ast`, do `visit` action.
 * 2. Before visit each children node a, b and c seperately, visitor do
 * `visit-before-child` action.
 * 3. After visit each children node, visitor do `visit-before-child` action.
 * 4. Finally when visitor leave `ast`, do `finish-visit` action.
 *
 * `visit` and `finish-visit` actions will always been executed even if ast
 * doesn't have any children node.
 *
 * `visit-before-child` and `visit-after-child` actions will been executed only
 * if ast has any child node.
 */
class Visitor : public Nameable {
public:
  Visitor(const Cowstr &name = "Visitor");
  virtual ~Visitor() = default;

  // do nothing by default
  virtual void visit(Ast *ast, VisitorContext *context);
  virtual void visitBefore(Ast *ast, Ast *child, VisitorContext *context);
  virtual void visitAfter(Ast *ast, Ast *child, VisitorContext *context);
  virtual void finishVisit(Ast *ast, VisitorContext *context);

  static void traverse(VisitorBinder *binder, Ast *ast);
};

// get visitor for ast
class VisitorBinder {
public:
  VisitorBinder(VisitorContext *context = nullptr);
  virtual ~VisitorBinder();
  virtual int bind(int astKind, Visitor *visitor);
  virtual Visitor *get(int astKind) const;
  virtual Visitor *get(Ast *ast) const;
  virtual VisitorContext *context() const;

protected:
  std::unordered_map<int, Visitor *> visitors_;
  VisitorContext *context_;
  Visitor *idleVisitor_;
};
