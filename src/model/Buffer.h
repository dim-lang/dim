// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "memory/Buf.h"
#include "memory/Row.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class Buffer : public Stringify {
public:
  Buffer(Buffer &&) = default;
  Buffer &operator=(Buffer &&) = default;
  virtual ~Buffer();

  const std::string &fileName() const;
  Row &get(int lineNumber);
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
  Buf readBuffer_;
  std::vector<Row> lineList_;

  friend class Row;
};

} // namespace fastype
