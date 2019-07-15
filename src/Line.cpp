// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Logging.h"
#include "Util.h"
#include "boost/align/align_up.hpp"
#include "exceptions/MemoryPointerException.h"
#include "exceptions/ParameterException.h"
#include "fmt/format.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <utility>

#ifndef ALLOC_ALIGN
#define ALLOC_ALIGN 16
#endif

namespace fastype {

Line::Line() : Logging("Line"), lineNumber_(-1), buffer_() {}

Line::Line(int32_t lineNumber) : Line() {
  if (lineNumber < 0) {
    F_THROW_EXF(ParameterException, "lineNumber: {}", lineNumber);
  }
  lineNumber_ = lineNumber;
}

Line::Line(int32_t lineNumber, int32_t capacity) : Line(lineNumber) {
  expand(capacity);
}

Line::Line(int32_t lineNumber, int32_t capacity, char value)
    : Line(lineNumber, capacity) {
  if ((int)value != 0) {
    for (int32_t i = 0; i < capacity_; i++)
      buffer_[i] = value;
  }
}

Line::Line(int32_t lineNumber, char *src, int32_t offset, int32_t n)
    : Line(lineNumber, n) {
  std::memcpy(buffer_, src + offset, n);
}

Line::~Line() { release(); }

void Line::clear() {
  if (buffer_ && size_ > 0) {
    std::memset(buffer_, 0, capacity_);
    size_ = 0;
    seek_ = 0;
  }
}

void Line::release() {
  F_DEBUG(toString());
  if (buffer_) {
    delete[] buffer_;
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    seek_ = 0;
  }
}

int32_t Line::expand(int32_t capacity) {
  if (capacity <= size_) {
    F_DEBUGF("capacity <= size_, capacity:{} this:{}", capacity, toString());
    return capacity_;
  }
  F_DEBUGF("capacity > size_, capacity:{} this:{}", capacity, toString());
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
  return capacity_;
}

bool Line::full() const { return size_ == capacity_; }

bool Line::empty() const { return size_ == 0; }

int32_t Line::margin() const {
  assert(capacity_ >= size_);
  return capacity_ - size_;
}

int32_t Line::seek() const { return seek_; }

int32_t Line::increase(int32_t inc) {
  assert(seek_ >= 0);
  assert(seek_ < size_);
  if (inc < 0) {
    return decrease(-inc);
  }
  int32_t old = seek_;
  seek_ += F_MIN_I32(size_ - seek_, inc);
  return old;
}

int32_t Line::decrease(int32_t dec) {
  assert(seek_ >= 0);
  assert(seek_ < size_);
  if (dec < 0) {
    return increase(-dec);
  }
  int32_t old = seek_;
  seek_ -= F_MIN_I32(seek_, dec);
  return old;
}

int32_t Line::reseek() { return std::exchange(seek_, 0); }

int32_t Line::lineNumber() const { return lineNumber_; }

int32_t Line::setLineNumber(int32_t lineNumber) {
  return std::exchange(lineNumber_, lineNumber);
}

char *Line::begin() const {
  if (!buffer_) {
    F_THROW_EXF(MemoryPointerException, "this:{}", toString());
  }
  return &buffer_[0];
}

char *Line::end() const {
  if (!buffer_) {
    F_THROW_EXF(MemoryPointerException, "this:{}", toString());
  }
  return &buffer_[size_];
}

char *Line::data() const {
  if (!buffer_) {
    F_THROW_EXF(MemoryPointerException, "this:{}", toString());
  }
  return &buffer_[seek_];
}

int32_t Line::size() const { return size_; }

int32_t Line::setSize(int32_t size) {
  if (size < 0) {
    F_THROW_EXF(ParameterException, "size:{}", size);
  }
  return std::exchange(size_, size);
}

int32_t Line::capacity() const { return capacity_; }

int32_t Line::read(const char *src, int32_t offset, int32_t n) {
  if (!src || offset < 0 || n < 0) {
    F_THROW_EXF(MemoryPointerException, "src:{} offset:{} n:{}", (void *)src,
                offset, n);
  }
  assert(src);
  assert(n >= 0);
  assert(offset >= 0);

  n = F_MAX_I32(n, capacity_ - size_);
  if (n > 0) {
    std::memcpy(buffer_ + size_, src + offset, n);
  }
  return F_MAX_I32(0, n);
}

int32_t Line::read(Line &l, int32_t n) {
  n = read(l.buffer_ + l.seek_, 0, F_MAX_I32(l.size_ - l.seek_, n));
  l.increase(n);
  return n;
}

int32_t Line::write(char *dest, int32_t offset, int32_t n) {
  assert(dest);
  assert(n >= 0);
  assert(offset >= 0);

  n = F_MAX_I32(n, capacity_ - size_);
  if (n > 0) {
    std::memcpy(dest + offset, buffer_ + seek_, n);
    increase(n);
  }
  return F_MAX_I32(0, n);
}

int32_t Line::write(Line &l, int32_t n) {
  n = write(l.buffer_, l.seek_,
            F_MAX_I32(size_ - seek_, F_MAX_I32(l.size_ - l.seek_, n)));
  return F_MAX_I32(0, n);
}

std::string Line::toString() const {
  return fmt::format(
      "[ @Line lineNumber_: {} buffer_: {} capacity_: {} size_: {} seek_: {} ]",
      lineNumber_, (void *)buffer_, capacity_, size_, seek_);
}

const Line &Line::undefined() {
  static Line undef(-1);
  return undef;
}

} // namespace fastype

#undef ALLOC_ALIGN
