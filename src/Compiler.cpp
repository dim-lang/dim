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
#include "llvm/Config/llvm-config.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <string>
#include <system_error>

void Compiler::createObjectFile(const Cowstr &inputFile,
                                const Cowstr &outputFile, int optLevel,
                                bool debugInfo, const Cowstr &cpu,
                                const Cowstr &features) {
  Cowstr dest = outputFile.empty() ? (inputFile + ".o") : outputFile;

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetDisassembler();

  std::string targetTriple = llvm::sys::getDefaultTargetTriple();
  std::string lookupTargetError;
  const llvm::Target *target =
      llvm::TargetRegistry::lookupTarget(targetTriple, lookupTargetError);
  ASSERT(target, "error: {}", lookupTargetError);

  llvm::TargetOptions targetOptions;
  llvm::Optional<llvm::Reloc::Model> relocModel =
      llvm::Optional<llvm::Reloc::Model>();
  llvm::TargetMachine *targetMachine = target->createTargetMachine(
      targetTriple, cpu.str(), features.str(), targetOptions, relocModel);

  Scanner scanner(inputFile);
  ASSERT(scanner.parse() == 0, "error: syntax error in {}\n", inputFile);

  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  IrBuilder irBuilder(optLevel > 0);

  PhaseManager pm({&symbolBuilder, &symbolResolver, &irBuilder});
  pm.run(scanner.compileUnit());

  irBuilder.llvmModule()->setDataLayout(targetMachine->createDataLayout());
  irBuilder.llvmModule()->setTargetTriple(targetTriple);

  std::error_code dest_errcode;
  llvm::raw_fd_ostream dest_os(dest.str(), dest_errcode,
                               llvm::sys::fs::OF_None);
  ASSERT(!dest_errcode, "error: cannot create file for {}: {}", dest,
         dest_errcode.message());

  llvm::legacy::PassManager passManager;
#if (LLVM_VERSION_MAJOR > 9)
  llvm::CodeGenFileType objFileType = llvm::CGFT_ObjectFile;
#else
  llvm::TargetMachine::CodeGenFileType objFileType =
      llvm::TargetMachine::CGFT_ObjectFile;
#endif
  ASSERT(!targetMachine->addPassesToEmitFile(passManager, dest_os, nullptr,
                                             objFileType),
         "error: LLVM target machine cannot emit object file");

  passManager.run(*irBuilder.llvmModule());
  dest_os.flush();
  dest_os.close();
}

void Compiler::create_llvm_ll_file(const Cowstr &inputFile,
                                   const Cowstr &outputFile,
                                   bool enableFunctionPass) {
  Cowstr dest = outputFile.empty() ? (inputFile + ".ll") : outputFile;

  Scanner scanner(inputFile);
  ASSERT(scanner.parse() == 0, "error: syntax error in {}\n", inputFile);

  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  IrBuilder irBuilder(enableFunctionPass);

  PhaseManager pm({&symbolBuilder, &symbolResolver, &irBuilder});
  pm.run(scanner.compileUnit());

  FileWriter fwriter(dest);
  fwriter.write(Cowstr::from(irBuilder.llvmModule()));
}

void Compiler::dumpAst(const Cowstr &inputFile) {
  Scanner scanner(inputFile);
  ASSERT(scanner.parse() == 0, "error: syntax error in {}\n", inputFile);

  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  Dumper dumper;
  PhaseManager pm({&symbolBuilder, &symbolResolver, &dumper});
  pm.run(scanner.compileUnit());

  for (int i = 0; i < (int)dumper.dump().size(); ++i) {
    PRINT("{}\n", dumper.dump()[i]);
  }
}
