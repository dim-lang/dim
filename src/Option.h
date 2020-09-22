// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Configure.h"
#include "boost/program_options.hpp"
#include "boost/program_options/parsers.hpp"
#include <sstream>
#include <string>
#include <vector>

#define OPT_HELP "help"
#define OPT_VERSION "version"
#define OPT_INPUT_FILE "input-file"
#define OPT_DUMP_AST "dump-ast"
#define OPT_DUMP_SYMBOL "dump-symbol"

class Option {
public:
  Option(int argc, char **argv) : opt_desc("nerdc options") {
    opt_desc.add_options()(OPT_HELP ",h", "help message")(
        OPT_VERSION ",v", "version information")(
        OPT_INPUT_FILE,
        boost::program_options::value<std::vector<std::string>>(),
        "input files")(OPT_DUMP_AST, "dump ast in graphviz dot file")(
        OPT_DUMP_SYMBOL, "dump symbol in graphviz dot file");
    positional_opt_desc.add(OPT_INPUT_FILE, -1);
    boost::program_options::parsed_options options =
        boost::program_options::command_line_parser(argc, argv)
            .options(opt_desc)
            .positional(positional_opt_desc)
            .allow_unregistered()
            .run();
    std::vector<std::string> unregistered =
        boost::program_options::collect_unrecognized(
            options.options, boost::program_options::include_positional);
    if (unregistered.size() > 0) {
      throw unregistered;
    }
    boost::program_options::store(options, variable_map);
    boost::program_options::notify(variable_map);
  }
  virtual ~Option() = default;

  // --help, -h
  // --version, -v
  // --input-file
  // --dump-ast
  // --dump-symbol
  bool has(const std::string &opt) const { return variable_map.count(opt); }

  template <typename T> T get(const std::string &opt) const {
    return variable_map[opt].as<T>();
  }

  template <> std::string get<std::string>(const std::string &opt) const {
    if (opt == OPT_HELP) {
      std::stringstream ss;
      ss << opt_desc;
      return ss.str();
    }
    if (opt == OPT_VERSION) {
      return "nerdc-" PROJECT_VERSION;
    }
    return variable_map[opt].as<std::string>();
  }

private:
  Option();
  boost::program_options::options_description opt_desc;
  boost::program_options::positional_options_description positional_opt_desc;
  boost::program_options::variables_map variable_map;
};
