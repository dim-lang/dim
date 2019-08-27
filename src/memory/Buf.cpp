// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "memory/Buf.h"
#include "Logging.h"
#include "Profile.h"
#include "boost/align/align_up.hpp"
#include "fmt/format.h"
#include <algorithm>
#include <utility>

#define F_ALIGN_UP 16

namespace fastype {

Buf::Buf() : data_(nullptr), size_(0), capacity_(0) {
  // F_DEBUGF("No Args Constructor:{}", toString());
}

Buf::Buf(int capacity) : Buf() {
  expand(capacity);
  // F_DEBUGF("Capacity Constructor:{}", toString());
}

Buf::~Buf() { release(); }

Buf::Buf(const Buf &other) : Buf() {
  if (other.data_) {
    expand(other.capacity_);
    std::memset(data_, 0, capacity_ * sizeof(char));
    std::memcpy(data_, other.data_, size_ * sizeof(char));
  }
  F_DEBUGF("Copy Constructor: {}", toString());
}

Buf &Buf::operator=(const Buf &other) {
  if (this == &other) {
    return *this;
  }
  expand(other.capacity_);
  std::memset(data_, 0, capacity_ * sizeof(char));
  std::memcpy(data_, other.data_, size_ * sizeof(char));
  F_DEBUGF("Copy Assign: {}", toString());
  return *this;
}

Buf::Buf(Buf &&other) : Buf() {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

Buf &Buf::operator=(Buf &&other) {
  if (this == &other) {
    return *this;
  }
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  return *this;
}

void Buf::expand(int capacity) {
  F_CHECKF(capacity > 0, "capacity {} > 0", capacity);
  // F_DEBUGF("capacity:{}", capacity);
  int newCapacity = std::max<int>(
      F_ALIGN_UP, (int)boost::alignment::align_up(capacity, F_ALIGN_UP));
  F_DEBUGF("capacity:{} newCapacity:{}", capacity, newCapacity);
  F_CHECKF(newCapacity >= 2 * capacity_, "newCapacity {} >= 2 * capacity_ {}",
           newCapacity, capacity_);
  char *newData = (char *)realloc(data_, newCapacity * sizeof(char));
  if (!newData) {
    return;
  }
  data_ = newData;
  capacity_ = newCapacity;
  F_DEBUGF("after expand:{}", toString());
}

bool Buf::empty() const { return size_ <= 0; }

bool Buf::full() const { return size_ >= capacity_; }

void Buf::clear() {
  if (data_) {
    std::memset(data_, 0, capacity_ * sizeof(char));
    size_ = 0;
  }
}

void Buf::release() {
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

void Buf::truncate(int start, int length) {
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

void Buf::trim(int length) {
  leftTrim(length);
  rightTrim(length);
}

void Buf::leftTrim(int length) {
  F_CHECKF(length >= 0, "length {} >= 0", length);
  if (!data_) {
    return;
  }
  length = std::min<int>(length, size_);
  std::memmove(data_, data_ + length, size_ - length);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

void Buf::rightTrim(int length) {
  F_CHECKF(length >= 0, "length {} >= 0", length);
  if (!data_) {
    return;
  }
  length = std::min<int>(length, size_);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

char *Buf::data() { return data_; }

const char *Buf::data() const { return data_; }

char &Buf::operator[](int index) {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < capacity_, "index {} < capacity_ {}", index, capacity_);
  return data_[index];
}

const char &Buf::operator[](int index) const {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < capacity_, "index {} < capacity_ {}", index, capacity_);
  return data_[index];
}

int Buf::size() const {
  F_CHECKF(size_ >= 0, "size_ {} >= 0", size_);
  return size_;
}

void Buf::setSize(int size) {
  F_CHECKF(size >= 0, "size {} >= 0", size);
  F_CHECKF(size_ >= 0, "size_ {} >= 0", size_);
  size_ = size;
}

void Buf::incSize(int update) {
  F_CHECKF(update > 0, "update {} > 0", update);
  F_CHECKF(size_ + update <= capacity_, "size_ {} + update {} <= capacity_ {}",
           size_, update, capacity_);
  size_ += update;
}

void Buf::decSize(int update) {
  F_CHECKF(update > 0, "update {} > 0", update);
  F_CHECKF(size_ - update >= 0, "size_ {} - update {} >= 0", size_, update);
  size_ -= update;
}

int Buf::capacity() const {
  F_CHECKF(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  return capacity_;
}

std::string Buf::toString() const {
  return fmt::format("[ @Buf data_:{} size_:{} capacity_:{} ]", (void *)data_,
                     size_, capacity_);
}

} // namespace fastype

#undef F_ALIGN_UP
