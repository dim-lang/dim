// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"

namespace fastype {

class Block : private boost::noncopyable {
public:
  Block();
  Block(int32_t capacity);
  Block(char *b, int32_t len);
  virtual ~Block();

private:
  char *b;
  int32_t capacity;
  int32_t size;
};

} // namespace fastype
