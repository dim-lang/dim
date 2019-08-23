// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "Option.h"
#include "Term.h"
#include "model/Cache.h"
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;
using std::shared_ptr;
using std::string;
using std::vector;

int main(int argc, char **argv) {
  F_LOG_INIT(".", "fastype");
  F_INFOF("starting, argc:{} argv[0]:{}", argc, argv[0]);

  fastype::Option conf(argc, argv);

  if (conf.hasHelp()) {
    std::cout << conf.help() << std::endl;
    return 0;
  }

  if (conf.hasVersion()) {
    std::cout << conf.version() << std::endl;
    return 0;
  }

  if (conf.hasInputFile()) {
    vector<string> inputFileList = conf.inputFileList();
    shared_ptr<fastype::Term> term = fastype::Term::open(inputFileList[0]);
    term->show(inputFileList[0]);
  }

  return 0;
}
