// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "boost/core/noncopyable.hpp"
#include "boost/preprocessor/cat.hpp"
#include "iface/Nameable.h"
#include "infra/Cowstr.h"
#include <unordered_map>
#include <vector>

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
class Visitor : public Nameable, private boost::noncopyable {
public:
  Visitor(const Cowstr &name = "Visitor");
  virtual ~Visitor() = default;

  // get binder, context and visitor
  virtual VisitorBinder *binder() const;
  virtual VisitorContext *context() const;
  virtual Visitor *visitor(AstKind astKind) const;

  // do nothing by default
  virtual void visit(Ast *ast);
  virtual void visitBefore(Ast *ast, Ast *child);
  virtual void visitAfter(Ast *ast, Ast *child);
  virtual void finishVisit(Ast *ast);

  static void traverse(VisitorBinder *binder, Ast *ast);

protected:
  VisitorBinder *visitorBinder_;
  friend class VisitorBinder;
};

// get visitor for ast
class VisitorBinder : private boost::noncopyable {
public:
  VisitorBinder(VisitorContext *context = nullptr);
  virtual ~VisitorBinder() = default;
  virtual int bind(AstKind kind, Visitor *visitor);
  virtual Visitor *visitor(AstKind kind) const;
  virtual VisitorContext *context() const;

protected:
  VisitorContext *context_;
  std::unordered_map<int, Visitor *> visitors_;
};

#define VISITOR(x) BOOST_PP_CAT(x, Visitor)
