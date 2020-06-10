// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/filesystem.hpp"
#include <string>
#include <vector>

namespace detail {

class FileWriterIterator {};

class FileInfo {
public:
  FileInfo(const std::string &fileName);
  // close fp_
  virtual ~FileInfo();
  const std::string &fileName() const;

protected:
  std::string fileName_;
  FILE *fp_;
};

} // namespace detail

class FileReader : public detail::FileInfo {
public:
  FileReader(const std::string &fileName);
  virtual ~FileReader();

  // reset reading status
  void reset();
};

class FileWriter : public detail::FileInfo {
public:
  FileWriter(const std::string &fileName);
  virtual ~FileWriter();
};

class FileAppender : public detail::FileInfo {
public:
  FileAppender(const std::string &fileName);
  virtual ~FileAppender();
};

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
