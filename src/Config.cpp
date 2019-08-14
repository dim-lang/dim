// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Config.h"
#include "Define.h"
#include "Logging.h"
#include "boost/program_options/parsers.hpp"

#define F_OPT "Fastype Options"
#define F_OPT_HELP "help"
#define F_OPT_H "h"
#define F_OPT_VERSION "version"
#define F_OPT_V "v"
#define F_OPT_INPUT_FILE "input-file"
#define F_OPT_I "i"
#define F_OPT_PORT "port"
#define F_OPT_P "p"
#define F_OPT_PORT_DEFAULT 10001
#define F_OPT_THREAD_SIZE "thread-size"
#define F_OPT_T "t"
#define F_OPT_THREAD_SIZE_DEFAULT 4
#define F_OPT_DAEMONIZE "daemonize"
#define F_OPT_D "d"

namespace fastype {

Config::Config() : optDesc_(F_OPT) {
  optDesc_.add_options()(F_OPT_HELP "," F_OPT_H, "help message")(
      F_OPT_VERSION "," F_OPT_V, "version information")(
      F_OPT_INPUT_FILE "," F_OPT_I, boost_po::value<std::vector<std::string>>(),
      "input file name")(F_OPT_PORT "," F_OPT_P, boost_po::value<int>(),
                         "service port number")(F_OPT_DAEMONIZE "," F_OPT_D,
                                                "run as a daemonize");
  posOptDesc_.add(F_OPT_INPUT_FILE, -1);
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
    boost_po::store(boost_po::parse_config_file(fileNames[i].data(), optDesc_),
                    varMap_);
  }
  boost_po::notify(varMap_);
}

bool Config::hasHelp() const { return varMap_.count(F_OPT_HELP); }

std::string Config::help() const {
  std::stringstream ss;
  ss.clear();
  ss << optDesc_;
  return ss.str();
}

bool Config::hasVersion() const { return varMap_.count(F_OPT_VERSION); }

std::string Config::version() const { return "Fastype-" PROJECT_VERSION; }

bool Config::hasInputFile() const { return varMap_.count(F_OPT_INPUT_FILE); }

std::vector<std::string> Config::inputFileList() const {
  return varMap_[F_OPT_INPUT_FILE].as<std::vector<std::string>>();
}

int Config::port() const {
  if (varMap_.count(F_OPT_PORT)) {
    return varMap_[F_OPT_PORT].as<int>();
  }
  return F_OPT_PORT_DEFAULT;
}

int Config::threadSize() const {
  if (varMap_.count(F_OPT_THREAD_SIZE)) {
    return varMap_[F_OPT_THREAD_SIZE].as<int>();
  }
  return F_OPT_THREAD_SIZE_DEFAULT;
}

bool Config::daemonize() const { return varMap_.count(F_OPT_DAEMONIZE); }

} // namespace fastype

#undef F_OPT
#undef F_OPT_HELP
#undef F_OPT_H
#undef F_OPT_VERSION
#undef F_OPT_V
#undef F_OPT_INPUT_FILE
#undef F_OPT_I
#undef F_OPT_PORT
#undef F_OPT_P
#undef F_OPT_PORT_DEFAULT
#undef F_OPT_THREAD_SIZE
#undef F_OPT_T
#undef F_OPT_THREAD_SIZE_DEFAULT
#undef F_OPT_DAEMONIZE
#undef F_OPT_D
