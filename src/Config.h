// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "boost/program_options.hpp"
#include <sstream>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;

namespace fastype {

class Config : public Logging {
public:
  Config(int argCount, char **argString);
  Config(const std::vector<std::string> &fileNames);
  virtual ~Config() = default;

  bool needHelp();
  std::string help();
  bool needVersion();
  std::string version();
  std::vector<std::string> inputFileNameList();

private:
  Config();

  boost_po::options_description optDesc_;
  boost_po::positional_options_description posOptDesc_;
  boost_po::variables_map varMap_;
  std::stringstream ss_;
};

} // namespace fastype
