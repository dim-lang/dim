// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "Logging.h"
#include "boost/align/align_up.hpp"
#include "fmt/format.h"
#include <algorithm>
#include <utility>

#define ALIGN_UP 16

namespace fastype {

Buffer::Buffer() : Logging("Buffer"), data_(nullptr), size_(0), capacity_(0) {
  F_DEBUGF("No Args Constructor:{}", toString());
}

Buffer::Buffer(int capacity) : Buffer() {
  expand(capacity);
  F_DEBUGF("Capacity Constructor:{}", toString());
}

Buffer::~Buffer() { release(); }

// Buffer::Buffer(const Buffer &other) : Buffer() {
// if (other.data_) {
// expand(other.capacity_);
// std::memset(data_, 0, capacity_ * sizeof(char));
// std::memcpy(data_, other.data_, size_ * sizeof(char));
//}
// F_DEBUGF("Copy Constructor: {}", toString());
//}

// Buffer &Buffer::operator=(const Buffer &other) {
// if (this == &other) {
// return *this;
//}
// expand(other.capacity_);
// std::memset(data_, 0, capacity_ * sizeof(char));
// std::memcpy(data_, other.data_, size_ * sizeof(char));
// F_DEBUGF("Copy Assign: {}", toString());
// return *this;
//}

Buffer::Buffer(Buffer &&other) : Buffer() {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

Buffer &Buffer::operator=(Buffer &&other) {
  if (this == &other) {
    return *this;
  }
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  return *this;
}

void Buffer::expand(int capacity) {
  F_CHECKF(capacity > 0, "capacity {} > 0", capacity);
  F_DEBUGF("capacity:{}", capacity);
  int newCapacity = std::max<int>(
      ALIGN_UP, (int)boost::alignment::align_up(capacity, ALIGN_UP));
  F_DEBUGF("capacity:{} newCapacity:{}", capacity, newCapacity);
  F_CHECKF(newCapacity >= 2 * capacity_, "newCapacity {} >= 2 * capacity_ {}",
           newCapacity, capacity_);
  char *newData = new char[newCapacity];
  std::memset(newData, 0, newCapacity * sizeof(char));
  if (size_ > 0) {
    std::memcpy(newData, data_, size_ * sizeof(char));
  }
  std::swap(data_, newData);
  std::swap(capacity_, newCapacity);
  if (newData) {
    delete[] newData;
    newData = nullptr;
  }
}

bool Buffer::empty() const { return size_ <= 0; }

bool Buffer::full() const { return size_ >= capacity_; }

void Buffer::clear() {
  if (data_) {
    std::memset(data_, 0, capacity_ * sizeof(char));
    size_ = 0;
  }
}

void Buffer::release() {
  if (data_) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
  F_CHECKF(data_ == nullptr, "data_ {} == nullptr", (void *)data_);
  F_CHECKF(size_ == 0, "size_ {} == 0", size_);
  F_CHECKF(capacity_ == 0, "capacity_ {} == 0", capacity_);
}

void Buffer::truncate(int start, int length) {
  F_CHECKF(start >= 0, "start {} >= 0", start);
  F_CHECKF(length > 0, "length {} > 0", length);
  if (!data_) {
    return;
  }
  if (start > size_) {
    return;
  }
  length = std::min<int>(length, size_ - start);
  std::memmove(data_ + start, data_ + start + length, length);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

void Buffer::trim(int length) {
  leftTrim(length);
  rightTrim(length);
}

void Buffer::leftTrim(int length) {
  F_CHECKF(length >= 0, "length {} >= 0", length);
  if (!data_) {
    return;
  }
  length = std::min<int>(length, size_);
  std::memmove(data_, data_ + length, size_ - length);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

void Buffer::rightTrim(int length) {
  F_CHECKF(length >= 0, "length {} >= 0", length);
  if (!data_) {
    return;
  }
  length = std::min<int>(length, size_);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

char *Buffer::data() { return data_; }

const char *Buffer::data() const { return data_; }

char &Buffer::operator[](int index) {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < size_, "index {} < size_ {}", index, size_);
  return data_[index];
}

const char &Buffer::operator[](int index) const {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < size_, "index {} < size_ {}", index, size_);
  return data_[index];
}

int Buffer::size() const {
  F_CHECKF(size_ >= 0, "size_ {} >= 0", size_);
  return size_;
}

void Buffer::setSize(int size) {
  F_CHECKF(size >= 0, "size {} >= 0", size);
  F_CHECKF(size_ >= 0, "size_ {} >= 0", size_);
  size_ = size;
}

int Buffer::capacity() const {
  F_CHECKF(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  return capacity_;
}

std::string Buffer::toString() const {
  return fmt::format("[ @Buffer data_:{} size_:{} capacity_:{} ]",
                     (void *)data_, size_, capacity_);
}

} // namespace fastype

#undef ALIGN_UP
