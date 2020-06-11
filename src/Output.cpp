// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Output.h"
#include "Log.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <sstream>

std::error_code output_fd(IrTranslateUnit *tunit, IrContext *context,
                          const std::string &sourceFileName) {
  EX_ASSERT(tunit, "tunit is null");
  EX_ASSERT(context, "context is null");
  EX_ASSERT(!sourceFileName.empty(), "sourceFileName.empty false: {}",
            sourceFileName);

  std::error_code errcode;
  std::string llFileName = sourceFileName + std::string(".ll");
  llvm::raw_fd_ostream fd(llFileName, errcode);
  if (errcode) {
    return errcode;
  }
  for (int i = 0; i < tunit->size(); i++) {
    IrDefinition *ir = tunit->get(i);
    llvm::Value *v = ir->codeGen(context);
    EX_ASSERT(v, "v is null");
    v->print(fd);
  }
  fd.close();
  return errcode;
}

std::string output_string(IrTranslateUnit *tunit, IrContext *context) {
  EX_ASSERT(tunit, "tunit is null");
  EX_ASSERT(context, "context is null");

  std::stringstream ss;
  for (int i = 0; i < tunit->size(); i++) {
    IrDefinition *ir = tunit->get(i);
    llvm::Value *v = ir->codeGen(context);
    EX_ASSERT(v, "v is null");
    std::string tmp;
    llvm::raw_string_ostream sos(tmp);
    v->print(sos);
    ss << sos.str();
    ss << "\n";
  }
  return ss.str();
}
