// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "fmt/format.h"
#include "llvms/Util.h"
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

llvm::LLVMContext context;
llvm::IRBuilder<> builder(context);

void add() {
  // case: i8 + i8
  fmt::print("dump i8 + i8, i8 min: {}, i8 max: {}\n", INT8_MIN, (int)INT8_MAX);
  {
    // -7 + -101 = -108
    int8_t a = -7;
    int8_t b = -101;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("{} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 127 + -128 = -1
    int8_t a = 127;
    int8_t b = -128;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("{} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 127 + 1 = -128 overflow
    int8_t a = 127;
    int8_t b = 1;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -77 + -100 = ? overflow
    int8_t a = -77;
    int8_t b = -100;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: i8 + u8
  fmt::print("\ndump i8 + u8, i8 min: {}, i8 max: {}, u8 min: {}, u8 max: {}\n",
             INT8_MIN, (int)INT8_MAX, 0U, UINT8_MAX);
  {
    // 7 + 245 = 252
    int8_t a = 7;
    uint8_t b = 245;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("{} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -128 + 255 = 127
    int8_t a = -128;
    uint8_t b = 255;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("{} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 88 + 250 = ? overflow
    int8_t a = 88;
    uint8_t b = 250;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: u8 + u8
  fmt::print("\ndump u8 + u8, u8 min: {}, u8 max: {}\n", 0U, UINT8_MAX);
  {
    // 7 + 248 = 255
    uint8_t a = 7;
    uint8_t b = 248;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("{} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 1 + 255 = ? overflow
    uint8_t a = 1;
    uint8_t b = 255;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateAdd(p, q, getOpTmp(Op::Add));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: f32 + f32
  fmt::print("\ndump f32 + f32, f32 min: {}, f32 max: {}\n", FLT_MIN, FLT_MAX);
  {
    // 1.02 + 23.1 = 24.12
    float a = 1.02;
    float b = 23.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateFAdd(p, q, getOpTmp(Op::Add));
    fmt::print("{} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // FLT_MAX-1.1 + 20.1 = ? overflow
    float a = FLT_MAX - 1.1;
    float b = 20.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateFAdd(p, q, getOpTmp(Op::Add));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Add), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
}

void div() {
  // case: i8 / i8
  fmt::print("dump i8 / i8, i8 min: {}, i8 max: {}\n", INT8_MIN, (int)INT8_MAX);
  {
    // -10 / -7 = 1
    int8_t a = -10;
    int8_t b = -7;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 52 / -10 = -5
    int8_t a = 52;
    int8_t b = -10;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -127 / 13 = 10
    int8_t a = -127;
    int8_t b = 13;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
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
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 127 / -1 = -127
    int8_t a = -1;
    uint8_t b = 255;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
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
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 29 / 10 = 20
    uint8_t a = 29;
    uint8_t b = 10;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
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
    fmt::print("{} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // FLT_MAX-1.1 / 0.1 = ? overflow
    float a = FLT_MAX - 1.1;
    float b = 0.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateSDiv(p, q, getOpTmp(Op::Div));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Div), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
}

void mul() {
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
}

void sub() {
  // case: i8 - i8
  fmt::print("dump i8 - i8, i8 min: {}, i8 max: {}\n", INT8_MIN, (int)INT8_MAX);
  {
    // -7 - -101 = 94
    int8_t a = -7;
    int8_t b = -101;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -128 - 1 = ? overflow
    int8_t a = -128;
    int8_t b = 1;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 1 - -127 = ? overflow
    int8_t a = 1;
    int8_t b = -127;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, true));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: i8 - u8
  fmt::print("\ndump i8 - u8, i8 min: {}, i8 max: {}, u8 min: {}, u8 max: {}\n",
             INT8_MIN, (int)INT8_MAX, 0U, UINT8_MAX);
  {
    // 7 - 102 = -95
    int8_t a = 7;
    uint8_t b = 102;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -10 - 55 = -65
    int8_t a = -10;
    uint8_t b = 55;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // -1 - 128 = ? overflow
    int8_t a = -1;
    uint8_t b = 128;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, true));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
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
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // 1 - 2 = ? overflow
    uint8_t a = 1;
    uint8_t b = 2;
    llvm::Value *p = llvm::ConstantInt::get(context, llvm::APInt(8, a, false));
    llvm::Value *q = llvm::ConstantInt::get(context, llvm::APInt(8, b, false));
    llvm::Value *r = builder.CreateSub(p, q, getOpTmp(Op::Sub));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }

  // case: f32 - f32
  fmt::print("\ndump f32 - f32, f32 min: {}, f32 max: {}\n", FLT_MIN, FLT_MAX);
  {
    // 31.2 - 73.1 = -41.9
    float a = 31.2;
    float b = 73.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateFSub(p, q, getOpTmp(Op::Sub));
    fmt::print("{} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
  {
    // FLT_MAX-1.1 - -20.1 = ? overflow
    float a = FLT_MAX - 1.1;
    float b = -20.1;
    llvm::Value *p = llvm::ConstantFP::get(context, llvm::APFloat(a));
    llvm::Value *q = llvm::ConstantFP::get(context, llvm::APFloat(b));
    llvm::Value *r = builder.CreateFSub(p, q, getOpTmp(Op::Sub));
    fmt::print("overflow {} {} {} = ", a, getOpName(Op::Sub), b);
    r->print(llvm::outs(), true);
    fmt::print("\n");
  }
}

int main(void) {
  add();
  sub();
  mul();
  div();
  return 0;
}
