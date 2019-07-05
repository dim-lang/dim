// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include "Line.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File : private boost::noncopyable {
public:
  virtual ~File();
  File(File &&) = default;
  File &operator(File &&) = default;

  const std::string &getFileName() const;

  static std::unique_ptr<File> open(const std::string &fileName);
  static void close(std::unique_ptr<File> file);

private:
  File(const std::string &fileName);

  int64_t load(int count);
  int64_t loadOne();
  int64_t loadAll();

  FILE *fd;
  std::string fileName;
  std::shared_ptr<Logger> log;

  char *readBuffer;
  char *writeBuffer;
  std::vector<std::unique_ptr<Buffer>> buffer;

  friend class LineIterator;
};

} // namespace fastype
