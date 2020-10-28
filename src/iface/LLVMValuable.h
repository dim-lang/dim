// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

namespace llvm {
class Value;
} // namespace llvm

class LLVMValuable {
public:
  LLVMValuable(llvm::Value *value = nullptr);
  virtual ~LLVMValuable() = default;
  virtual llvm::Value *&llvmValue();
  virtual llvm::Value *llvmValue() const;

protected:
  llvm::Value *llvmValuable_;
};
