// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "Configure.h"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"

#define OPT_HELP "help"
#define OPT_VERSION "version"
#define OPT_INPUT_FILE "input-file"
#define OPT_DUMP_AST "dump-ast"
#define OPT_DUMP_LL "dump-ll"
#define OPT_DUMP_BC "dump-bc"
#define OPT_C "c"
#define OPT_O "o"

/**
 * dim-compiler usage:
 *  dim-compiler [options] [input files]
 *
 * options:
 *  --help, -h        help message
 *  --version, -v     version information
 *  -o [file name]    output file name
 *                    for input file 'source.dim', default object file
 *                    name is 'source.o' if [file name] is not specified,
 *                    assemble file name is 'source.s', LLVM ll file name
 *                    is 'source.ll', LLVM binary code file name is
 *                    'source.bc'.
 *  -O [level]        optimization level, by default level=0
 *  -dump-ast         dump abstract syntax tree
 *  -c                create object file
 *  -s                create assemble file
 *  -ll               create LLVM .ll file
 *  -bc               create LLVM .bc file
 *  -g                add debugging information in object file
 *                    only works when -c is specified and optimization level=0.
 */
class CompilerOption {
public:
  CompilerOption(int argc, char **argv);
  virtual ~CompilerOption() = default;

  bool success() const;
  const std::string &message() const;

  bool has(const std::string &opt) const;

  template <typename T> T get(const std::string &opt) const {
    return vm_[opt].as<T>();
  }

  template <> std::string get<std::string>(const std::string &opt) const {
    if (opt == "help") {
      std::stringstream ss;
      ss << desc_;
      return ss.str();
    }
    if (opt == "version") {
      return "dim-compiler-" PROJECT_VERSION;
    }
    return vm_[opt].as<std::string>();
  }

private:
  boost::program_options::options_description desc_;
  boost::program_options::positional_options_description pos_desc_;
  boost::program_options::variables_map vm_;

  // parsing result
  bool success_;
  std::string message_;
};

class LinkerOption {
public:
  LinkerOption(int argc, char **argv);
  virtual ~LinkerOption() = default;
};

class Option {
public:
  /**
   * dimc usage:
   *  dimc [options] [input files]
   *
   * options:
   *  --help, -h          help message
   *  --version, -v       version information
   *  --dump-<option>     dump with <option>:
   *                        ast: abstract syntax tree dot file
   *                        ll: LLVM ll file
   *                        bc: LLVM bitcode(bc) file
   *  --output-<option>   output with <option>:
   *                        obj: object file
   *                        exe: executable file
   *                        lib: library file
   */
  Option(int argc, char **argv)
      : desc_("dim usage:\n  dimc [options] [input files] -o output "
              "file\n\n") {
    desc_.add_options()(OPT_HELP ",h", "help message")(OPT_VERSION ",v",
                                                       "version information")(
        OPT_INPUT_FILE,
        boost::program_options::value<std::vector<std::string>>(),
        "input files")(OPT_DUMP_AST,
                       "dump abstract syntax tree in graphviz dot file")(
        OPT_DUMP_LL, "dump LLVM ll file")(OPT_DUMP_BC,
                                          "dump LLVM bitcode(bc) file");

    pos_desc_.add(OPT_INPUT_FILE, -1);

    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc_)
            .positional(pos_desc_)
            .run(),
        vm_);
    boost::program_options::notify(vm_);
  }
  virtual ~Option() = default;

  bool has(const std::string &opt) const { return vm_.count(opt); }

  template <typename T> T get(const std::string &opt) const {
    return vm_[opt].as<T>();
  }

  template <> std::string get<std::string>(const std::string &opt) const {
    if (opt == OPT_HELP) {
      std::stringstream ss;
      ss << desc_;
      return ss.str();
    }
    if (opt == OPT_VERSION) {
      return "dimc-" PROJECT_VERSION;
    }
    return vm_[opt].as<std::string>();
  }

private:
  CompilerOption compilerOption;
  LinkerOption linkerOption;
};
