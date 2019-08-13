// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Config.h"
#include "Logging.h"
#include "Term.h"
#include "model/File.h"
#include <cstdio>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;
using std::shared_ptr;
using std::string;
using std::vector;

int main(int argc, char **argv) {
  F_LOGGER("fastype");
  F_INFO("starting...");

  fastype::Config conf(argc, argv);

  if (conf.hasHelp()) {
    std::printf("%s", conf.help().data());
    return 0;
  }

  if (conf.hasVersion()) {
    std::printf("%s", conf.version().data());
    return 0;
  }

  if (conf.hasInputFile()) {
    vector<string> inputFileList = conf.inputFileList();
    shared_ptr<fastype::Term> term = fastype::Term::open(inputFileList[0]);
    term->show(inputFileList[0]);
  }

  return 0;
}
