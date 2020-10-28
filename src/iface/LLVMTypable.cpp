// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/LLVMTypable.h"

LLVMTypable::LLVMTypable(llvm::Type *type) : llvmTypable_(type) {}

llvm::Type *&LLVMTypable::llvmType() { return llvmTypable_; }

llvm::Type *LLVMTypable::llvmType() const { return llvmTypable_; }
