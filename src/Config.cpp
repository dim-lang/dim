// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Config.h"
#include "Define.h"

namespace fastype {

Config::Config(int argCount, char **argList)
    : optDesc_("Fastype Options"),
      version_(std::string("Fastype-") + PROJECT_VERSION), needHelp_(false),
      needVersion_(false) {
  optDesc_.add_options()("help,h", "Help messages")("version,v", version_)(
      "filename,f", boost_po::value<std::vector<std::string>>(), "File name");
  posOptDesc_.add("filename", -1);

  boost_po::store(boost_po::command_line_parser(argCount, argList)
                      .options(optDesc_)
                      .positional(posOptDesc_)
                      .run(),
                  varMap_);
  boost_po::notify(varMap_);

  if (varMap_.count("help")) {
    needHelp_ = true;
  }

  if (varMap_.count("version")) {
    needVersion_ = true;
  }

  if (!varMap_.count("filename")) {
    fileNameList_ = {"."};
  } else {
    fileNameList_ = varMap_["file-name"].as<std::vector<std::string>>();
  }
}

} // namespace fastype
