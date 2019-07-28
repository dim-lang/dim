// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Global.h"
#include "Logging.h"
#include "Term.h"
#include "boost/program_options.hpp"
#include "unicode/unistr.h"
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
  shared_ptr<Logger> logging_ = LogManager::getLogger("fastype");

  boost_po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "version,v", fastype::Global::FastypeVersion)(
      "file-name,f", boost_po::value<vector<string>>(), "file name");
  boost_po::positional_options_description posDesc;
  posDesc.add("file-name", -1);

  boost_po::variables_map vm;
  boost_po::store(boost_po::command_line_parser(argc, argv)
                      .options(desc)
                      .positional(posDesc)
                      .run(),
                  vm);
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
  if (!vm.count("file-name")) {
    cout << "file name missing!" << endl;
    return 0;
  }

  fileNameList = vm["file-name"].as<vector<string>>();
  shared_ptr<fastype::Term> term = fastype::Term::open(fileNameList[0]);
  term->show(fileNameList[0]);

  int x;
  std::cin >> x;

  // shared_ptr<File> f = File::open(fileNameList[0]);
  return 0;
}
