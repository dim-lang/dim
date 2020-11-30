// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Compiler.h"
#include "Dumper.h"
#include "IrBuilder.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "iface/Phase.h"
#include "infra/Files.h"
#include "infra/Log.h"

static Cowstr sourcePrefix(const Cowstr source) {
  ASSERT(source.endWith(".dim"),
         "error: source file {} does not end with .dim\n", source);
  return source.subString(0, source.length() - 4);
}

Compiler::Compiler(const Cowstr &source, CompileMode mode,
                   bool optimizeLLFunction, int optimizationLevel,
                   bool debugInfo, const Cowstr &output)
    : source_(source), mode_(mode), optimizeLLFunction_(optimizeLLFunction),
      optimizationLevel_(optimizationLevel), debugInfo_(debugInfo),
      output_(output) {}

void Compiler::compile() {
  switch (mode_) {
  case CompileMode::OBJ:
    if (output_.empty()) {
      output_ = sourcePrefix(source_) + ".o";
    }
    createObjectFile();
    break;
  case CompileMode::ASM:
    if (output_.empty()) {
      output_ = sourcePrefix(source_) + ".s";
    }
    createAssembleFile();
    break;
  case CompileMode::LLVM_LL:
    if (output_.empty()) {
      output_ = sourcePrefix(source_) + ".ll";
    }
    createLLVM_LL();
    break;
  case CompileMode::LLVM_BC:
    if (output_.empty()) {
      output_ = sourcePrefix(source_) + ".bc";
    }
    createLLVM_BinaryCode();
    break;
  case CompileMode::DUMP_AST:
    dumpAbstractSyntaxTree();
    break;
  default:
    ERROR("error: unknown compile mode:{}\n", mode_._to_string());
  }
}

void Compiler::createObjectFile() {
  ERROR("error: create object file .o not implemented\n");
}

void Compiler::createAssembleFile() {
  ERROR("error: create assemble file .s not implemented\n");
}

void Compiler::createLLVM_LL() {
  Scanner scanner(source_);
  ASSERT(scanner.parse() == 0, "error: syntax error in {}\n", source_);

  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  IrBuilder irBuilder(optimizeLLFunction_);

  PhaseManager pm({&symbolBuilder, &symbolResolver, &irBuilder});
  pm.run(scanner.compileUnit());

  FileWriter fwriter(output_);
  fwriter.write(Cowstr::from(irBuilder.llvmModule()));
}

void Compiler::createLLVM_BinaryCode() {
  ERROR("error: create LLVM binary code file .bc not implemented\n");
}

void Compiler::dumpAbstractSyntaxTree() {
  Scanner scanner(source_);
  ASSERT(scanner.parse() == 0, "error: syntax error in {}\n", source_);

  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  Dumper dumper;

  PhaseManager pm({&symbolBuilder, &symbolResolver, &dumper});
  pm.run(scanner.compileUnit());

  for (int i = 0; i < (int)dumper.dump().size(); ++i) {
    PRINT("{}", dumper.dump()[i]);
  }
}
