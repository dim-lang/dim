// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include "Line.h"
#include "Stringify.h"
#include "boost/core/noncopyable.hpp"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class Cache : private boost::noncopyable, public Stringify {
public:
  Cache(Cache &&) = default;
  Cache &operator=(Cache &&) = default;
  virtual ~Cache();

  const std::string &fileName() const;
  Line &get(int lineNumber);
  int count();
  bool empty();
  int loaded() const;
  int truncate(int start, int length);
  int clear();
  virtual std::string toString() const;

  static std::shared_ptr<Cache> create(const std::string &fileName);
  static void destroy(std::shared_ptr<Cache> file);

private:
  Cache(const std::string &fileName);

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
