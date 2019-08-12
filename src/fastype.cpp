// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Config.h"
#include "Logging.h"
#include "Term.h"
#include "boost/program_options.hpp"
#include "model/File.h"
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;
using fastype::File;
using fastype::Logger;
using fastype::LogManager;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::vector;

int main(int argc, char **argv) {
  F_LOGGER("fastype");
  F_INFO("starting...");

  fastype::Config conf(argc, argv);

  string fileName = "";
  shared_ptr<fastype::Term> term = fastype::Term::open(fileName);
  term->show(fileName);
  return 0;
}
