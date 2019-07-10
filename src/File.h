// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
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
  File(File &&) = default;
  File &operator=(File &&) = default;

  const std::string &getFileName() const;
  Line begin();
  Line end();
  Line getLine(int32_t lineNumber);
  std::string toString() const;

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const std::string &fileName);
  virtual ~File();

  // read n buffers and lines
  // @return readed bytes
  int64_t read(int n);
  // read 1 buffer and lines
  // @return readed bytes
  int64_t readOne();
  // read all buffers and lines
  // @return readed bytes
  int64_t readAll();

  std::string fileName;
  FILE *fd;
  bool eof;
  Buffer readBuffer;
  std::vector<std::shared_ptr<Buffer>> bufferList;
  std::vector<Line> lineList;
  std::shared_ptr<Logger> log;
};

} // namespace fastype
