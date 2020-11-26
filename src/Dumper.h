// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "AstClasses.h"
#include "iface/Phase.h"
#include "iface/Visitor.h"
#include "infra/Cowstr.h"
#include <vector>

class Dumper : public Phase, public Visitor {
public:
  Dumper();
  virtual ~Dumper() = default;
  virtual void run(Ast *ast);
  virtual std::vector<Cowstr> &dump();
  virtual const std::vector<Cowstr> &dump() const;

  virtual void visitInteger(A_Integer *ast);
  virtual void visitFloat(A_Float *ast);
  virtual void visitBoolean(A_Boolean *ast);
  virtual void visitCharacter(A_Character *ast);
  virtual void visitString(A_String *ast);
  virtual void visitNil(A_Nil *ast);
  virtual void visitVoid(A_Void *ast);
  virtual void visitVarId(A_VarId *ast);
  virtual void visitBreak(A_Break *ast);
  virtual void visitContinue(A_Continue *ast);

  virtual void visitThrow(A_Throw *ast);
  virtual void visitReturn(A_Return *ast);
  virtual void visitAssign(A_Assign *ast);
  virtual void visitPostfix(A_Postfix *ast);
  virtual void visitInfix(A_Infix *ast);
  virtual void visitPrefix(A_Prefix *ast);
  virtual void visitCall(A_Call *ast);
  virtual void visitExprs(A_Exprs *ast);
  virtual void visitIf(A_If *ast);
  virtual void visitLoop(A_Loop *ast);
  virtual void visitYield(A_Yield *ast);
  virtual void visitLoopCondition(A_LoopCondition *ast);
  virtual void visitLoopEnumerator(A_LoopEnumerator *ast);
  virtual void visitDoWhile(A_DoWhile *ast);
  virtual void visitTry(A_Try *ast);
  virtual void visitBlock(A_Block *ast);
  virtual void visitBlockStats(A_BlockStats *ast);
  virtual void visitPlainType(A_PlainType *ast);
  virtual void visitFuncDef(A_FuncDef *ast);
  virtual void visitFuncSign(A_FuncSign *ast);
  virtual void visitParams(A_Params *ast);
  virtual void visitParam(A_Param *ast);
  virtual void visitVarDef(A_VarDef *ast);
  virtual void visitTopStats(A_TopStats *ast);
  virtual void visitCompileUnit(A_CompileUnit *ast);

private:
  std::vector<Cowstr> dump_;
  int indent_;
};
