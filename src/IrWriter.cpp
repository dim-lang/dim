// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrWriter.h"
#include "Exception.h"
#include "Log.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <system_error>

#define LL ".ll"
#define OBJ ".o"

IrLLWriter::IrLLWriter(IrContext *context) : context_(context) {
  EX_ASSERT(context_, "context_ is null");
}

void IrLLWriter::toStdout() {
  context_->llvmModule->print(llvm::outs(), nullptr);
}

void IrLLWriter::toStderr() {
  context_->llvmModule->print(llvm::errs(), nullptr);
}

std::string IrLLWriter::toFileOstream() {
  std::string llFileName = context_->sourceName + LL;
  std::error_code errcode;
  llvm::raw_fd_ostream fos(llFileName, errcode);
  EX_ASSERT(!errcode, "raw_fd_ostream open file error: errcode {}",
            errcode.message());
  context_->llvmModule->print(fos, nullptr);
  fos.close();
  return llFileName;
}

std::string IrLLWriter::toStringOstream() {
  std::string output;
  llvm::raw_string_ostream sos(output);
  context_->llvmModule->print(sos, nullptr);
  return output;
}

IrObjWriter::IrObjWriter(IrContext *context) : context_(context) {
  std::string targetTriple = llvm::sys::getDefaultTargetTriple();
  LOG_INFO("targetTriple: {}", targetTriple);
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetDisassembler();
  std::string errorMessage;
  const llvm::Target *target =
      llvm::TargetRegistry::lookupTarget(targetTriple, errorMessage);
  EX_ASSERT(target, "target must not null");
  std::string cpu = "generic";
  std::string features = "";
  llvm::TargetOptions opt;
  llvm::Optional<llvm::Reloc::Model> rm = llvm::Optional<llvm::Reloc::Model>();
  targetMachine_ =
      target->createTargetMachine(targetTriple, cpu, features, opt, rm);
  context_->llvmModule->setDataLayout(targetMachine_->createDataLayout());
  context_->llvmModule->setTargetTriple(targetTriple);
}

void IrObjWriter::toStdout() { EX_ASSERT(false, "method not support"); }

void IrObjWriter::toStderr() { EX_ASSERT(false, "method not support"); }

std::string IrObjWriter::toFileOstream() {
  std::string objFileName = context_->sourceName + OBJ;
  std::error_code errcode;
  llvm::raw_fd_ostream fos(objFileName, errcode, llvm::sys::fs::OF_None);
  EX_ASSERT(!errcode, "raw_fd_ostream open file error: errcode {}",
            errcode.message());
  llvm::legacy::PassManager passManager;
  if (targetMachine_->addPassesToEmitFile(passManager, fos, nullptr,
                                          llvm::CGFT_ObjectFile)) {
    EX_ASSERT(false, "targetMachine->addPassesToEmitFile must be true");
  }
  passManager.run(*context_->llvmModule);
  fos.flush();
  fos.close();
  return objFileName;
}

std::string IrObjWriter::toStringOstream() {
  EX_ASSERT(false, "method not support");
}
