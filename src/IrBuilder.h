// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Phase.h"
#include "Visitor.h"
#include "infra/Cowstr.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <vector>

class IrBuilder : public Phase {
public:
  IrBuilder();
  virtual ~IrBuilder();
  virtual void run(Ast *ast);
  virtual const Cowstr &llvmLL() const;
  virtual llvm::LLVMContext &llvmContext();
  virtual const llvm::LLVMContext &llvmContext() const;
  virtual llvm::IRBuilder<> &llvmIRBuilder();
  virtual const llvm::IRBuilder<> &llvmIRBuilder() const;
  virtual llvm::Module *&llvmModule();
  virtual const llvm::Module *llvmModule() const;

private:
  llvm::LLVMContext llvmContext_;
  llvm::IRBuilder<> llvmIRBuilder_;
  llvm::Module *llvmModule_;
  Cowstr llvmLL_;
  std::vector<Visitor *> visitors_;
  VisitorContext *context_;
  VisitorBinder binder_;
};
