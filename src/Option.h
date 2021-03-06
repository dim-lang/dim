// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "Configure.h"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"

/**
 * dimc usage:
 *  dimc [options] [input files]
 *
 * options:
 *  --help, -h                  help message
 *
 *  --version, -v               version information
 *
 *  --output, -o [output file]  output file name
 *                              if `output file` is not specified, output file
 *                              name generated with input file concatinate with
 *                              certain suffix:
 *                              object file: '.o'
 *                              assemble file: '.s'
 *                              LLVM ll file: '.ll'
 *                              LLVM bitcode file: '.bc'
 *
 *  --codegen, -c [type]      specify the `type` of output files to generate
 *                            asm: generate assemble file
 *                            llvm-ll: generate LLVM LL file
 *                            llvm-bc: generate LLVM binary code file
 *                            obj: generate object file
 *                            lib: generate dynamic library
 *                            bin: generate native executable file
 *
 *  --optimize, -O [level]    optimization `level` [0-3], by default level is 0
 *
 *  --debug, -g               add debugging information in object file
 *
 *  --dump, -d [type]         dump compile information `type`
 *                            ast: dump abstract syntax file
 *
 *  --input-files [input files]   input multiple files only when
 *                                --codegen=lib/bin
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
