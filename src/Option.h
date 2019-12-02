// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <sstream>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;

namespace fastype {

class Option {
public:
  Option(int argCount, char **argString);
  Option(const std::vector<std::string> &fileNames);
  virtual ~Option() = default;

  // --help, -h
  bool hasHelp() const;
  std::string help() const;

  // --version, -v
  bool hasVersion() const;
  std::string version() const;

  // --input-file, -i
  bool hasInputFile() const;
  std::vector<std::string> inputFileList() const;

  // --host
  std::string host() const;

  // --port, -p
  // by default: 10001
  int port() const;

  // --thread, -t
  // by default: 4
  int threadSize() const;

  // --debug, -d
  // by default: false
  bool debug() const;

private:
  Option();

  boost_po::options_description optDesc_;
  boost_po::positional_options_description posOptDesc_;
  boost_po::variables_map varMap_;
};

} // namespace fastype
