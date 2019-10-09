// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Block.h"
#include "Cowstr.h"
#include "Row.h"
#include "SmartPointer.h"
#include "Stringify.h"
#include <cstdio>
#include <cstring>
#include <vector>

namespace fastype {

/**
 * Buffer is a file's memory storage
 */

class Buffer : public Stringify {
public:
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(Buffer &&) = delete;
  virtual ~Buffer();

  const std::string &fileName() const;
  Row get(int lineNumber);
  int count();
  bool empty();
  int loaded() const;
  int truncate(int start, int length);
  int clear();
  virtual std::string toString() const;

  static Sptr<Buffer> open(const std::string &fileName);
  static void close(Sptr<Buffer> file);

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
};

} // namespace fastype
