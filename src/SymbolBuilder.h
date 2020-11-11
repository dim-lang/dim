// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "SymbolClasses.h"
#include "iface/Phase.h"
#include "iface/Visitor.h"

class SymbolBuilder : public Phase, public Visitor {
public:
  SymbolBuilder();
  virtual ~SymbolBuilder() = default;
  virtual void run(Ast *ast);

  virtual void visitLoop(A_Loop *ast);
  virtual void visitLoopEnumerator(A_LoopEnumerator *ast);
  virtual void visitBlock(A_Block *ast);
  virtual void visitFuncDef(A_FuncDef *ast);
  virtual void visitVarDef(A_VarDef *ast);
  virtual void visitCompileUnit(A_CompileUnit *ast);

private:
  Scope *currentScope_;
};
