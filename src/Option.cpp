// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Option.h"
#include "Configure.h"
#include "Log.h"
#include <boost/program_options/parsers.hpp>
#include <fstream>

#define F_OPT "Fastype Options"
#define F_OPT_HELP "help"
#define F_OPT_H "h"
#define F_OPT_VERSION "version"
#define F_OPT_V "v"
#define F_OPT_INPUT_FILE "inputfile"
#define F_OPT_I "i"
#define F_OPT_HOST "host"
#define F_OPT_PORT "port"
#define F_OPT_P "p"
#define F_OPT_PORT_DEFAULT 10001
#define F_OPT_THREAD "thread"
#define F_OPT_T "t"
#define F_OPT_THREAD_DEFAULT 4
#define F_OPT_DEBUG "debug"
#define F_OPT_D "d"

Option::Option() : optDesc_(F_OPT) {
  optDesc_.add_options()(F_OPT_HELP "," F_OPT_H, "help message")(
      F_OPT_VERSION "," F_OPT_V, "version information")(
      F_OPT_INPUT_FILE "," F_OPT_I, boost_po::value<std::vector<std::string>>(),
      "input file names")(F_OPT_HOST, boost_po::value<std::string>(),
                          "service host")(
      F_OPT_PORT "," F_OPT_P, boost_po::value<int>(),
      "service port")(F_OPT_THREAD "," F_OPT_T, boost_po::value<int>(),
                      "thread number")(F_OPT_DEBUG "," F_OPT_D, "debug mode");
  posOptDesc_.add(F_OPT_INPUT_FILE, -1);
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

bool Option::hasHelp() const { return varMap_.count(F_OPT_HELP); }

std::string Option::help() const {
  std::stringstream ss;
  ss << optDesc_;
  return ss.str();
}

bool Option::hasVersion() const { return varMap_.count(F_OPT_VERSION); }

std::string Option::version() const { return "Fastype-" PROJECT_VERSION; }

bool Option::hasInputFile() const { return varMap_.count(F_OPT_INPUT_FILE); }

std::vector<std::string> Option::inputFileList() const {
  return varMap_[F_OPT_INPUT_FILE].as<std::vector<std::string>>();
}

int Option::port() const {
  if (varMap_.count(F_OPT_PORT)) {
    return varMap_[F_OPT_PORT].as<int>();
  }
  return F_OPT_PORT_DEFAULT;
}

std::string Option::host() const {
  if (varMap_.count(F_OPT_HOST)) {
    return varMap_[F_OPT_HOST].as<std::string>();
  }
  return "127.0.0.1";
}

int Option::threadSize() const {
  if (varMap_.count(F_OPT_THREAD)) {
    return varMap_[F_OPT_THREAD].as<int>();
  }
  return F_OPT_THREAD_DEFAULT;
}

bool Option::debug() const { return varMap_.count(F_OPT_DEBUG); }

#undef F_OPT
#undef F_OPT_HELP
#undef F_OPT_H
#undef F_OPT_VERSION
#undef F_OPT_V
#undef F_OPT_INPUT_FILE
#undef F_OPT_I
#undef F_OPT_HOST
#undef F_OPT_PORT
#undef F_OPT_P
#undef F_OPT_PORT_DEFAULT
#undef F_OPT_THREAD
#undef F_OPT_T
#undef F_OPT_THREAD_DEFAULT
#undef F_OPT_DEBUG
#undef F_OPT_D
