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
 * dimc usage:
 *  dimc [options] [input files]
 *
 * options:
 *  --help, -h                help message
 *
 *  --version, -v             version information
 *
 *  --output, -o [file name]  output file name
 *                            for input file 'source.dim', if [file name] is not
 *                            specified, default generate object file name is
 *                            'source.o', assemble file name is 'source.s', LLVM
 *                            ll file name is 'source.ll', LLVM binary code file
 *                            name is 'source.bc'.
 *
 *  --codegen, -c             specify the types of output files to generate
 *                            asm: generate assemble file
 *                            llvm-ll: generate LLVM LL file
 *                            llvm-bc: generate LLVM binary code file
 *                            obj: generate object file
 *                            lib: generate dynamic library
 *                            bin: generate native executable file
 *
 *  --optimize, -O [level]    optimization level [0-4], by default level is 0
 *                            only works when --emit=obj.
 *
 *  -g                        add debugging information in object file
 *                            only works when --emit=obj.
 *
 *  --dump, -d                dump compile information
 *                            ast: dump abstract syntax file
 *
 *  --input-files [input files]   input multiple files only when --emit=lib/bin
 */
class Option {
public:
  Option(int argc, char **argv);
  virtual ~Option() = default;

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
      return "dimc-" PROJECT_VERSION;
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
