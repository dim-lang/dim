// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "SymbolClasses.h"
#include "iface/Phase.h"
#include "iface/Visitor.h"

class SymbolResolver : public Phase, public Visitor {
public:
  SymbolResolver();
  virtual ~SymbolResolver();
  virtual void run(Ast *ast);

  virtual void visitLoop(A_Loop *ast);
  virtual void visitBlock(A_Block *ast);
  virtual void visitFuncDef(A_FuncDef *ast);
  virtual void visitFuncSign(A_FuncSign *ast);
  virtual void visitCompileUnit(A_CompileUnit *ast);
  virtual void visitVarId(A_VarId *ast);

private:
  Scope *currentScope_;
};
