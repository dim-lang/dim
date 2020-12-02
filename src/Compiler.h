// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "infra/Cowstr.h"

class Compiler {
public:
  static void createObjectFile(const Cowstr &inputFile,
                               const Cowstr &outputFile = "", int optLevel = 0,
                               bool debugInfo = false,
                               const Cowstr &cpu = "generic",
                               const Cowstr &features = "");

  static void create_llvm_ll_file(const Cowstr &inputFile,
                                  const Cowstr &outputFile = "",
                                  bool enableFunctionPass = false);

  static void dumpAst(const Cowstr &inputFile);
};
