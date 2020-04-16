// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include <string>
#include <unordered_map>

void i8Add(llvm::LLVMContext &c, llvm::IRBuilder<> &b) {
  // llvm::Value *l = llvm::ConstantInt::get(c, );
}

void u8Add() {}
void i16Add() {}
void u16Add() {}
void i32Add() {}
void u32Add() {}
void i64Add() {}
void u64Add() {}

int main(void) {
  llvm::LLVMContext c;
  llvm::IRBuilder<> b(c);
  llvm::Module *m;
  std::unordered_map<std::string, llvm::Value *> s;
  return 0;
}
