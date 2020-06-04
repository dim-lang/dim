// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Output.h"
#include "Log.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <sstream>

std::error_code output_fd(IrTranslateUnit *tunit, IrContext *context,
                          const std::string &fileName) {
  LOG_ASSERT(tunit, "tunit is null");
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(!fileName.empty(), "fileName.empty false: {}", fileName);

  std::error_code errcode;
  llvm::raw_fd_ostream fd(fileName, errcode);
  if (errcode) {
    return errcode;
  }
  for (int i = 0; i < tunit->size(); i++) {
    IrDefinition *ir = tunit->get(i);
    llvm::Value *v = ir->codeGen(context);
    LOG_ASSERT(v, "v is null");
    v->print(fd);
  }
  fd.close();
  return errcode;
}

std::string output_string(IrTranslateUnit *tunit, IrContext *context) {
  LOG_ASSERT(tunit, "tunit is null");
  LOG_ASSERT(context, "context is null");

  std::stringstream ss;
  for (int i = 0; i < tunit->size(); i++) {
    IrDefinition *ir = tunit->get(i);
    llvm::Value *v = ir->codeGen(context);
    LOG_ASSERT(v, "v is null");
    std::string tmp;
    llvm::raw_string_ostream sos(tmp);
    v->print(sos);
    ss << sos.str();
    ss << "\n";
  }
  return ss.str();
}
