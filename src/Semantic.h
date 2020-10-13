// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Visitor.h"
#include <memory>

class Scope;

#define SB_DECL1(x)                                                            \
  class x : public BaseVisitor {                                               \
  public:                                                                      \
    x() {}                                                                     \
    virtual ~x() = default;                                                    \
    virtual VisitorKind kind() const { return VisitorKind::x; }                \
  };

#define SB_DECL2(x)                                                            \
  class x : public BaseVisitor {                                               \
  public:                                                                      \
    x() {}                                                                     \
    virtual ~x() = default;                                                    \
    virtual VisitorKind kind() const { return VisitorKind::x; }                \
    virtual void visit(VisitorBinder *binder, Ast *anode);                     \
  };

class SymbolBuilder {
public:
  SymbolBuilder();
  virtual ~SymbolBuilder();
  virtual void run(Ast *anode);

  class BaseVisitor : public Visitor {
  public:
    BaseVisitor() : scope_(nullptr) {}
    virtual Scope *&scope() { return scope_; }
    virtual Scope *scope() const { return scope_; }

  protected:
    Scope *scope_;
  };

  // do nothing
  SB_DECL1(Integer)
  SB_DECL1(Float)
  SB_DECL1(Boolean)
  SB_DECL1(Character)
  SB_DECL1(String)
  SB_DECL1(Nil)
  SB_DECL1(Void)
  SB_DECL1(VarId)
  SB_DECL1(Break)
  SB_DECL1(Continue)
  SB_DECL1(PlainType)

  // pass scope parameter
  SB_DECL2(Throw)
  SB_DECL2(Return)
  SB_DECL2(Assign)
  SB_DECL2(PostfixExpr)
  SB_DECL2(PrefixExpr)
  SB_DECL2(InfixExpr)
  SB_DECL2(Call)
  SB_DECL2(Exprs)
  SB_DECL2(If)
  SB_DECL2(Yield)
  SB_DECL2(LoopCondition)
  SB_DECL2(DoWhile)
  SB_DECL2(Try)
  SB_DECL2(BlockStats)
  SB_DECL2(FuncSign)
  SB_DECL2(Params)
  SB_DECL2(TopStats)

  // do visit action
  SB_DECL2(Loop)
  SB_DECL2(LoopEnumerator)
  SB_DECL2(Block)
  SB_DECL2(VarDef)
  SB_DECL2(Param)
  SB_DECL2(FuncDef)
  SB_DECL2(CompileUnit)

private:
  VisitorBinder *binder_;
};

#undef SB_DECL1
#undef SB_DECL2

class SymbolReviewer {
public:
  SymbolReviewer();
  virtual ~SymbolReviewer();
  virtual void run(Ast *anode);
};
