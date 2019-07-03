// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Block.h"
#include "boost/align/align_up.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>

#define ALLOC_ALIGN 16

namespace fastype {

std::shared_ptr<Block> Block::moveFrom(char *buf, int32_t len,
                                       int32_t capacity) {
  std::shared_ptr<Block> block = std::shared_ptr<Block>(new Block());
  block->buffer = buf;
  block->size = len;
  block->capacity = capacity;
  return block;
}

std::shared_ptr<Block> Block::copyFrom(char *buf, int32_t len) {
  std::shared_ptr<Block> block = std::shared_ptr<Block>(new Block(len));
  std::memcpy(block->buffer, buf, len);
  return block;
}

std::shared_ptr<Block> Block::create(int32_t capacity);

std::shared_ptr<Block> Block::create(int32_t capacity, char value);

void Block::free(std::shared_ptr<Block> block);

Block::Block() : buffer(nullptr), capacity(0), size(0) {}

Block::Block(int32_t capacity) : Block() {
  int align_cap = boost::alignment::align_up(ALLOC_ALIGN, capacity);
  capacity = align_cap;
  size = 0;
  buffer = new char[align_cap];
  std::memset(buffer, 0, align_cap);
}

Block::Block(int32_t capacity, char value) : Block(capacity) {
  if ((int)value != 0) {
    int align_cap = boost::alignment::align_up(ALLOC_ALIGN, capacity);
    for (int32_t i = 0; i < align_cap; i++)
      buffer[i] = value;
  }
}

Block::~Block() {
  if (buffer) {
    delete[] buffer;
    capacity = 0;
    size = 0;
  }
}

} // namespace fastype
