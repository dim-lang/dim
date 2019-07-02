// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/unistr.h"
#include "unicode/ustdio.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File : private boost::noncopyable {
public:
  virtual ~File();

  const icu::UnicodeString &getFileName() const;
  int read(int line);

  static std::shared_ptr<File> open(const icu::UnicodeString &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const icu::UnicodeString &fileName);

  FILE *fd;
  icu::UnicodeString fileName;
  std::shared_ptr<Logger> log;

  char *readBuf_;
  std::vector<char *> lineBuf;
};

} // namespace fastype
