// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "boost/align/align_up.hpp"
#include "fmt/format.h"
#include <utility>

#define ALIGNMENT 16

namespace fastype {

Buffer::Buffer() : data_(nullptr), size_(0), capacity_(-1) {}

Buffer::Buffer(Buffer &&other)
    : data_(std::move(other.data_)), size_(std::move(other.size_)),
      capacity_(std::move(other.capacity_)) {}

Buffer &Buffer::operator=(Buffer &&other) {
  if (this == &other) {
    return *this;
  }
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  return *this;
}

Buffer::~Buffer() {
  if (data_) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
}

char *&Buffer::data() { return data_; }

void Buffer::setData(char *data) { data_ = data; }

int &Buffer::size() { return size_; }

void Buffer::setSize(int size) { size_ = size; }

int &Buffer::capacity() { return capacity_; }

void Buffer::setCapacity(int capacity) { capacity_ = capacity; }

int Buffer::expand(int n) {
  int newCapacity = boost::align_up(n, ALIGNMENT);
  char *newData = new char[newCapacity];
}

virtual std::string toString() const;

} // namespace fastype

#undef ALIGNMENT
