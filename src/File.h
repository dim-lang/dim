// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "Logging.h"
#include "Stringify.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/unistr.h"
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
  icu::UnicodeString &getLine(int lineNumber);
  int lineCount();
  bool empty();
  int loaded() const;
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

  std::string fileName_;
  FILE *fd_;
  bool loaded_;
  std::vector<char> readBuffer_;
  std::vector<Line> lineList_;

  UConverter *converter_;

  friend class Line;
};

} // namespace fastype
