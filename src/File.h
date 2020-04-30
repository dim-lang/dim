// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/filesystem.hpp"
#include <string>
#include <vector>

class File {
public:
  // read all into one string
  static std::string read(const std::string &fileName);

  // read all to line by line
  static std::vector<std::string> readline(const std::string &fileName);

  // write all into one file
  static int write(const std::string &fileName, const std::string &text);

  // write all lines into one file
  static int writeline(const std::string &fileName,
                       const std::vector<std::string> &lines);

  // write all into one file
  static int append(const std::string &fileName, const std::string &text);

  // write all lines into one file
  static int appendline(const std::string &fileName,
                        const std::vector<std::string> &lines);
};
