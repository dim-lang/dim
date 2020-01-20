// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Log.h"
#include "Option.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>
#include <vector>
namespace boost_po = boost::program_options;

int main(int argc, char **argv) {
  Log::initialize(".", "fastype");
  Option conf(argc, argv);
  FINFO("starting, argc:{} argv[0]:{}", argc, argv[0]);

  if (conf.hasHelp()) {
    std::cout << conf.help() << std::endl;
    return 0;
  }

  if (conf.hasVersion()) {
    std::cout << conf.version() << std::endl;
    return 0;
  }

  if (conf.hasInputFile()) {
    std::vector<std::string> inputFileList = conf.inputFileList();
    std::string firstFileName = inputFileList[0];
    icu::UnicodeString firstFileNameUnicode =
        icu::UnicodeString::fromUTF8(firstFileName);
  }

  return 0;
}
