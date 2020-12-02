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
      Compiler::dumpAst(opt.get<std::vector<std::string>>("input-files")[0]);
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

      if (codegenOpt == "obj") {
        ASSERT(opt.has("input-files"), "error: missing input file name\n");
        int optLevel = opt.get<int>("optimize");
        if (optLevel < 0 || optLevel > 3) {
          PRINT("warn: invalid optLevel {}, using optLevel=0\n");
          optLevel = 0;
        }
        bool debugInfo = opt.has("debug");
        (void)debugInfo;

        std::vector<std::string> inputFileList =
            opt.get<std::vector<std::string>>("input-files");

        // multiple input files
        if (inputFileList.size() > 1) {
          ASSERT_MSG(
              !opt.has("output"),
              "warn: output file {} cannot work for more than 2 input files\n",
              opt.get<std::string>("output"));
          for (int i = 0; i < (int)inputFileList.size(); ++i) {
            Compiler::createObjectFile(inputFileList[i], "", optLevel);
          }
        } else if (inputFileList.size() == 1) {
          // single input file
          Cowstr outputFile =
              opt.has("output") ? opt.get<std::string>("output") : "";
          Compiler::createObjectFile(inputFileList[0], outputFile, optLevel);
        }
      } // obj

      if (codegenOpt == "llvm-ll") {
        ASSERT(opt.has("input-files"), "error: missing input file name\n");
        int optLevel = opt.get<int>("optimize");
        if (optLevel < 0 || optLevel > 3) {
          PRINT("warn: invalid optLevel {}, using optLevel=0\n");
          optLevel = 0;
        }
        bool debugInfo = opt.has("debug");
        (void)debugInfo;

        std::vector<std::string> inputFileList =
            opt.get<std::vector<std::string>>("input-files");

        // multiple input files
        if (inputFileList.size() > 1) {
          ASSERT_MSG(
              !opt.has("output"),
              "warn: output file {} cannot work for more than 2 input files\n",
              opt.get<std::string>("output"));
          for (int i = 0; i < (int)inputFileList.size(); ++i) {
            Compiler::create_llvm_ll_file(inputFileList[i], "", optLevel > 0);
          }
        } else if (inputFileList.size() == 1) {
          // single input file
          Cowstr outputFile =
              opt.has("output") ? opt.get<std::string>("output") : "";
          Compiler::create_llvm_ll_file(inputFileList[0], outputFile,
                                        optLevel > 0);
        }
      } // llvm-ll
    }
  } catch (Exception &e) {
    PRINT("{}", e.message());
  } catch (std::exception &e) {
    PRINT("error: {}\n", e.what());
  }

  return 0;
}
