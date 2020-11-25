// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/Visitor.h"
#include "Ast.h"

#define ACCEPT(x)                                                              \
  do {                                                                         \
    if (ast->x) {                                                              \
      ast->x->accept(this);                                                    \
    }                                                                          \
  } while (0)

void Visitor::visitInteger(A_Integer *ast) {}
void Visitor::visitFloat(A_Float *ast) {}
void Visitor::visitBoolean(A_Boolean *ast) {}
void Visitor::visitCharacter(A_Character *ast) {}
void Visitor::visitString(A_String *ast) {}
void Visitor::visitNil(A_Nil *ast) {}
void Visitor::visitVoid(A_Void *ast) {}
void Visitor::visitVarId(A_VarId *ast) {}

void Visitor::visitThrow(A_Throw *ast) { ACCEPT(expr); }
void Visitor::visitReturn(A_Return *ast) { ACCEPT(expr); }

void Visitor::visitBreak(A_Break *ast) {}
void Visitor::visitContinue(A_Continue *ast) {}

void Visitor::visitAssign(A_Assign *ast) {
  ACCEPT(assignor);
  ACCEPT(assignee);
}
void Visitor::visitPostfix(A_Postfix *ast) { ACCEPT(expr); }
void Visitor::visitInfix(A_Infix *ast) {
  ACCEPT(left);
  ACCEPT(right);
}
void Visitor::visitPrefix(A_Prefix *ast) { ACCEPT(expr); }
void Visitor::visitCall(A_Call *ast) {
  ACCEPT(args);
  ACCEPT(id);
}
void Visitor::visitExprs(A_Exprs *ast) {
  ACCEPT(expr);
  ACCEPT(next);
}
void Visitor::visitIf(A_If *ast) {
  ACCEPT(condition);
  ACCEPT(thenp);
  ACCEPT(elsep);
}
void Visitor::visitLoop(A_Loop *ast) {
  ACCEPT(condition);
  ACCEPT(body);
}
void Visitor::visitYield(A_Yield *ast) { ACCEPT(expr); }
void Visitor::visitLoopCondition(A_LoopCondition *ast) {
  ACCEPT(init);
  ACCEPT(condition);
  ACCEPT(update);
}
void Visitor::visitLoopEnumerator(A_LoopEnumerator *ast) {
  ACCEPT(expr);
  ACCEPT(type);
  ACCEPT(id);
}
void Visitor::visitDoWhile(A_DoWhile *ast) {
  ACCEPT(body);
  ACCEPT(condition);
}
void Visitor::visitTry(A_Try *ast) {
  ACCEPT(tryp);
  ACCEPT(catchp);
  ACCEPT(finallyp);
}
void Visitor::visitBlock(A_Block *ast) { ACCEPT(blockStats); }
void Visitor::visitBlockStats(A_BlockStats *ast) {
  ACCEPT(blockStat);
  ACCEPT(next);
}

void Visitor::visitPlainType(A_PlainType *ast) {}

void Visitor::visitFuncDef(A_FuncDef *ast) {
  ACCEPT(resultType);
  ACCEPT(funcSign);
  ACCEPT(body);
}
void Visitor::visitFuncSign(A_FuncSign *ast) {
  ACCEPT(params);
  ACCEPT(id);
}
void Visitor::visitParams(A_Params *ast) {
  ACCEPT(param);
  ACCEPT(next);
}
void Visitor::visitParam(A_Param *ast) {
  ACCEPT(type);
  ACCEPT(id);
}
void Visitor::visitVarDef(A_VarDef *ast) {
  ACCEPT(expr);
  ACCEPT(type);
  ACCEPT(id);
}
void Visitor::visitTopStats(A_TopStats *ast) {
  ACCEPT(topStat);
  ACCEPT(next);
}
void Visitor::visitCompileUnit(A_CompileUnit *ast) { ACCEPT(topStats); }
