// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrUtil.h"
#include "Exception.h"
#include "Ir.h"
#include "NameGenerator.h"
#include "llvm/Support/raw_ostream.h"
#include <system_error>

#define PREFIX std::string("shp.ir.")

std::string IrUtil::namegen(const std::string &name) {
  static NameGenerator nameGenerator(PREFIX, ".", ".");
  return nameGenerator.generate(name);
}

static std::string toLLVMNameImpl(const std::string &name,
                                  const std::string prefix) {
  EX_ASSERT(name.length() > 0, "name {} length {} <= 0", name, name.length());
  std::string tmp(name);
  for (int i = 0; i < (int)tmp.length(); i++) {
    if (tmp[i] == '_') {
      tmp[i] = '.';
    }
  }
  return prefix + tmp;
}

static std::string fromLLVMNameImpl(const std::string &name,
                                    const std::string prefix) {
  EX_ASSERT(name.length() > prefix.length(),
            "name {} length {} <= prefix {} length {}", name, name.length(),
            prefix, prefix.length());
  std::string tmp(
      name.substr(prefix.length(), name.length() - prefix.length()));
  for (int i = 0; i < (int)tmp.length(); i++) {
    if (tmp[i] == '.') {
      tmp[i] = '_';
    }
  }
  return tmp;
}

std::string IrUtil::toLLVMName(const std::string &name) {
  return toLLVMNameImpl(name, PREFIX);
}

std::string IrUtil::fromLLVMName(const std::string &name) {
  return fromLLVMNameImpl(name, PREFIX);
}

void IrUtil::toStdout(IrContext *context) {
  EX_ASSERT(context, "context is null");
  context->llvmModule->print(llvm::outs(), nullptr);
}

void IrUtil::toStderr(IrContext *context) {
  EX_ASSERT(context, "context is null");
  context->llvmModule->print(llvm::errs(), nullptr);
}

#define LL ".ll"

std::error_code IrUtil::toFileOstream(IrContext *context,
                                      const std::string &fileName) {
  EX_ASSERT(context, "context is null");
  EX_ASSERT(!fileName.empty(), "fileName.empty false: {}", fileName);
  std::error_code errcode;
  llvm::raw_fd_ostream fos(fileName + LL, errcode);
  if (errcode) {
    return errcode;
  }
  context->llvmModule->print(fos, nullptr);
  fos.close();
  return errcode;
}

std::string IrUtil::toStringOstream(IrContext *context) {
  EX_ASSERT(context, "context is null");
  std::string output;
  llvm::raw_string_ostream sos(output);
  context->llvmModule->print(sos, nullptr);
  return output;
}
