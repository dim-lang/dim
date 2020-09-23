// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Graph.h"
#include "Log.h"
// #include "Option.h"
#include "Configure.h"
#include "Scanner.h"
#include "Symbol.h"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"
#include "fmt/format.h"
#include "parser.tab.hh"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define OPT_HELP "help"
#define OPT_VERSION "version"
#define OPT_INPUT_FILE "input-file"
#define OPT_DUMP_AST "dump-ast"
#define OPT_DUMP_SYMBOL "dump-symbol"
#define OPT_DUMP_LLVM "dump-llvm"

class Option {
public:
  // --help, -h
  // --version, -v
  // --input-file
  // --dump-ast
  // --dump-symbol
  // --dump-llvm
  Option(int argc, char **argv) : opt_desc("nerdc options") {
    opt_desc.add_options()(OPT_HELP ",h", "help message")(
        OPT_VERSION ",v", "version information")(
        OPT_INPUT_FILE,
        boost::program_options::value<std::vector<std::string>>(),
        "input files")(OPT_DUMP_AST, "dump ast in graphviz dot file")(
        OPT_DUMP_SYMBOL, "dump symbol in graphviz dot file")(
        OPT_DUMP_LLVM, "dump llvm in ll file");

    positional_opt_desc.add(OPT_INPUT_FILE, -1);

    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(opt_desc)
            .positional(positional_opt_desc)
            .run(),
        var_map);
    boost::program_options::notify(var_map);
  }
  virtual ~Option() = default;

  bool has(const std::string &opt) const { return var_map.count(opt); }

  template <typename T> T get(const std::string &opt) const {
    return var_map[opt].as<T>();
  }

  template <> std::string get<std::string>(const std::string &opt) const {
    if (opt == OPT_HELP) {
      std::stringstream ss;
      ss << opt_desc;
      return ss.str();
    }
    if (opt == OPT_VERSION) {
      return "nerdc-" PROJECT_VERSION;
    }
    return var_map[opt].as<std::string>();
  }

private:
  boost::program_options::options_description opt_desc;
  boost::program_options::positional_options_description positional_opt_desc;
  boost::program_options::variables_map var_map;
};

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
    fmt::print("Dumping ast in {}", fileName + ".ast.dot\n");
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
    fmt::print("Dumping symbol in {}", fileName + ".symbol.dot\n");
  }
}

int main(int argc, char **argv) {
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

  try {
    Option opt(argc, argv);

    if (opt.has(OPT_HELP)) {
      fmt::print("{}\n", opt.get<std::string>(OPT_HELP));
      return 0;
    }

    if (opt.has(OPT_VERSION)) {
      fmt::print("{}\n", opt.get<std::string>(OPT_VERSION));
      return 0;
    }

    if (opt.has(OPT_DUMP_AST)) {
      if (!opt.has(OPT_INPUT_FILE)) {
        fmt::print("Error! missing file names\n");
        return 0;
      }
      dumpAst(opt.get<std::vector<std::string>>(OPT_INPUT_FILE));
    }

    if (opt.has(OPT_DUMP_SYMBOL)) {
      if (!opt.has(OPT_INPUT_FILE)) {
        fmt::print("Error! missing file names\n");
        return 0;
      }
      dumpSymbol(opt.get<std::vector<std::string>>(OPT_INPUT_FILE));
    }

  } catch (const boost::program_options::unknown_option &unknown) {
    fmt::print("Error! {}\n", unknown.what());
  }

  return 0;
}
