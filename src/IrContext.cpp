// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrContext.h"
#include "Exception.h"
#include "IrUtil.h"
#include "NameGenerator.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

static NameGenerator nameGenerator("shp.ir", ".", ".");

IrContext::IrContext(const std::string &a_moduleName)
    : moduleName(a_moduleName), symbolTable(nullptr), llvmContext(),
      llvmBuilder(llvmContext), llvmModule(nullptr), llvmLegacyFPM(nullptr) {
  llvmModule =
      new llvm::Module(nameGenerator.generate(moduleName), llvmContext);
  llvmLegacyFPM = new llvm::legacy::FunctionPassManager(llvmModule);
  llvmLegacyFPM->add(llvm::createInstructionCombiningPass());
  llvmLegacyFPM->add(llvm::createReassociatePass());
  llvmLegacyFPM->add(llvm::createGVNPass());
  llvmLegacyFPM->add(llvm::createCFGSimplificationPass());
  llvmLegacyFPM->add(llvm::createPromoteMemoryToRegisterPass());
  llvmLegacyFPM->doInitialization();
}

IrContext::~IrContext() {
  if (llvmModule) {
    delete llvmModule;
    llvmModule = nullptr;
  }
  if (llvmLegacyFPM) {
    delete llvmLegacyFPM;
    llvmLegacyFPM = nullptr;
  }
}
