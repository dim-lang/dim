// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include <memory>

namespace fastype {

class Block : private boost::noncopyable {
public:
  static std::shared_ptr<Block> moveFrom(char *buf, int32_t len,
                                         int32_t capacity);

  static std::shared_ptr<Block> copyFrom(char *buf, int32_t len);

  static std::shared_ptr<Block> create(int32_t capacity);

  static std::shared_ptr<Block> create(int32_t capacity, char value);

  static void free(std::shared_ptr<Block> block);

private:
  Block();
  Block(int32_t capacity);
  Block(int32_t capacity, char value);
  virtual ~Block();

  char *buffer;
  int32_t capacity;
  int32_t size;
};

} // namespace fastype
