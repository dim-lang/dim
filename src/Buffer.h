// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include <memory>

namespace fastype {

class Buffer : private boost::noncopyable {
public:
  static Buffer moveFrom(char *buf, int32_t len, int32_t capacity);

  static Buffer copyFrom(char *buf, int32_t len);

  static Buffer create(int32_t capacity);

  static Buffer create(int32_t capacity, char value);

  static void free(Buffer buf);

private:
  Buffer();
  Buffer(int32_t capacity);
  Buffer(int32_t capacity, char value);
  virtual ~Buffer();

  char *buffer;
  int32_t capacity;
  int32_t size;
};

} // namespace fastype
