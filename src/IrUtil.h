// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "IrContext.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

class IrUtil {
public:
  static std::string namegen(const std::string &name);

  // source code function name translation rule, such as:
  // `format_print` to `shp.ir.format.print`
  // `FormatPrint` to `shp.ir.FormatPrint`
  static std::string toLLVMName(const std::string &name);

  // source code function name reverse translation rule, such as:
  // `shp.ir.format.print` to `format_print`
  // `shp.ir.FormatPrint` to `FormatPrint`
  static std::string fromLLVMName(const std::string &name);

  // dump LLVM
  // - llvm::Value
  // - llvm::Function
  // - llvm::Type
  template <class T> static std::string dumpLLVM(T *t) {
    std::string output;
    llvm::raw_string_ostream sos(output);
    t->print(sos);
    return sos.str();
  }
};
