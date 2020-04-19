// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "fmt/format.h"
#include "llvm_IRBuilder_Util.h"

int main(void) {
  llvm::LLVMContext c;
  llvm::IRBuilder<> b(c);

  // case: i8 + i8
  fmt::print("dump i8 + i8, i8 min: {}, i8 max: {}\n", INT8_MIN, INT8_MAX);
  dumpInt(true, (int8_t)-7, true, (int8_t)-101, 8, c, b, Op::Add,
          CreateAdd); // n+n
  dumpInt(true, (int8_t)127, true, (int8_t)-128, 8, c, b, Op::Add,
          CreateAdd); // p+n
  dumpInt(true, (int8_t)127, true, (int8_t)1, 8, c, b, Op::Add,
          CreateAdd); // p+p overflow
  dumpInt(true, (int8_t)-77, true, (int8_t)-100, 8, c, b, Op::Add,
          CreateAdd); // n+n overflow

  // case: i8 + u8
  fmt::print("\ndump i8 + u8, i8 min: {}, i8 max: {}, u8 min: {}, u8 max: {}\n",
             INT8_MIN, INT8_MAX, 0U, UINT8_MAX);
  dumpInt(true, (int8_t)7, false, (uint8_t)245, 8, c, b, Op::Add,
          CreateAdd); // p+p
  dumpInt(true, (int8_t)-128, false, (uint8_t)255, 8, c, b, Op::Add,
          CreateAdd); // n+p
  dumpInt(true, (int8_t)88, false, (uint8_t)250, 8, c, b, Op::Add,
          CreateAdd); // p+p overflow
  dumpInt(true, (int8_t)-77, false, (uint8_t)100, 8, c, b, Op::Add,
          CreateAdd); // n+n overflow

  // case: u8 + u8
  fmt::print("\ndump u8 + u8, u8 min: {}, u8 max: {}\n", 0U, UINT8_MAX);
  dumpInt(false, (uint8_t)7, false, (uint8_t)245, 8, c, b, Op::Add,
          CreateAdd); // p+p
  dumpInt(false, (uint8_t)112, false, (uint8_t)55, 8, c, b, Op::Add,
          CreateAdd); // n+p
  dumpInt(false, (uint8_t)88, false, (uint8_t)250, 8, c, b, Op::Add,
          CreateAdd); // overflow
  dumpInt(false, (uint8_t)177, false, (uint8_t)100, 8, c, b, Op::Add,
          CreateAdd); // overflow

  return 0;
}
