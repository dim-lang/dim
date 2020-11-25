// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/LLVMValuable.h"

LLVMValuable::LLVMValuable(llvm::Value *value) : llvmValuable_(value) {}

llvm::Value *&LLVMValuable::llvmValue() { return llvmValuable_; }

llvm::Value *LLVMValuable::llvmValue() const { return llvmValuable_; }

void LLVMValuable::resetLLVMValue() { llvmValuable_ = nullptr; }
