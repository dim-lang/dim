// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Compiler.h"
#include "Configure.h"
#include "Dumper.h"
#include "Option.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"
#include "fmt/format.h"
#include "iface/Phase.h"
#include "infra/Log.h"
#include <string>
#include <vector>

static Dumper dumper;
static SymbolBuilder symbolBuilder;
static SymbolResolver symbolResolver;

static void dumpArgs(int argc, char **argv) {
  fmt::print(
      "Starting at {}, argc:{}, ",
      boost::filesystem::path(boost::filesystem::current_path()).string(),
      argc);
  for (int i = 0; i < argc; i++) {
    fmt::print("argv[{}]:{}", i, argv[i]);
    if (i < argc - 1) {
      fmt::print(", ");
    }
  }
  fmt::print("\n");
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
      Compiler dumper(opt.get<std::vector<std::string>>("input-files")[0],
                      CompileMode::DUMP_AST, false, 0, false);
      dumper.compile();
    }
  } catch (boost::program_options::unknown_option &unknown) {
    PRINT("error: {}\n", unknown.what());
  } catch (Exception &e) {
    PRINT("error: {}\n", e.message());
  }

  return 0;
}
