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

  // --help, -h
  bool hasHelp() const;
  std::string help() const;

  // --version, -v
  bool hasVersion() const;
  std::string version() const;

  // --input-file, -i
  bool hasInputFile() const;
  std::vector<std::string> inputFileList() const;

  // --port, -p
  // by default: 10001
  int port() const;

  // --thread-size, -t
  // by default: 4
  int threadSize() const;

  // --daemonize, -d
  // by default: false
  bool daemonize() const;

private:
  Config();

  boost_po::options_description optDesc_;
  boost_po::positional_options_description posOptDesc_;
  boost_po::variables_map varMap_;
};

} // namespace fastype
