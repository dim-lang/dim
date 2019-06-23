// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Global.h"
#include "Log.h"
#include "Term.h"
#include "boost/program_options.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;
using fastype::Logger;
using fastype::LogManager;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::vector;

int main(int argc, char **argv) {
  shared_ptr<Logger> log = LogManager::getLogger("main");

  boost_po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "version,v", fastype::Global::FastypeVersion)(
      "file-name,f", boost_po::value<vector<string>>(), "file name");

  boost_po::variables_map vm;
  boost_po::store(boost_po::parse_command_line(argc, argv, desc), vm);
  boost_po::notify(vm);
  if (vm.count("help")) {
    cout << desc << endl;
    return 0;
  }
  if (vm.count("version")) {
    cout << fastype::Global::FastypeVersion << endl;
    return 0;
  }

  vector<string> fileNameList;
  if (vm.count("file-name")) {
    for (int i = 0; i < vm["file-name"].as<vector<string>>().size(); i++) {
      cout << i << ": " << vm["file-name"].as<vector<string>>()[i] << endl;
    }
  }
  cout << endl;
  return 0;

  shared_ptr<fastype::Term> term = fastype::Term::open("Term");
  term->show("Term");

  return 0;
}
