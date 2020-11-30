// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Option.h"
#include "Configure.h"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"

namespace po = boost::program_options;

Option::Option(int argc, char **argv)
    : desc_("dimc usage:\n"
            "dimc [options] [input file]"),
      success_(true) {

  desc_.add_options()
      // --help,-h
      ("help,h", "help message")

      // --version,-v
      ("version,v", "version information")

      // --input-files
      ("input-files", po::value<std::vector<std::string>>(), "input files")

      // --output,-o
      ("output,o", po::value<std::string>(),
       "output file name\n"
       "for input file 'source.dim', if [file name] is not specified, default "
       "object file name is 'source.o', assemble file name is 'source.s', LLVM "
       "ll file name is 'source.ll', LLVM binary code file name is "
       "'source.bc'.")

      // --codegen, -c
      ("codegen,c", po::value<std::string>(),
       "specify the types of output files to generate\n"
       "asm: generate assemble *.s file\n"
       "llvm-ll: generate LLVM *.ll file\n"
       "llvm-bc: generate LLVM binary code *.bc file\n"
       "obj: generate object *.o file\n"
       "lib: generate dynamic library\n"
       "bin: generate native executable file")

      // --optimize, -O
      ("optimize,O", po::value<int>()->default_value(0),
       "optimization level [0-4], by default level is 0\n"
       "only works when --codegen=obj.")

      // --debug, -g
      ("debug,g", "add debugging information in object file\n"
                  "only works when --codegen=obj.")

      // --dump, -d
      ("dump,d", po::value<std::string>(),
       "dump compile information\n"
       "ast: dump abstract syntax file\n");

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

bool Option::success() const { return success_; }

const std::string &Option::message() const { return message_; }

bool Option::has(const std::string &opt) const { return vm_.count(opt); }
