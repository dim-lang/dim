// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <list>
#include <memory>

namespace fastype {

class File : private boost::noncopyable {
public:
  virtual ~File();
  File(File &&) = default;
  File &operator=(File &&) = default;

  const std::string &getFileName() const;
  Line beginLine();
  Line endLine();
  Line getLine(int32_t line);

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

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
  std::list<std::shared_ptr<Line>> lineList;

  friend class LineIterator;
};

} // namespace fastype
