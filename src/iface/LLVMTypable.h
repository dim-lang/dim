// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

namespace llvm {
class Type;
} // namespace llvm

class LLVMTypable {
public:
  LLVMTypable(llvm::Type *type = nullptr);
  virtual ~LLVMTypable() = default;
  virtual llvm::Type *&llvmType();
  virtual llvm::Type *llvmType() const;

protected:
  llvm::Type *llvmTypable_;
};
