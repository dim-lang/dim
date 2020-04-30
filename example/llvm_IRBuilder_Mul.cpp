// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "fmt/format.h"
#include "llvm_IRBuilder_Util.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
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

  // case: i8 * i8
  fmt::print("dump i8 * i8, i8 min: {}, i8 max: {}\n", INT8_MIN, (int)INT8_MAX);
  {
    // -7 * -10 = 70
    int8_t a = -7;
    int8_t b = -10;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("{} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -12 * 50 = ? overflow
    int8_t a = -12;
    int8_t b = 50;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 13 * -127 = ? overflow
    int8_t a = 13;
    int8_t b = -127;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: i8 * u8
  fmt::print("\ndump i8 * u8, i8 min: {}, i8 max: {}, u8 min: {}, u8 max: {}\n",
             INT8_MIN, (int)INT8_MAX, 0U, UINT8_MAX);
  {
    // 7 * 12 = 84
    int8_t a = 7;
    uint8_t b = 12;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("{} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -15 * 5 = -75
    int8_t a = -15;
    uint8_t b = 5;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("{} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -1 * 255 = ? overflow
    int8_t a = -1;
    uint8_t b = 255;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: u8 * u8
  fmt::print("\ndump u8 * u8, u8 min: {}, u8 max: {}\n", 0U, UINT8_MAX);
  {
    // 7 * 13 = 91
    uint8_t a = 7;
    uint8_t b = 13;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("{} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 10 * 29 = 290 overflow
    uint8_t a = 10;
    uint8_t b = 29;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateMul(p, q, getOpTmp(Op::Mul));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: f32 * f32
  fmt::print("\ndump f32 * f32, f32 min: {}, f32 max: {}\n", FLT_MIN, FLT_MAX);
  {
    // 31.2 * 73.1 = 2280.72
    float a = 31.2;
    float b = 73.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateFMul(p, q, getOpTmp(Op::Mul));
    fmt::print("{} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // FLT_MAX-1.1 * 2.1 = ? overflow
    float a = FLT_MAX - 1.1;
    float b = 2.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateFMul(p, q, getOpTmp(Op::Mul));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Mul), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  return 0;
}
