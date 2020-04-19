// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"
#include <cfloat>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

enum class Op { Add = 1, Sub, Mul, Div };

const char *getOpTmp(Op op);
const char *getOpName(Op op);

#define dumpInt(signedA, a, signedB, b, bits, context, builder, op, x)         \
  do {                                                                         \
    llvm::Value *aa =                                                          \
        llvm::ConstantInt::get(context, llvm::APInt(bits, a, signedA));        \
    llvm::Value *bb =                                                          \
        llvm::ConstantInt::get(context, llvm::APInt(bits, b, signedB));        \
    llvm::Value *r = builder.x(aa, bb, getOpTmp(op));                          \
    llvm::raw_os_ostream os(std::cout);                                        \
    fmt::print("{} {} {} = ", a, getOpName(op), b);                            \
    r->print(os, true);                                                        \
    fmt::print("\n");                                                          \
  } while (0)

#define dumpFloat(a, b, context, builder, op, x)                               \
  do {                                                                         \
    llvm::Value *aa =                                                          \
        llvm::ConstantFP::get(context, llvm::APFloat(8, a, true));             \
    llvm::Value *bb =                                                          \
        llvm::ConstantFP::get(context, llvm::APFloat(8, b, true));             \
    llvm::Value *r = builder.x(aa, bb, getOpTmp(op));                          \
    llvm::raw_os_ostream os(std::cout);                                        \
    fmt::print("{} {} {} = ", a, getOpName(op), b);                            \
    r->print(os, true);                                                        \
    fmt::print("\n");                                                          \
  } while (0)
