// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrWriter.h"
#include "Exception.h"
#include "llvm/Support/raw_ostream.h"

#define LL ".ll"
#define BC ".bc"

IrLLWriter::IrLLWriter(IrContext *context) : context_(context) {
  EX_ASSERT(context_, "context_ is null");
}

void IrLLWriter::toStdout() {
  context_->llvmModule->print(llvm::outs(), nullptr);
}

void IrLLWriter::toStderr() {
  context_->llvmModule->print(llvm::errs(), nullptr);
}

std::pair<std::error_code, std::string> IrLLWriter::toFileOstream() {
  std::string fileName = context_->sourceName + LL;
  std::error_code errcode;
  llvm::raw_fd_ostream fos(fileName, errcode);
  if (errcode) {
    return std::make_pair(errcode, fileName);
  }
  context_->llvmModule->print(fos, nullptr);
  fos.close();
  return std::make_pair(errcode, fileName);
}

std::string IrLLWriter::toStringOstream() {
  std::string output;
  llvm::raw_string_ostream sos(output);
  context_->llvmModule->print(sos, nullptr);
  return output;
}

IrBitCodeWriter::IrBitCodeWriter(IrContext *context) : context_(context) {}

void IrBitCodeWriter::toStdout() {
  context_->llvmModule->print(llvm::outs(), nullptr);
}

void IrBitCodeWriter::toStderr() {
  context_->llvmModule->print(llvm::errs(), nullptr);
}

std::pair<std::error_code, std::string> IrBitCodeWriter::toFileOstream() {
  std::string fileName = context_->sourceName + BC;
  std::error_code errcode;
  llvm::raw_fd_ostream fos(fileName, errcode);
  if (errcode) {
    return std::make_pair(errcode, fileName);
  }
  context_->llvmModule->print(fos, nullptr);
  fos.close();
  return std::make_pair(errcode, fileName);
}

std::string IrBitCodeWriter::toStringOstream() {
  std::string output;
  llvm::raw_string_ostream sos(output);
  context_->llvmModule->print(sos, nullptr);
  return output;
}
