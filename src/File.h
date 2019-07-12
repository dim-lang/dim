// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include "Line.h"
#include "LineBound.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File : private boost::noncopyable, public Logging {
public:
  File(File &&) = default;
  File &operator=(File &&) = default;
  virtual ~File();

  virtual const std::string &fileName() const;
  virtual Line begin();
  virtual Line end();
  virtual Line line(int32_t lineNumber);
  virtual std::string toString() const;

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const std::string &fileName);

  // load n buffers and lines
  // @return loaded bytes
  int64_t load(int n);
  // load 1 buffer and lines
  // @return loaded bytes
  int64_t loadOne();
  // load all buffers and lines
  // @return loaded bytes
  int64_t loadAll();

  // if last line exists and is opened, close it
  void closeLastLine(LineBound right);
  // open new line at specified left line bound
  void openNewLine(File *fp, int32_t lineNumber, LineBound left);

  std::string fileName_;
  FILE *fd_;
  bool eof_;
  Buffer readBuffer_;
  std::vector<std::shared_ptr<Buffer>> bufferList_;
  std::vector<Line> lineList_;

  friend class Line;
  friend class Buffer;
};

} // namespace fastype
