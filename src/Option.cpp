// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Option.h"
#include "Configure.h"
#include "Log.h"
#include <fstream>

#define C_OPT "Options"
#define C_OPT_HELP "help"
#define C_OPT_H "h"
#define C_OPT_VERSION "version"
#define C_OPT_V "v"
#define C_OPT_FILE_NAMES "file-name"
#define C_OPT_F "f"
#define C_OPT_JOB "job"
#define C_OPT_J "j"
#define C_OPT_DEBUG "debug"
#define C_OPT_D "d"

Option::Option() : optDesc_(C_OPT) {
  optDesc_.add_options()(C_OPT_HELP "," C_OPT_H, "help message")(
      C_OPT_VERSION "," C_OPT_V, "version information")(
      C_OPT_FILE_NAMES "," C_OPT_F, boost_po::value<std::vector<std::string>>(),
      "file names")(C_OPT_JOB "," C_OPT_J, "concurrent job count")(
      C_OPT_DEBUG "," C_OPT_D, "debug mode");
  posOptDesc_.add(C_OPT_FILE_NAMES, -1);
}

Option::Option(int argCount, char **argList) : Option() {
  boost_po::store(boost_po::command_line_parser(argCount, argList)
                      .options(optDesc_)
                      .positional(posOptDesc_)
                      .run(),
                  varMap_);
  boost_po::notify(varMap_);
}

Option::Option(const std::vector<std::string> &fileNames) : Option() {
  for (int i = 0; i < (int)fileNames.size(); i++) {
    std::ifstream ifile(fileNames[i].data());
    boost_po::store(boost_po::parse_config_file(ifile, optDesc_), varMap_);
  }
  boost_po::notify(varMap_);
}

bool Option::hasHelp() const { return varMap_.count(C_OPT_HELP); }

std::string Option::help() const {
  std::stringstream ss;
  ss << optDesc_;
  return ss.str();
}

bool Option::hasVersion() const { return varMap_.count(C_OPT_VERSION); }

std::string Option::version() const { return "coli-" PROJECT_VERSION; }

bool Option::hasFileNames() const { return varMap_.count(C_OPT_FILE_NAMES); }

std::vector<std::string> Option::fileNames() const {
  return varMap_[C_OPT_FILE_NAMES].as<std::vector<std::string>>();
}

int Option::job() const {
  return varMap_.count(C_OPT_JOB) ? varMap_[C_OPT_JOB].as<int>() : 1;
}

bool Option::debug() const { return varMap_.count(C_OPT_DEBUG); }

#undef C_OPT
#undef C_OPT_HELP
#undef C_OPT_H
#undef C_OPT_VERSION
#undef C_OPT_V
#undef C_OPT_FILE_NAMES
#undef C_OPT_F
#undef C_OPT_JOB
#undef C_OPT_J
#undef C_OPT_DEBUG
#undef C_OPT_D
