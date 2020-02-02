// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <sstream>
#include <string>
#include <vector>
namespace boost_po = boost::program_options;

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

  // --file-name, -f
  bool hasFileNames() const;
  std::vector<std::string> fileNames() const;

  // --job, -j
  // by default: system thread size
  int job() const;

  // --debug, -d
  // by default: false
  bool debug() const;

private:
  Option();

  boost_po::options_description optDesc_;
  boost_po::positional_options_description posOptDesc_;
  boost_po::variables_map varMap_;
};
