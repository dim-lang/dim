// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Option.h"
namespace po = boost::program_options;

Option::Option(int argc, char **argv)
    : desc_("dimc [options] [input files]"), success_(true) {

  desc_.add_options()
      // --help,-h
      ("help,h", "help message")

      // --version,-v
      ("version,v", "version information")

      // --input-files
      ("input-files",
       po::value<std::vector<std::string>>()->value_name("input files"),
       "input files")

      // --output,-o
      ("output,o", po::value<std::string>()->value_name("output file"),
       "output file\n"
       "if `output file` is not specified, output file name generated with "
       "input file concatinate with certain suffix:\n"
       "object file: '.o'\n"
       "assemble file: '.s'\n"
       "LLVM ll file: '.ll'\n"
       "LLVM bitcode file: '.bc'")

      // --codegen, -c
      ("codegen,c", po::value<std::string>()->value_name("type"),
       "specify the type of output files to generate\n"
       "asm: generate assemble *.s file\n"
       "llvm-ll: generate LLVM *.ll file\n"
       "llvm-bc: generate LLVM binary code *.bc file\n"
       "obj: generate object *.o file\n"
       "lib: generate dynamic library\n"
       "bin: generate native executable file")

      // --optimize, -O
      ("optimize,O", po::value<int>()->default_value(0)->value_name("level"),
       "optimization level [0-3], by default level is 0")

      // --debug, -g
      ("debug,g", "add debugging information in object file")

      // --dump, -d
      ("dump,d", po::value<std::string>()->value_name("type"),
       "dump compile information type\n"
       "ast: dump abstract syntax file");

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
