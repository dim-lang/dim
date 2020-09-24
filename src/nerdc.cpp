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
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define OPT_HELP "help"
#define OPT_VERSION "version"
#define OPT_INPUT_FILE "input-file"
#define OPT_DUMP_AST "dump-ast"
#define OPT_DUMP_SYMBOL "dump-symbol"
#define OPT_DUMP_LL "dump-ll"
#define OPT_DUMP_BC "dump-bc"
#define OPT_C "c"
#define OPT_O "o"

class Option {
public:
  /**
   * nerdc usage:
   *  nerdc [options] [input files]
   *
   * options:
   *  --help, -h          help message
   *  --version, -v       version information
   *  --dump-<option>     dump with <option>:
   *                        ast: abstract syntax tree dot file
   *                        symbol: symbol table dot file
   *                        ll: LLVM ll file
   *                        bc: LLVM bitcode(bc) file
   *  --output-<option>   output with <option>:
   *                        obj: object file
   *                        exe: executable file
   *                        lib: library file
   */
  Option(int argc, char **argv)
      : desc_flags("nerdc usage:\n  nerdc [options] [input files] -o output "
                   "file\n\n") {
    desc_flags.add_options()(OPT_HELP ",h", "help message")(
        OPT_VERSION ",v", "version information")(
        OPT_INPUT_FILE,
        boost::program_options::value<std::vector<std::string>>(),
        "input files")(OPT_DUMP_AST,
                       "dump abstract syntax tree in graphviz dot file")(
        OPT_DUMP_SYMBOL, "dump symbol table in graphviz dot file")(
        OPT_DUMP_LL, "dump LLVM ll file")(OPT_DUMP_BC,
                                          "dump LLVM bitcode(bc) file");

    positional_desc_flags.add(OPT_INPUT_FILE, -1);

    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc_flags)
            .positional(positional_desc_flags)
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
      ss << desc_flags;
      return ss.str();
    }
    if (opt == OPT_VERSION) {
      return "nerdc-" PROJECT_VERSION;
    }
    return var_map[opt].as<std::string>();
  }

private:
  boost::program_options::options_description desc_flags;
  boost::program_options::options_description desc_options;
  boost::program_options::positional_options_description positional_desc_flags;
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
    Cowstr dumpfile = fmt::format("{}.ast.dot", fileName);
    g.draw(dumpfile);
    fmt::print("Dumping ast in {}\n", dumpfile);
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
    Cowstr dumpfile = fmt::format("{}.symbol.dot", fileName);
    g.draw(dumpfile);
    fmt::print("Dumping symbol in {}\n", dumpfile);
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

  } catch (boost::program_options::unknown_option &unknown) {
    fmt::print("Error! {}\n", unknown.what());
  } catch (std::exception &e) {
    fmt::print("Unknown Error! {}\n", e.what());
  }

  return 0;
}
