// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "fmt/format.h"
#include "llvm_IRBuilder_Util.h"
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

int main(void) {
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);

  // case: i8 - i8
  fmt::print("dump i8 - i8, i8 min: {}, i8 max: {}\n", INT8_MIN, INT8_MAX);
  {
    // -7 - -101 = 94
    int8_t a = -7;
    int8_t b = -101;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // -128 - 1 = ? overflow
    int8_t a = -128;
    int8_t b = 1;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // 1 - -127 = ? overflow
    int8_t a = 1;
    int8_t b = -127;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }

  // case: i8 - u8
  fmt::print("\ndump i8 - u8, i8 min: {}, i8 max: {}, u8 min: {}, u8 max: {}\n",
             INT8_MIN, INT8_MAX, 0U, UINT8_MAX);
  {
    // 7 - 102 = -95
    int8_t a = 7;
    uint8_t b = 102;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // -10 - 55 = -65
    int8_t a = -10;
    uint8_t b = 55;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // -1 - 128 = ? overflow
    int8_t a = -1;
    uint8_t b = 128;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }

  // case: u8 - u8
  fmt::print("\ndump u8 - u8, u8 min: {}, u8 max: {}\n", 0U, UINT8_MAX);
  {
    // 37 - 10 = 27
    uint8_t a = 37;
    uint8_t b = 10;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // 1 - 2 = ? overflow
    uint8_t a = 1;
    uint8_t b = 2;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(os, true);
    fmt::print("\n");
  }
  return 0;
}
