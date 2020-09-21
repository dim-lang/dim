// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Option.h"
#include "Configure.h"
#include <fstream>
#include <sstream>

namespace boost_po = boost::program_options;

#define OPT_HELP "help"
#define OPT_VERSION "version"
#define OPT_FILE "file"
#define OPT_DUMP_AST "dump-ast"
#define OPT_DUMP_SYMBOL "dump-symbol"

Option::Option() : optDesc_("Options") {
  optDesc_.add_options()(OPT_HELP ",h", "help message")(OPT_VERSION ",v",
                                                        "version information")(
      OPT_FILE ",f", boost_po::value<std::vector<std::string>>(),
      "file name")(OPT_DUMP_AST, "dump ast in graphviz dot file")(
      OPT_DUMP_SYMBOL, "dump symbol in graphviz dot file");
  posOptDesc_.add(OPT_FILE, -1);
}

Option::Option(int argCount, char **argList) : Option() {
  boost_po::parsed_options options =
      boost_po::command_line_parser(argCount, argList)
          .options(optDesc_)
          .positional(posOptDesc_)
          .allow_unregistered()
          .run();
  std::vector<std::string> unregistered = boost_po::collect_unrecognized(
      options.options, boost_po::include_positional);
  if (unregistered.size() > 0) {
    throw unregistered;
  }
  boost_po::store(options, varMap_);
  boost_po::notify(varMap_);
}

Option::Option(const std::vector<std::string> &fileNames) : Option() {
  for (int i = 0; i < (int)fileNames.size(); i++) {
    std::ifstream ifile(fileNames[i].c_str());
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

std::string Option::version() const { return "nerdc-" PROJECT_VERSION; }

bool Option::hasFileNames() const { return varMap_.count(OPT_FILE); }

std::vector<std::string> Option::fileNames() const {
  return varMap_[OPT_FILE].as<std::vector<std::string>>();
}

bool Option::hasDumpAst() const { return varMap_.count(OPT_DUMP_AST); }

bool Option::hasDumpSymbol() const { return varMap_.count(OPT_DUMP_SYMBOL); }
