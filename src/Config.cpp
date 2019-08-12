// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Config.h"
#include "Define.h"
#include "Logging.h"
#include "boost/program_options/parsers.hpp"

namespace fastype {

Config::Config() : Logging("Config"), optDesc_("Fastype Options") {
  optDesc_.add_options()("help,h", "Help messages")("version,v",
                                                    "Version information")(
      "filename,f", boost_po::value<std::vector<std::string>>(), "File name");
  posOptDesc_.add("filename", -1);
}

Config::Config(int argCount, char **argList) : Config() {
  boost_po::store(boost_po::command_line_parser(argCount, argList)
                      .options(optDesc_)
                      .positional(posOptDesc_)
                      .run(),
                  varMap_);
  boost_po::notify(varMap_);
}

Config::Config(const std::vector<std::string> &fileNames) : Config() {
  for (int i = 0; i < fileNames.size(); i++) {
    boost_po::store(boost_po::parse_config_file(fileNames[i], optDesc_),
                    varMap_);
  }
  boost_po::notify(varMap_);
}

bool Config::needHelp() { return varMap_.count("help"); }

std::string Config::help() {
  ss_.clear();
  ss_ << optDesc_;
  return ss_.str();
}

bool Config::needVersion() { return varMap_.count("version"); }

std::string Config::version() {
  ss_.clear();
  ss_ << optDesc_;
  return ss_.str();
}

std::vector<std::string> inputFileNameList() {
  std::vector<std::string> fnList;
  if (varMap_.count("filename")) {
    fnList = varMap_["filename"].as<std::vector<std::string>>();
  } else {
    fnList = {"."};
  }
  return fnList;
}

} // namespace fastype
