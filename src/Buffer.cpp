// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "Util.h"
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
  expand(capacity);
}

Buffer::Buffer(int32_t capacity, char value) : Buffer(capacity) {
  fdebug_assert_state();
  if ((int)value != 0) {
    for (int32_t i = 0; i < this->capacity; i++)
      buffer[i] = value;
  }
}

Buffer::Buffer(char *src, int32_t offset, int32_t n) : Buffer(n) {
  fdebug_assert_state();
  std::memcpy(buffer, src + offset, n);
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

int32_t Buffer::expand(int32_t capacity) {
  fdebug_assert_state();
  if (capacity <= this->size) {
    return this->capacity;
  }
  int32_t new_capacity =
      F_MAX_I32(ALLOC_ALIGN, boost::alignment::align_up(ALLOC_ALIGN, capacity));
  char *new_buffer = new char[new_capacity];
  int32_t new_size = this->size;
  int32_t new_seek = this->seek;
  std::memset(new_buffer, 0, new_capacity);
  if (this->buffer && this->size > 0) {
    std::memcpy(new_buffer, this->buffer, this->capacity);
  }
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

int32_t Buffer::getMargin() const { return this->capacity - this->size; }

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

int32_t Buffer::increaseSeek(int32_t inc) {
  fdebug_assert_state();
  assert(seek >= 0);
  assert(seek < this->size);
  if (inc < 0) {
    return decreaseSeek(-inc);
  }
  int32_t old_seek = this->seek;
  this->seek += F_MIN_I32(this->size - this->seek, inc);
  return old_seek;
}

int32_t Buffer::decreaseSeek(int32_t dec) {
  fdebug_assert_state();
  assert(seek >= 0);
  assert(seek < this->size);
  if (dec < 0) {
    return increaseSeek(-dec);
  }
  int32_t old_seek = this->seek;
  this->seek -= F_MIN_I32(this->seek, dec);
  return old_seek;
}

int32_t Buffer::reset() {
  fdebug_assert_state();
  int32_t old_seek = this->seek;
  this->seek = 0;
  return old_seek;
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

int32_t Buffer::read(const char *src, int32_t offset, int32_t n) {
  assert(src);
  assert(n >= 0);
  assert(offset >= 0);
  fdebug_assert_state();

  int32_t n = F_MAX_I32(n, this->capacity - this->size);
  const char *endBuffer = std::find_first_if(
      src + offset, src + offset + n, [](const char *i) { return *i == '\n'; });
  n = F_MIN_I32(n, endBuffer - (src + offset));
  if (n > 0) {
    std::memcpy(this->buffer, src + offset, n);
  }
  return F_MAX_I32(0, n);
}

int32_t Buffer::read(Buffer &l, int32_t n) {
  int32_t n =
      this->read(l.buffer + l.seek, 0, F_MAX_I32(l.getSize() - l.getSeek(), n));
  l.increaseSeek(n);
  return n;
}

int32_t Buffer::write(char *dest, int32_t offset, int32_t n) const {
  assert(dest);
  assert(n >= 0);
  assert(offset >= 0);
  fdebug_assert_state();

  int32_t n = F_MAX_I32(n, this->capacity - this->size);
  char *endBuffer =
      std::find_first_if(this->buffer + this->seek, this->buffer + this->size,
                         [](char *i) { return *i == '\n' });
  n = F_MIN_I32(n, endBuffer - (this->buffer + this->seek));
  if (n > 0) {
    std::memcpy(dest + offset, this->buffer + this->seek, n);
    increaseSeek(n);
  }
  return F_MAX_I32(0, n);
}

int32_t Buffer::write(Buffer &l, int32_t n) const {
  int32_t n = this->write(l.buffer, l.seek,
                          F_MAX_I32(this->getSize() - this->getSeek(),
                                    F_MAX_I32(l.getSize() - l.getSeek(), n)));
  return F_MAX_I32(0, n);
}

} // namespace fastype

#undef ALLOC_ALIGN
#undef fdebug_assert_state
#undef fdebug_assert_null
