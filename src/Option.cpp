// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Option.h"
#include "Configure.h"
#include "Log.h"
#include <fstream>

namespace boost_po = boost::program_options;

#define OPT "Options"
#define OPT_HELP "help"
#define OPT_H "h"
#define OPT_VERSION "version"
#define OPT_V "v"
#define OPT_FILE "file"
#define OPT_F "f"
#define OPT_DEBUG "debug"
#define OPT_D "d"

Option::Option() : optDesc_(OPT) {
  optDesc_.add_options()(OPT_HELP "," OPT_H, "help message")(
      OPT_VERSION "," OPT_V, "version information")(
      OPT_FILE "," OPT_F, boost_po::value<std::vector<std::string>>(),
      "file name")(OPT_DEBUG "," OPT_D, "debug mode");
  posOptDesc_.add(OPT_FILE, -1);
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

bool Option::hasHelp() const { return varMap_.count(OPT_HELP); }

std::string Option::help() const {
  std::stringstream ss;
  ss << optDesc_;
  return ss.str();
}

bool Option::hasVersion() const { return varMap_.count(OPT_VERSION); }

std::string Option::version() const { return "shepherdc-" PROJECT_VERSION; }

bool Option::hasFileNames() const { return varMap_.count(OPT_FILE); }

std::vector<std::string> Option::fileNames() const {
  return varMap_[OPT_FILE].as<std::vector<std::string>>();
}

bool Option::debug() const { return varMap_.count(OPT_DEBUG); }

#undef OPT
#undef OPT_HELP
#undef OPT_H
#undef OPT_VERSION
#undef OPT_V
#undef OPT_FILE
#undef OPT_F
#undef OPT_DEBUG
#undef OPT_D
