// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Phase.h"
#include "Visitor.h"

class Ast;
class Scope;

class SymbolBuilder : public Phase {
public:
  SymbolBuilder();
  virtual ~SymbolBuilder();
  virtual void run(Ast *ast);

private:
  // do nothing
  VISITOR_DECL0(Integer);
  VISITOR_DECL0(Float);
  VISITOR_DECL0(Boolean);
  VISITOR_DECL0(Character);
  VISITOR_DECL0(String);
  VISITOR_DECL0(Nil);
  VISITOR_DECL0(Void);
  VISITOR_DECL0(VarId);
  VISITOR_DECL0(Break);
  VISITOR_DECL0(Continue);
  VISITOR_DECL0(PlainType);
  VISITOR_DECL0(Throw);
  VISITOR_DECL0(Return);
  VISITOR_DECL0(Assign);
  VISITOR_DECL0(PostfixExpr);
  VISITOR_DECL0(PrefixExpr);
  VISITOR_DECL0(InfixExpr);
  VISITOR_DECL0(Call);
  VISITOR_DECL0(Exprs);
  VISITOR_DECL0(If);
  VISITOR_DECL0(Yield);
  VISITOR_DECL0(LoopCondition);
  VISITOR_DECL0(DoWhile);
  VISITOR_DECL0(Try);
  VISITOR_DECL0(BlockStats);
  VISITOR_DECL0(FuncSign);
  VISITOR_DECL0(Params);
  VISITOR_DECL0(TopStats);

  // do visit action
  VISITOR_DECL2(Loop);
  VISITOR_DECL1(LoopEnumerator);
  VISITOR_DECL2(Block);
  VISITOR_DECL1(VarDef);
  VISITOR_DECL1(Param);
  VISITOR_DECL2(FuncDef);
  VISITOR_DECL2(CompileUnit);

  struct Context : public VisitorContext {
    Context();
    virtual ~Context() = default;
    Scope *scope;
  } context_;
  VisitorBinder binder_;
  std::vector<Visitor *> visitors_;
};

class SymbolReviewer : public Phase {
public:
  SymbolReviewer();
  virtual ~SymbolReviewer();
  virtual void run(Ast *ast);
};
