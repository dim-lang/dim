// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File;

class FileIterator {
public:
  virtual ~FileIterator() = default;

private:
  FileIterator();

  friend class File;
};

class File : private boost::noncopyable {
public:
  virtual ~File();

  const std::string &getFileName() const;
  FileIterator getFileIterator();

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const std::string &fileName);
  int64_t readToBuffer();

  FILE *fd;
  std::string fileName;
  std::shared_ptr<Logger> log;

  char *readBuf;
  std::vector<char *> buffer;

  friend class LineIterator;
};

} // namespace fastype
