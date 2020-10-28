// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/LLVMModular.h"

LLVMModular::LLVMModular(llvm::Module *modules) : llvmModular_(modules) {}

llvm::Module *&LLVMModular::llvmModule() { return llvmModular_; }

llvm::Module *LLVMModular::llvmModule() const { return llvmModular_; }
