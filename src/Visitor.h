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
class VisitorContext;

class VisitorContext {
public:
  virtual ~VisitorContext() = default;
};

// get visitor for ast
class VisitorBinder {
public:
  VisitorBinder(VisitorContext *context = nullptr);
  virtual ~VisitorBinder() = default;
  virtual int bind(int astKind, Visitor *visitor);
  virtual Visitor *get(int astKind) const;
  virtual Visitor *get(Ast *ast) const;
  virtual void setContext(VisitorContext *context);
  virtual VisitorContext *context() const;

protected:
  std::unordered_map<int, Visitor *> visitors_;
  VisitorContext *context_;
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
 * Fristly visitor arrive ast, do the `visit` action. Then visit child
 * node a, b and c seperately, Finally visitor do `post-visit` action before
 * leaves ast.
 * If ast doesn't have any child node, `visit` `post-visit` action will still
 * been execute as well.
 */
class Visitor {
public:
  Visitor() = default;
  virtual ~Visitor() = default;

  // do nothing by default
  virtual void visit(Ast *ast, VisitorContext *context);
  virtual void postVisit(Ast *ast, VisitorContext *context);

  static void traverse(VisitorBinder *binder, Ast *ast);
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
