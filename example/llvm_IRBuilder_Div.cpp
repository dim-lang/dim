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

  // case: i8 / i8
  fmt::print("dump i8 / i8, i8 min: {}, i8 max: {}\n", INT8_MIN, (int)INT8_MAX);
  {
    // -10 / -7 = 1
    int8_t a = -10;
    int8_t b = -7;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // 52 / -10 = -5
    int8_t a = 52;
    int8_t b = -10;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // -127 / 13 = 10
    int8_t a = -127;
    int8_t b = 13;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }

  // case: i8 / u8
  fmt::print("\ndump i8 / u8, i8 min: {}, i8 max: {}, u8 min: {}, u8 max: {}\n",
             INT8_MIN, (int)INT8_MAX, 0U, UINT8_MAX);
  {
    // 12 / 7 = 1
    int8_t a = 12;
    uint8_t b = 7;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // 127 / -1 = -127
    int8_t a = -1;
    uint8_t b = 255;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }

  // case: u8 / u8
  fmt::print("\ndump u8 / u8, u8 min: {}, u8 max: {}\n", 0U, UINT8_MAX);
  {
    // 7 / 13 = 0
    uint8_t a = 7;
    uint8_t b = 13;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // 29 / 10 = 20
    uint8_t a = 29;
    uint8_t b = 10;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }

  // case: f32 / f32
  fmt::print("\ndump f32 / f32, f32 min: {}, f32 max: {}\n", FLT_MIN, FLT_MAX);
  {
    // 73.1 / 13.82 = 5.289
    float a = 73.1;
    float b = 13.82;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }
  {
    // FLT_MAX-1.1 / 0.1 = ? overflow
    float a = FLT_MAX - 1.1;
    float b = 0.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    llvm::raw_os_ostream os(std::cout);
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Div), b);
    r->print(os, true);
    fmt::print("\n");
  }
  return 0;
}
