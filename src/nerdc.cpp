// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Graph.h"
#include "Log.h"
#include "Option.h"
#include "Scanner.h"
#include "Symbol.h"
#include "boost/filesystem.hpp"
#include "fmt/format.h"
#include "parser.tab.hh"
#include <iostream>
#include <string>
#include <vector>

static void dumpAst(const std::vector<std::string> &fileNameList) {
  for (int i = 0; i < (int)fileNameList.size(); i++) {
    std::string fileName = fileNameList[i];
    Scanner scanner(fileName);
    if (scanner.parse() != 0) {
      fmt::print("Error! syntax error in {}!", fileName);
      return;
    }
    AstGraph g(scanner.compileUnit());
    g.draw(fileName + ".ast.dot");
    fmt::print("Dump ast in {}", fileName + ".ast.dot\n");
  }
}

static void dumpSymbol(const std::vector<std::string> &fileNameList) {
  for (int i = 0; i < (int)fileNameList.size(); i++) {
    std::string fileName = fileNameList[i];
    Scanner scanner(fileName);
    if (scanner.parse() != 0) {
      fmt::print("Error! syntax error in {}!", fileName);
      return;
    }
    SymbolGraph g(Scope::from(scanner.compileUnit()));
    g.draw(fileName + ".symbol.dot");
    fmt::print("Dump symbol in {}", fileName + ".symbol.dot\n");
  }
}

int main(int argc, char **argv) {
  fmt::print(
      "Start at {}, argc:{}, ",
      boost::filesystem::path(boost::filesystem::current_path()).string(),
      argc);
  for (int i = 0; i < argc; i++) {
    fmt::print("argv[{}]:{}", i, argv[i]);
    if (i < argc - 1) {
      fmt::print(", ");
    }
  }
  fmt::print("\n");

  try {
    Option conf(argc, argv);

    if (conf.hasHelp()) {
      fmt::print("{}\n", conf.help());
      return 0;
    }

    if (conf.hasVersion()) {
      fmt::print("{}\n", conf.version());
      return 0;
    }

    if (conf.hasDumpAst()) {
      if (!conf.hasFileNames()) {
        fmt::print("Error! missing file names\n");
        return 0;
      }
      dumpAst(conf.fileNames());
      return 0;
    }

    if (conf.hasDumpSymbol()) {
      if (!conf.hasFileNames()) {
        fmt::print("Error! missing file names\n");
        return 0;
      }
      dumpSymbol(conf.fileNames());
      return 0;
    }

    fmt::print("Error! missing parameter!\n");

  } catch (const std::vector<std::string> unregistered) {
    fmt::print("Error! unrecognized parameters: ");
    for (int i = 0; i < (int)unregistered.size(); i++) {
      fmt::print("{}", unregistered[i]);
      if (i < (int)unregistered.size() - 1) {
        fmt::print(", ");
      }
    }
    fmt::print("\n");
  }

  return 0;
}
