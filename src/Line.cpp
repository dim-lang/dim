// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
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

Line::Line() : buffer(nullptr), capacity(0), size(0), seek(0) {
  fdebug_assert_null();
}

Line::Line(int32_t capacity) : Line() {
  fdebug_assert_null();
  expandCapacity(capacity);
}

Line::Line(int32_t capacity, char value) : Line(capacity) {
  fdebug_assert_state();
  if ((int)value != 0) {
    for (int32_t i = 0; i < this->capacity; i++)
      buffer[i] = value;
  }
}
Line::Line(char *src, int32_t off, int32_t len) : Line(len) {
  fdebug_assert_state();
  std::memcpy(buffer, src + off, len);
}

Line::~Line() {
  release();
  fdebug_assert_null();
}

void Line::clear() {
  if (buffer) {
    std::memset(buffer, 0, capacity);
    size = 0;
    seek = 0;
  }
}

void Line::release() {
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

int32_t Line::expandCapacity(int32_t capacity) {
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

bool Line::full() const {
  fdebug_assert_state();
  return this->size == this->capacity;
}

bool Line::empty() const {
  fdebug_assert_state();
  return this->size == 0;
}

int32_t Line::getSeek() const {
  fdebug_assert_state();
  return seek;
}

int32_t Line::setSeek(int32_t seek) {
  fdebug_assert_state();
  assert(seek >= 0);
  assert(seek < this->size);
  int32_t old_seek = this->seek;
  this->seek = seek;
  return old_seek;
}

int32_t Line::reseek() {
  fdebug_assert_state();
  this->seek = 0;
}

bool Line::begin() const {
  fdebug_assert_state();
  return this->seek == 0;
}

bool Line::end() const {
  fdebug_assert_state();
  return this->seek == this->size;
}

const char *Line::getData() const {
  fdebug_assert_state();
  return &this->buffer[this->seek];
}

int32_t Line::getSize() const {
  fdebug_assert_state();
  return this->size;
}

int32_t Line::getCapacity() const {
  fdebug_assert_state();
  return this->capacity;
}

int32_t Line::read(const char *src, int32_t off, int32_t len) {
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

int32_t Line::read(Line &l, int32_t len) {
  int32_t n = this->read(l.buffer + l.seek, 0,
                         std::max<int32_t>(l.getSize() - l.getSeek(), len));
  l.setSeek(l.getSeek() + n);
  return n;
}

int32_t Line::write(char *dest, int32_t off, int32_t len) const {
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

int32_t Line::write(Line &l, int32_t len) const {
  int32_t n = this->write(
      l.buffer, l.seek,
      std::max<int32_t>(this->getSize() - this->getSeek(),
                        std::max<int32_t>(l.getSize() - l.getSeek(), len)));
  return std::max(0, n);
}

} // namespace fastype

#undef ALLOC_ALIGN
#undef fdebug_assert_state
#undef fdebug_assert_null
