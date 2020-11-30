// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "enum.h"
#include "infra/Cowstr.h"

BETTER_ENUM(CompileMode, int,
            // compile source files to .o object files, but do not link.
            // equal to `clang -c`
            OBJ = 20000,
            // compile source files to .s assemble code files.
            // equal to `clang -S`
            ASM,
            // compile source files to LLVM IR language .ll files.
            LLVM_LL,
            // compile source files to LLVM binary code .bc files.
            LLVM_BC,
            // dump abstract syntax tree
            DUMP_AST)

class Compiler {
public:
  Compiler(const Cowstr &source, CompileMode mode, bool optimizeLLFunction,
           int optimizationLevel, bool debugInfo, const Cowstr &output = "");
  virtual ~Compiler() = default;
  virtual void compile();

private:
  // source file name
  Cowstr source_;

  // compile options {

  // compile mode
  CompileMode mode_;

  // use llvm::legacy::FunctionPass to optimize LLVM functions
  bool optimizeLLFunction_;

  // optimization level in [0,3], only work when mode=OBJ
  // equal to `clang -O` optimization levels option
  int optimizationLevel_;

  // add debugging information
  // equal to `clang -g`
  bool debugInfo_;

  // output file name, not work for mode=AST
  // if not specified, default OBJ file name for source.dim is source.o, ASM
  // file name is source.s, LL file name is source.ll, BC file name is source.bc
  Cowstr output_;

  // compile options }

  virtual void createObjectFile();
  virtual void createAssembleFile();
  virtual void createLLVM_LL();
  virtual void createLLVM_BinaryCode();
  virtual void dumpAbstractSyntaxTree();
};
