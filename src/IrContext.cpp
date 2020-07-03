// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrContext.h"
#include "Exception.h"
#include "IrUtil.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

IrContext *IrContext::instance = nullptr;

void IrContext::initialize(const std::string &fileName) {
  if (instance) {
    delete instance;
    instance = nullptr;
  }
  instance = new IrContext(fileName);
}

IrContext *IrContext::get() {
  EX_ASSERT(instance, "instance must not be null");
  return instance;
}

IrContext::IrContext(const std::string &a_fileName)
    : fileName(a_fileName), symbolTable(nullptr), llvmContext(),
      llvmBuilder(llvmContext), llvmModule(nullptr), llvmLegacyFPM(nullptr) {
  llvmModule = new llvm::Module(IrUtil::prefix() + fileName, llvmContext);
  llvmLegacyFPM = new llvm::legacy::FunctionPassManager(llvmModule);
  llvmLegacyFPM->add(llvm::createInstructionCombiningPass());
  llvmLegacyFPM->add(llvm::createReassociatePass());
  llvmLegacyFPM->add(llvm::createGVNPass());
  llvmLegacyFPM->add(llvm::createCFGSimplificationPass());
  llvmLegacyFPM->add(llvm::createPromoteMemoryToRegisterPass());
  llvmLegacyFPM->doInitialization();
}

void IrContext::release() {
  delete llvmModule;
  llvmModule = nullptr;
  delete llvmLegacyFPM;
  llvmLegacyFPM = nullptr;
}

