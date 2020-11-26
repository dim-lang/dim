// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

class IrUtil {
public:
  // stringize LLVM:
  //  llvm::Value
  //  llvm::Function
  //  llvm::Type
  template <class T> static std::string stringize(T *t) {
    std::string output;
    llvm::raw_string_ostream sos(output);
    t->print(sos);
    return sos.str();
  }
};
