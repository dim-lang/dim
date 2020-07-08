// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Output.h"
#include "Exception.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <sstream>

#define LL std::string(".ll")

#if 0
std::error_code output_fd(IrTranslateUnit *tunit, const std::string &fileName) {
  EX_ASSERT(tunit, "tunit is null");
  EX_ASSERT(!fileName.empty(), "fileName.empty false: {}", fileName);

  std::error_code errcode;
  llvm::raw_fd_ostream fd(fileName + LL, errcode);
  if (errcode) {
    return errcode;
  }
  for (int i = 0; i < tunit->size(); i++) {
    IrDefinition *ir = tunit->get(i);
    llvm::Value *v = ir->codeGen();
    EX_ASSERT(v, "v is null");
    v->print(fd);
  }
  fd.close();
  return errcode;
}

std::string output_string(IrTranslateUnit *tunit) {
  EX_ASSERT(tunit, "tunit is null");

  std::stringstream ss;
  for (int i = 0; i < tunit->size(); i++) {
    IrDefinition *ir = tunit->get(i);
    llvm::Value *v = ir->codeGen();
    EX_ASSERT(v, "v is null");
    std::string tmp;
    llvm::raw_string_ostream sos(tmp);
    v->print(sos);
    ss << sos.str();
    ss << "\n";
  }
  return ss.str();
}
#endif

llvm::raw_ostream &output_os(IrContext *context, llvm::raw_ostream &os) {
  EX_ASSERT(context, "context is null");
  context->llvmModule->print(os, nullptr);
  return os;
}

std::error_code output_file(IrContext *context, const std::string &fileName) {
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

std::string output_string(IrContext *context) {
  EX_ASSERT(context, "context is null");

  std::string s;
  llvm::raw_string_ostream os(s);
  context->llvmModule->print(os, nullptr);
  return s;
}
