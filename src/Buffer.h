// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Block.h"
#include "Cowstr.h"
#include "Row.h"
#include "Stringify.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

/**
 * Buffer is a file's memory storage
 */

class Buffer : public Stringify {
public:
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer(Buffer &&);
  Buffer &operator=(Buffer &&);
  virtual ~Buffer();

  const std::string &fileName() const;
  Row get(int lineNumber);
  int count();
  bool empty();
  int loaded() const;
  int truncate(int start, int length);
  int clear();
  virtual std::string toString() const;

  static std::shared_ptr<Buffer> open(const std::string &fileName);
  static void close(std::shared_ptr<Buffer> file);

private:
  Buffer(const std::string &fileName);

  // load all lines
  // @return readed bytes
  int64_t load();

  std::string fileName_;
  FILE *fd_;
  bool loaded_;
  Block readBuffer_;
  std::vector<Row> lineList_;

  friend class Row;
};

} // namespace fastype
