// Copyright 2019- <co-lang>
// Apache License Version 2.0

#pragma once
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"
#include <sstream>
#include <string>
#include <vector>

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

  boost::program_options::options_description optDesc_;
  boost::program_options::positional_options_description posOptDesc_;
  boost::program_options::variables_map varMap_;
};
