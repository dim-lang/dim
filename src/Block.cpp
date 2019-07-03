// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Block.h"
#include "boost/align/align_up.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>

#define ALLOC_ALIGN 16

namespace fastype {

Block::Block() : b(nullptr), capacity(0), size(0) {}

Block::Block(int32_t capacity)
    : b(new char[capacity]), capacity(capacity), size(0) {}

Block::Block(char *b, int32_t len) : b(nullptr), capacity(0), size(0) {
  assert(b);
  assert(len > 0);
  capacity = boost::alignment::align_up(len, ALLOC_ALIGN);
  this->b = new char[capacity];
  std::memcpy(this->b, b, len * sizeof(char));
}

Block::~Block() {
  if (b) {
    delete[] b;
    capacity = 0;
    size = 0;
  }
}

} // namespace fastype
