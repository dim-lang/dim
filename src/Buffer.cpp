// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "Log.h"
#include "Util.h"
#include "boost/align/align_up.hpp"
#include "fmt/format.h"
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
    assert(capacity_ >= 0);                                                    \
    assert(size_ >= 0);                                                        \
    assert(seek_ >= 0);                                                        \
    assert(seek_ <= size_ - 1);                                                \
  }
#endif

#ifndef fdebug_assert_null
#define fdebug_assert_null()                                                   \
  {                                                                            \
    assert(buffer_ == nullptr);                                                \
    assert(capacity_ == 0);                                                    \
    assert(size_ == 0);                                                        \
    assert(seek_ == 0);                                                        \
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

Buffer::Buffer()
    : buffer_(nullptr), capacity_(0), size_(0), seek_(0),
      log_(LogManager::getLogger("Buffer")) {
  F_DEBUGF("Buffer at: {}", (void *)this);
}

Buffer::Buffer(int32_t capacity) : Buffer() {
  fdebug_assert_null();
  expand(capacity);
}

Buffer::Buffer(int32_t capacity, char value) : Buffer(capacity) {
  fdebug_assert_state();
  if ((int)value != 0) {
    for (int32_t i = 0; i < capacity_; i++)
      buffer_[i] = value;
  }
}

Buffer::Buffer(char *src, int32_t offset, int32_t n) : Buffer(n) {
  fdebug_assert_state();
  std::memcpy(buffer_, src + offset, n);
}

Buffer::~Buffer() {
  release();
  fdebug_assert_null();
}

void Buffer::clear() {
  if (buffer_ && size_ > 0) {
    std::memset(buffer_, 0, capacity_);
    size_ = 0;
    seek_ = 0;
  }
}

void Buffer::release() {
  fdebug_assert_state();
  if (buffer_) {
    delete[] buffer_;
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    seek_ = 0;
  }
  fdebug_assert_null();
}

int32_t Buffer::expand(int32_t capacity) {
  fdebug_assert_state();
  if (capacity <= size_) {
    return capacity_;
  }
  int32_t new_capacity =
      F_MAX_I32(ALLOC_ALIGN, boost::alignment::align_up(ALLOC_ALIGN, capacity));
  char *new_buffer = new char[new_capacity];
  int32_t new_size = size_;
  int32_t new_seek = seek_;
  std::memset(new_buffer, 0, new_capacity);
  if (buffer_ && size_ > 0) {
    std::memcpy(new_buffer, buffer_, capacity_);
  }
  release();
  buffer_ = new_buffer;
  size_ = new_size;
  capacity_ = new_capacity;
  seek_ = new_seek;
  fdebug_assert_state();
  return capacity_;
}

bool Buffer::full() const {
  fdebug_assert_state();
  return size_ == capacity_;
}

bool Buffer::empty() const {
  fdebug_assert_state();
  return size_ == 0;
}

int32_t Buffer::margin() const {
  assert(capacity_ >= size_);
  return capacity_ - size_;
}

int32_t Buffer::seek() const {
  fdebug_assert_state();
  return seek_;
}

int32_t Buffer::increase(int32_t inc) {
  fdebug_assert_state();
  assert(seek_ >= 0);
  assert(seek_ < size_);
  if (inc < 0) {
    return decrease(-inc);
  }
  int32_t old = seek_;
  seek_ += F_MIN_I32(size_ - seek_, inc);
  return old;
}

int32_t Buffer::decrease(int32_t dec) {
  fdebug_assert_state();
  assert(seek_ >= 0);
  assert(seek_ < size_);
  if (dec < 0) {
    return increase(-dec);
  }
  int32_t old = seek_;
  seek_ -= F_MIN_I32(seek_, dec);
  return old;
}

int32_t Buffer::reseek() {
  fdebug_assert_state();
  int32_t old_seek = seek_;
  seek_ = 0;
  return old_seek;
}

bool Buffer::begin() const {
  fdebug_assert_state();
  return seek_ == 0;
}

bool Buffer::end() const {
  fdebug_assert_state();
  return seek_ == size_;
}

char *Buffer::data() const {
  fdebug_assert_state();
  return &buffer_[seek_];
}

int32_t Buffer::size() const {
  fdebug_assert_state();
  return size_;
}

int32_t Buffer::setSize(int32_t size) {
  int32_t old = size_;
  size_ = size;
  return old;
}

int32_t Buffer::capacity() const {
  fdebug_assert_state();
  return capacity_;
}

static const std::string EndOfLine = "\n";

int32_t Buffer::read(const char *src, int32_t offset, int32_t n) {
  assert(src);
  assert(n >= 0);
  assert(offset >= 0);
  fdebug_assert_state();

  n = F_MAX_I32(n, capacity_ - size_);
  const char *eb = std::find_first_of(src + offset, src + offset + n,
                                      EndOfLine.begin(), EndOfLine.end());
  n = F_MIN_I32(n, eb - (src + offset));
  if (n > 0) {
    std::memcpy(buffer_, src + offset, n);
  }
  return F_MAX_I32(0, n);
}

int32_t Buffer::read(Buffer &l, int32_t n) {
  n = read(l.buffer_ + l.seek_, 0, F_MAX_I32(l.size_ - l.seek_, n));
  l.increase(n);
  return n;
}

int32_t Buffer::write(char *dest, int32_t offset, int32_t n) {
  assert(dest);
  assert(n >= 0);
  assert(offset >= 0);
  fdebug_assert_state();

  n = F_MAX_I32(n, capacity_ - size_);
  char *eb = std::find_first_of(buffer_ + seek_, buffer_ + size_,
                                EndOfLine.begin(), EndOfLine.end());
  n = F_MIN_I32(n, eb - (buffer_ + seek_));
  if (n > 0) {
    std::memcpy(dest + offset, buffer_ + seek_, n);
    increase(n);
  }
  return F_MAX_I32(0, n);
}

int32_t Buffer::write(Buffer &l, int32_t n) {
  n = write(l.buffer_, l.seek_,
            F_MAX_I32(size_ - seek_, F_MAX_I32(l.size_ - l.seek_, n)));
  return F_MAX_I32(0, n);
}

std::string Buffer::toString() const {
  return fmt::format("[ @Buffer buffer: {}, capacity: {} size: {} seek: {} ]",
                     (void *)buffer_, capacity_, size_, seek_);
}

} // namespace fastype

#undef ALLOC_ALIGN
#undef fdebug_assert_state
#undef fdebug_assert_null
