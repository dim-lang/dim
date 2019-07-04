// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "boost/align/align_up.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <utility>

#define ALLOC_ALIGN 16

namespace fastype {

Buffer::Buffer() : buffer(nullptr), capacity(0), size(0) {}

Buffer::Buffer(int32_t capacity) : Buffer() {
  int align_cap = boost::alignment::align_up(ALLOC_ALIGN, capacity);
  capacity = align_cap;
  size = 0;
  buffer = new char[align_cap];
  std::memset(buffer, 0, align_cap);
}

Buffer::Buffer(int32_t capacity, char value) : Buffer(capacity) {
  if ((int)value != 0) {
    int align_cap = boost::alignment::align_up(ALLOC_ALIGN, capacity);
    for (int32_t i = 0; i < align_cap; i++)
      buffer[i] = value;
  }
}

Buffer::~Buffer() {
  if (buffer) {
    delete[] buffer;
    buffer = nullptr;
    capacity = 0;
    size = 0;
  }
}

} // namespace fastype
