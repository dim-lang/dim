// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

namespace llvm {
class Module;
} // namespace llvm

class LLVMModular {
public:
  LLVMModular(llvm::Module *modules = nullptr);
  virtual ~LLVMModular() = default;
  virtual llvm::Module *&llvmModule();
  virtual llvm::Module *llvmModule() const;

protected:
  llvm::Module *llvmModular_;
};
