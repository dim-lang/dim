// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "boost/align/align_up.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <utility>

#ifndef ALLOC_ALIGN
#define ALLOC_ALIGN 16
#endif

#ifndef NDEBUG

#ifndef fdebug_assert_state
#define fdebug_assert_state()                                                  \
  {                                                                            \
    assert(this->capacity >= 0);                                               \
    assert(this->size >= 0);                                                   \
    assert(this->seek >= 0);                                                   \
    assert(this->seek <= this->size - 1);                                      \
  }
#endif

#ifndef fdebug_assert_null
#define fdebug_assert_null()                                                   \
  {                                                                            \
    assert(this->buffer == nullptr);                                           \
    assert(this->capacity == 0);                                               \
    assert(this->size == 0);                                                   \
    assert(this->seek == 0);                                                   \
  }
#endif

#else

#ifndef fdebug_assert_state
#define fdebug_assert_state()
#endif
#ifndef fdebug_assert_null
#define fdebug_assert_null()
#endif

#endif

namespace fastype {

Buffer::Buffer() : buffer(nullptr), capacity(0), size(0), seek(0) {
  fdebug_assert_null();
}

Buffer::Buffer(int32_t capacity) : Buffer() {
  fdebug_assert_null();
  expandCapacity(capacity);
}

Buffer::Buffer(int32_t capacity, char value) : Buffer(capacity) {
  fdebug_assert_state();
  if ((int)value != 0) {
    for (int32_t i = 0; i < this->capacity; i++)
      buffer[i] = value;
  }
}
Buffer::Buffer(char *src, int32_t off, int32_t len) : Buffer(len) {
  fdebug_assert_state();
  std::memcpy(buffer, src + off, len);
}

Buffer::~Buffer() {
  release();
  fdebug_assert_null();
}

void Buffer::clear() {
  if (buffer) {
    std::memset(buffer, 0, capacity);
    size = 0;
    seek = 0;
  }
}

void Buffer::release() {
  fdebug_assert_state();
  if (buffer) {
    delete[] buffer;
    buffer = nullptr;
    size = 0;
    capacity = 0;
    seek = 0;
  }
  fdebug_assert_null();
}

int32_t Buffer::expandCapacity(int32_t capacity) {
  fdebug_assert_state();
  if (capacity <= this->size) {
    return this->capacity;
  }
  int32_t new_capacity = std::max<int32_t>(
      ALLOC_ALIGN, boost::alignment::align_up(ALLOC_ALIGN, capacity));
  char *new_buffer = new char[new_capacity];
  int32_t new_size = this->size;
  int32_t new_seek = this->seek;
  std::memset(new_buffer, 0, new_capacity);
  std::memcpy(new_buffer, this->buffer, this->capacity);
  release();
  this->buffer = new_buffer;
  this->size = new_size;
  this->capacity = new_capacity;
  this->seek = new_seek;
  fdebug_assert_state();
  return this->capacity;
}

bool Buffer::full() const {
  fdebug_assert_state();
  return this->size == this->capacity;
}

bool Buffer::empty() const {
  fdebug_assert_state();
  return this->size == 0;
}

int32_t Buffer::getSeek() const {
  fdebug_assert_state();
  return seek;
}

int32_t Buffer::setSeek(int32_t seek) {
  fdebug_assert_state();
  assert(seek >= 0);
  assert(seek < this->size);
  int32_t old_seek = this->seek;
  this->seek = seek;
  return old_seek;
}

int32_t Buffer::reseek() {
  fdebug_assert_state();
  this->seek = 0;
}

bool Buffer::begin() const {
  fdebug_assert_state();
  return this->seek == 0;
}

bool Buffer::end() const {
  fdebug_assert_state();
  return this->seek == this->size;
}

const char *Buffer::getData() const {
  fdebug_assert_state();
  return &this->buffer[this->seek];
}

int32_t Buffer::getSize() const {
  fdebug_assert_state();
  return this->size;
}

int32_t Buffer::getCapacity() const {
  fdebug_assert_state();
  return this->capacity;
}

int32_t Buffer::read(const char *src, int32_t off, int32_t len) {
  assert(src);
  assert(len >= 0);
  assert(off >= 0);
  fdebug_assert_state();

  int32_t n = std::max(len, this->capacity - this->size);
  if (n > 0) {
    std::memcpy(this->buffer, src + off, n);
  }
  return std::max(0, n);
}

int32_t Buffer::read(Buffer &buf, int32_t len) {
  int32_t n = this->read(buf.buffer + buf.seek, 0,
                         std::max<int32_t>(buf.getSize() - buf.getSeek(), len));
  buf.setSeek(buf.getSeek() + n);
  return n;
}

int32_t Buffer::write(char *dest, int32_t off, int32_t len) const {
  assert(dest);
  assert(len >= 0);
  assert(off >= 0);
  fdebug_assert_state();

  int32_t n = std::max(len, this->capacity - this->size);
  if (n > 0) {
    std::memcpy(dest + off, this->buffer + this->seek, n);
  }
  return std::max(0, n);
}

int32_t Buffer::write(Buffer &buf, int32_t len) const {
  int32_t n = this->write(
      buf.buffer, buf.seek,
      std::max<int32_t>(this->getSize() - this->getSeek(),
                        std::max<int32_t>(buf.getSize() - buf.getSeek(), len)));
  return std::max(0, n);
}

} // namespace fastype

#undef ALLOC_ALIGN
#undef fdebug_assert_state
#undef fdebug_assert_null
