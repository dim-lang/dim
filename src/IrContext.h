// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "SymbolTable.h"
#include "boost/core/noncopyable.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include <string>

class IrContext : private boost::noncopyable {
public:
  IrContext(const std::string &a_moduleName);
  virtual ~IrContext();

  std::string moduleName;
  SymbolTable *symbolTable;
  llvm::LLVMContext llvmContext;
  llvm::IRBuilder<> llvmBuilder;
  llvm::Module *llvmModule;
  llvm::legacy::FunctionPassManager *llvmLegacyFPM;
};
