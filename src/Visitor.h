// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Name.h"
#include "enum.h"
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
 * For any tree node which contains N (N >= 0) child node:
 *
 *         ast
 *        / |..\
 *       c1 c2  cN
 *
 * visitor traverse an entire tree in preorder with:
 *  - visit
 *  - post visit
 *
 * For example traverse tree node below:
 *
 *        ast
 *       / | \
 *      a  b  c
 *
 * Fristly visitor arrive ast, do the `visit` action.
 * After visit all child node a, b and c, do `post-visit` action.
 *
 * These 2 actions still execute even if ast doesn't have any children.
 */
class Visitor : public Nameable {
public:
  Visitor(const Cowstr &name = "IdleVisitor");
  virtual ~Visitor() = default;

  // do nothing by default
  virtual void visit(Ast *ast, VisitorContext *context);
  virtual void postVisit(Ast *ast, VisitorContext *context);

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

#define VISITOR_DECL0(x)                                                       \
  class x : public Visitor {}

#define VISITOR_DECL1(x)                                                       \
  class x : public Visitor {                                                   \
  public:                                                                      \
    virtual void visit(Ast *ast, VisitorContext *context);                     \
  }

#define VISITOR_DECL2(x)                                                       \
  class x : public Visitor {                                                   \
  public:                                                                      \
    virtual void visit(Ast *ast, VisitorContext *context);                     \
    virtual void postVisit(Ast *ast, VisitorContext *context);                 \
  }
