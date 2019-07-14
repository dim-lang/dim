// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "Logging.h"
#include "Position.h"
#include "Stringify.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File : private boost::noncopyable, public Logging, Stringify {
public:
  File(File &&) = default;
  File &operator=(File &&) = default;
  virtual ~File();

  const std::string &fileName() const;
  std::shared_ptr<Line> begin();
  std::shared_ptr<Line> end();
  std::shared_ptr<Line> line(int32_t lineNumber);
  std::shared_ptr<Line> next(std::shared_ptr<Line> l);
  std::shared_ptr<Line> previous(std::shared_ptr<Line> l);
  virtual std::string toString() const;

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const std::string &fileName);

  // load 1 buffer and lines
  // @return loaded bytes
  int64_t load();

  // load at least n lines (e.g lineList_.size() >= n), or no more to read
  // @return loaded bytes
  int64_t loadUntil(int n);

  // load all buffers and lines
  // @return loaded bytes
  int64_t loadAll();

  // if last line exists and is opened, close it
  void closeLastLine(Position right);
  // open new line at specified left line bound
  void openNewLine(File *fp, int32_t lineNumber, Position left);

  bool hasNext(std::shared_ptr<Line> l);
  bool hasPrevious(std::shared_ptr<Line> l);

  std::string fileName_;
  FILE *fd_;
  bool loaded_;
  Line readBuffer_;
  std::vector<std::shared_ptr<Line>> lineList_;

  friend class Line;
};

} // namespace fastype
