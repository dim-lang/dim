// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Option.h"
#include "Configure.h"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"

namespace po = boost::program_options;

DimCompilerOption::DimCompilerOption(int argc, char **argv)
    : desc_("dim-compiler usage:\n"
            "dim-compiler [options] [input file]\n"),
      success_(true) {
  desc_.add_options()("help,h", "help message")("version,v",
                                                "version information")(
      "input-files,i", po::value<std::vector<std::string>>(), "input files")(
      "o", po::value<std::string>(),
      "output file name\n"
      "for input file 'source.dim', default object file name is 'source.o' if "
      "[file name] is not specified, assemble file name is 'source.s', LLVM ll "
      "file name is 'source.ll', LLVM binary code file name is 'source.bc'.")(
      "O", po::value<int>()->default_value(0),
      "optimization level, default level=0")("dump-ast",
                                             "dump abstract syntax tree")(
      "c", "create object file")("s", "create assemble file")(
      "ll", "create LLVM ll file")("bc", "create LLVM binary code file")(
      "g", "add debugging information in object file\n"
           "only works when -c is specified and optimization level=0.");

  pos_desc_.add("input-files", -1);

  try {
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc_)
            .positional(pos_desc_)
            .run(),
        vm_);
    boost::program_options::notify(vm_);
  } catch (const po::unknown_option &e) {
    message_ = e.what();
    success_ = false;
  }
}

bool DimCompilerOption::success() const { return success_; }

const std::string &DimCompilerOption::message() const { return message_; }

bool DimCompilerOption::has(const std::string &opt) const {
  return vm_.count(opt);
}
