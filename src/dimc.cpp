// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Compiler.h"
#include "Option.h"
#include "boost/filesystem.hpp"
#include "boost/program_options/parsers.hpp"
#include "fmt/format.h"
#include "infra/Log.h"
#include <string>
#include <vector>

static void dumpArgs(int argc, char **argv) {
  LOG_INFO("working at {}, argc:{}",
           boost::filesystem::path(boost::filesystem::current_path()).string(),
           argc);
  for (int i = 0; i < argc; i++) {
    LOG_INFO("argv[{}]:{}", i, argv[i]);
  }
}

int main(int argc, char **argv) {
#ifndef NDEBUG
  dumpArgs(argc, argv);
#endif

  try {
    Option opt(argc, argv);
    if (opt.has("help")) {
      fmt::print("{}\n", opt.get<std::string>("help"));
      return 0;
    }
    if (opt.has("version")) {
      fmt::print("{}\n", opt.get<std::string>("version"));
      return 0;
    }
    if (opt.has("dump")) {
      ASSERT(opt.get<std::string>("dump") == "ast",
             "error: unknown dump type {}\n", opt.get<std::string>("dump"));
      ASSERT(opt.has("input-files"), "error: missing input file names\n");
      ASSERT(opt.get<std::vector<std::string>>("input-files").size() == 1,
             "error: input one file at a time\n");
      Compiler dump_compiler(
          opt.get<std::vector<std::string>>("input-files")[0],
          CompileMode::DUMP_AST, 0, false);
      dump_compiler.compile();
    }
    if (opt.has("codegen")) {
      std::string codegenOpt = opt.get<std::string>("codegen");
      ASSERT(codegenOpt == "asm" || codegenOpt == "llvm-ll" ||
                 codegenOpt == "llvm-bc" || codegenOpt == "obj" ||
                 codegenOpt == "lib" || codegenOpt == "bin",
             "error: unknown codegen type {}\n", codegenOpt);
      ASSERT(codegenOpt != "lib", "error: --codegen=lib not implemented\n");
      ASSERT(codegenOpt != "bin", "error: --codegen=bin not implemented\n");
      ASSERT(codegenOpt != "llvm-bc",
             "error: --codegen=llvm-bc not implemented\n");
      ASSERT(codegenOpt != "asm", "error: --codegen=asm not implemented\n");
      ASSERT(codegenOpt != "obj", "error: --codegen=obj not implemented\n");

      if (codegenOpt == "llvm-ll") {
        ASSERT(opt.has("input-files"), "error: missing input file name\n");
        int optimizationLevel = opt.get<int>("optimize");
        ASSERT(optimizationLevel >= 0 && optimizationLevel <= 3,
               "error: invalid optimization level --optimize={}",
               optimizationLevel);
        bool debugInfo = opt.has("debug");

        std::vector<std::string> inputFileList =
            opt.get<std::vector<std::string>>("input-files");

        // multiple input files
        if (inputFileList.size() > 1) {
          ASSERT(!opt.has("output"),
                 "error: output file name {} cannot work "
                 "for more than 2 input files\n",
                 opt.get<std::string>("output"));
          for (int i = 0; i < (int)inputFileList.size(); ++i) {
            Compiler ll_compiler(inputFileList[i], CompileMode::LLVM_LL,
                                 optimizationLevel, debugInfo);
            ll_compiler.compile();
          }
        } else if (inputFileList.size() == 1) {
          // single input file
          Cowstr outputFile = "";
          if (opt.has("output")) {
            outputFile = opt.get<std::string>("output");
          }
          Compiler ll_compiler(inputFileList[0], CompileMode::LLVM_LL,
                               optimizationLevel, debugInfo, outputFile);
          ll_compiler.compile();
        }
      } // llvm-ll
    }
  } catch (boost::program_options::unknown_option &unknown) {
    PRINT("error: {}\n", unknown.what());
  } catch (Exception &e) {
    PRINT("{}", e.message());
  }

  return 0;
}
