// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "boost/core/noncopyable.hpp"
#include "model/Buffer.h"
#include "model/Line.h"
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
  Line &get(int lineNumber);
  int count();
  bool empty();
  int loaded() const;
  int truncate(int start, int length);
  int clear();
  virtual std::string toString() const;

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const std::string &fileName);

  // load all lines
  // @return readed bytes
  int64_t load();

  std::string fileName_;
  FILE *fd_;
  bool loaded_;
  Buffer readBuffer_;
  std::vector<Line> lineList_;

  friend class Line;
};

} // namespace fastype
