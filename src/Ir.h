// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"

class Ast;

class Ir {
public:
  virtual ~Ir() = default;
  virtual llvm::Value *codeGen() = 0;
};

class IrExpressionList : public Ir {
public:
  IrExpressionList(Ast *node);
};
