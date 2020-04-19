// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "fmt/format.h"
#include "llvm_IRBuilder_Util.h"

int main(void) {
  llvm::LLVMContext c;
  llvm::IRBuilder<> b(c);

  // case: i8 + i8
  fmt::print("\ndump i8 + i8, i8 min: {}, i8 max: {}\n", INT8_MIN, INT8_MAX);
  dumpInt((int8_t)1, (int8_t)2, 8, c, b, ADD, CreateAdd);      // p+p
  dumpInt((int8_t)-7, (int8_t)-101, 8, c, b, ADD, CreateAdd);  // n+n
  dumpInt((int8_t)-12, (int8_t)127, 8, c, b, ADD, CreateAdd);  // n+p
  dumpInt((int8_t)127, (int8_t)-128, 8, c, b, ADD, CreateAdd); // p+n
  dumpInt((int8_t)127, (int8_t)1, 8, c, b, ADD,
          CreateAdd); // p+p overflow
  dumpInt((int8_t)-77, (int8_t)-100, 8, c, b, ADD,
          CreateAdd); // n+n overflow

  // case: i8 + u8
  std::printf(
      "\ndump i8 + u8, i8 min: %d, i8 max: %d, u8 min: %d, u8 max: %d\n",
      INT8_MIN, INT8_MAX, 0U, UINT8_MAX);
  dumpInt((int8_t)7, (uint8_t)245, 8, c, b, ADD, CreateAdd); // p+p
  dumpInt((int8_t)-128, (uint8_t)255, 8, c, b, ADD,
          CreateAdd); // n+p
  dumpInt((int8_t)88, (uint8_t)250, 8, c, b, ADD,
          CreateAdd); // p+p overflow
  dumpInt((int8_t)-77, (uint8_t)-100, 8, c, b, ADD,
          CreateAdd); // n+n overflow

  // case: i8 - i8
  std::printf("\ndump i8 - i8, min: %d, max: %d\n", INT8_MIN, INT8_MAX);
  dumpInt((int8_t)1, (int8_t)2, c, b, SUB, CreateSub);     // p-p
  dumpInt((int8_t)-7, (int8_t)-101, c, b, SUB, CreateSub); // n-n
  dumpInt((int8_t)27, (int8_t)-28, c, b, SUB, CreateSub);  // p-n
  dumpInt((int8_t)12, (int8_t)-107, c, b, SUB, CreateSub); // p-n
  dumpInt((int8_t)-127, (int8_t)1, c, b, SUB, CreateSub);  // n-p
  dumpInt((int8_t)-1, (int8_t)126, c, b, SUB, CreateSub);  // n-p
  dumpInt((int8_t)88, (int8_t)-79, c, b, SUB,
          CreateSub); // p-n overflow
  dumpInt((int8_t)127, (int8_t)-1, c, b, SUB,
          CreateSub); // p-n overflow
  dumpInt((int8_t)-77, (int8_t)100, c, b, SUB,
          CreateSub); // n-p overflow
  dumpInt((int8_t)-128, (int8_t)1, c, b, SUB,
          CreateSub); // n-p overflow

  // case: i8 * i8
  std::printf("\ndump i8 * i8, min: %d, max: %d\n", INT8_MIN, INT8_MAX);
  dumpInt((int8_t)1, (int8_t)2, c, b, MUL, CreateMul);    // p*p
  dumpInt((int8_t)-12, (int8_t)-8, c, b, MUL, CreateMul); // n*n
  dumpInt((int8_t)-7, (int8_t)-11, c, b, MUL,
          CreateMul); // n*n
  dumpInt((int8_t)7, (int8_t)-13, c, b, MUL,
          CreateMul);                                     // p*n
  dumpInt((int8_t)2, (int8_t)-17, c, b, MUL, CreateMul);  // p*n
  dumpInt((int8_t)-17, (int8_t)1, c, b, MUL, CreateMul);  // n*p
  dumpInt((int8_t)-1, (int8_t)126, c, b, MUL, CreateMul); // n*p
  dumpInt((int8_t)88, (int8_t)-79, c, b, MUL,
          CreateMul); // p*n overflow
  dumpInt((int8_t)127, (int8_t)-12, c, b, MUL,
          CreateMul); // p*n overflow
  dumpInt((int8_t)-77, (int8_t)100, c, b, MUL,
          CreateMul); // n*p overflow
  dumpInt((int8_t)-128, (int8_t)13, c, b, MUL,
          CreateMul); // n*p overflow

  // case: i8 / i8
  std::printf("\ndump i8 / i8, min: %d, max: %d\n", INT8_MIN, INT8_MAX);
  dumpInt((int8_t)1, (int8_t)2, c, b, DIV, CreateSDiv);    // p*p
  dumpInt((int8_t)-12, (int8_t)-8, c, b, DIV, CreateSDiv); // n*n
  dumpInt((int8_t)-7, (int8_t)-11, c, b, DIV,
          CreateSDiv); // n*n
  dumpInt((int8_t)7, (int8_t)-13, c, b, DIV,
          CreateSDiv);                                     // p*n
  dumpInt((int8_t)2, (int8_t)-17, c, b, DIV, CreateSDiv);  // p*n
  dumpInt((int8_t)-17, (int8_t)1, c, b, DIV, CreateSDiv);  // n*p
  dumpInt((int8_t)-1, (int8_t)126, c, b, DIV, CreateSDiv); // n*p
  dumpInt((int8_t)88, (int8_t)-79, c, b, DIV,
          CreateSDiv); // p*n overflow
  dumpInt((int8_t)127, (int8_t)-12, c, b, DIV,
          CreateSDiv); // p*n overflow
  dumpInt((int8_t)-77, (int8_t)100, c, b, DIV,
          CreateSDiv); // n*p overflow
  dumpInt((int8_t)-128, (int8_t)13, c, b, DIV,
          CreateSDiv); // n*p overflow

  // case: f32 op f32
  std::printf("\ndump f32 op f32, f32 min: %d, f32 max: %d\n", FLT_MIN,
              FLT_MAX);
  dumpFloat((float)0.01, (float)0.02, c, b, ADD, CreateFAdd);
  dumpFloat((float)10281.0, (float)-1820.02, c, b, ADD, CreateFAdd);
  dumpFloat((float)0.012, (float)2.01, c, b, SUB, CreateFSub);
  dumpFloat((float)1820.012, (float)-2082.01, c, b, SUB, CreateFSub);
  dumpFloat((float)0.01201, (float)2.0101, c, b, MUL, CreateFMul);
  dumpFloat((float)9820.201, (float)282.01, c, b, MUL, CreateFMul);
  dumpFloat((float)1.001, (float)0.1, c, b, DIV, CreateFDiv);
  dumpFloat((float)182.001, (float)9820.1, c, b, DIV, CreateFDiv);

  // case: f32 op f64
  std::printf("\ndump f32 op f64, f32 min: %d, f32 max: %d\n", FLT_MIN,
              FLT_MAX);

  // case: f64 op f64
  std::printf("\ndump f32 op f64, f32 min: %d, f32 max: %d\n", FLT_MIN,
              FLT_MAX);

  return 0;
}
