// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Block.h"
#include "Logging.h"
#include <cstdlib>
#include <cstring>

#define F_ALLOC_UNIT 8

namespace fastype {

Block::Block() : buf_(nullptr), start_(0), end_(0), capacity_(0) {}

Block::Block(int capacity) : Block() { expand(capacity); }

Block::~Block() { release(); }

Block::Block(const Block &s) : Block(s.head(), s.size()) {}

Block::Block(char c) : Block(&c, 1) {}

Block::Block(const char *s, int n) : Block() {
  F_CHECK(n >= 0, "n {} >= 0", n);
  if (n > 0) {
    expand(n);
    std::memcpy(buf_, s, n);
    end_ = n;
  }
}

Block::Block(const std::string &s) : Block(s.data(), (int)s.length()) {}

Block &Block::operator=(const Block &s) {
  if (this == &s) {
    return *this;
  }
  if (s.size() > 0) {
    expand(s.size());
    std::memcpy(buf_, s.head(), s.size());
  }
  end_ = s.size();
  return *this;
}

Block::Block(Block &&s) : Block() { swap(s); }

Block &Block::operator=(Block &&s) { return this == &s ? *this : swap(s); }

Block &Block::swap(Block &s) {
  std::swap(buf_, s.buf_);
  std::swap(start_, s.start_);
  std::swap(end_, s.end_);
  std::swap(capacity_, s.capacity_);
  return *this;
}

Block &Block::expand(int capacity) {
  // do nothing
  if (capacity <= 0) {
    return *this;
  }
  if (capacity <= capacity_) {
    return *this;
  }

  capacity = std::max(F_ALLOC_UNIT, capacity);
  char *nb = new char[capacity];
  int sz = size();
  char *ob = buf_;

  if (buf_) {
    // case 1: memory disorder
    if (!memoryInOrder()) {
      int c1 = capacity_ - start_;
      std::memcpy(nb, buf_ + start_, c1);
      std::memcpy(nb + c1, buf_, end_);
    } else {
      // case 2: memory in order
      std::memcpy(nb, buf_ + start_, sz);
    }
  }

  // after expantion, data is aligned with the beginning of buffer
  buf_ = nb;
  start_ = 0;
  end_ = sz;
  capacity_ = capacity;

  if (ob) {
    delete[] ob;
  }
  return *this;
}

Block &Block::clear() {
  if (buf_) {
    std::memset(buf_, 0, capacity_);
  }
  start_ = 0;
  end_ = 0;
  return *this;
}

Block &Block::release() {
  if (buf_) {
    delete[] buf_;
    buf_ = nullptr;
  }
  start_ = 0;
  end_ = 0;
  capacity_ = 0;
  return *this;
}

Block &Block::concat(const Block &s) { return concat(s.head(), s.size()); }

Block &Block::concat(const char *s, int n) {
  // if tail capacity has no more capacity, expand new memory
  if (tailCapacity() < n) {
    expand(std::max(capacity() + n + 1, capacity() * 2 + F_ALLOC_UNIT));
  }

  std::memcpy(buf_ + end_, s, n);
  end_ += n;
  return *this;
}

Block &Block::concat(const std::string &s) {
  return concat(s.data(), (int)s.length());
}

Block &Block::operator+=(const Block &s) { return concat(s); }

Block &Block::operator+=(const std::string &s) { return concat(s); }

Block &Block::concatHead(const Block &s) {
  return concatHead(s.head(), s.size());
}

Block &Block::concatHead(const std::string &s) {
  return concatHead(s.data(), (int)s.length());
}

Block &Block::concatHead(const char *s, int n) {
  if (!s || n <= 0) {
    return *this;
  }
  F_CHECK(n > 0, "n {} > 0", n);
  // if head capacity has no more capacity, expand new memory
  if (headCapacity() < n) {
    expand(std::max(capacity() + n + 1, capacity() * 2 + F_ALLOC_UNIT));
    std::memmove(buf_ + n, buf_, end_);
    std::memcpy(buf_, s, n);
    end_ += n;
  } else {
    std::memcpy(buf_ + start_ - n, s, n);
    start_ -= n;
  }
  return *this;
}

Block &Block::truncate(int start) {
  int n = size() - start;
  return truncate(start, n);
}

Block &Block::truncate(int start, int n) {
  int sz = size();
  removeHead(start);
  removeTail(sz - (start + n));
  return *this;
}

Block &Block::removeHead(int n) {
  n = std::min(n, size());

  if (memoryInOrder()) {
    start_ += n;
  } else {
    if (headDistance() >= n) {
      start_ += n;
    } else {
      start_ = n - headDistance();
    }
  }

  return *this;
}

Block &Block::removeTail(int n) {
  n = std::min(n, size());

  if (memoryInOrder()) {
    end_ -= n;
  } else {
    if (tailDistance() >= n) {
      end_ -= n;
    } else {
      end_ = capacity_ - (n - tailDistance());
    }
  }

  return *this;
}

std::string Block::toString() const {
  return fmt::format(
      "[ @Block buf_:{} buf_ ptr:{}, start_:{} end_:{} capacity_:{} ]",
      buf_ ? buf_ : "null", (void *)buf_, start_, end_, capacity_);
}

char *Block::head() { return buf_ ? (buf_ + start_) : nullptr; }

const char *Block::head() const { return buf_ ? (buf_ + start_) : nullptr; }

char *Block::tail() { return buf_ ? (buf_ + end_) : nullptr; }

const char *Block::tail() const { return buf_ ? (buf_ + end_) : nullptr; }

char &Block::operator[](int pos) { return buf_[start_ + pos]; }

const char &Block::operator[](int pos) const { return buf_[start_ + pos]; }

char &Block::at(int pos) { return buf_[start_ + pos]; }

const char &Block::at(int pos) const { return buf_[start_ + pos]; }

bool Block::empty() const { return end_ == start_; }

int Block::size() const {
  return memoryInOrder() ? (end_ - start_) : (end_ + capacity_ - start_);
}

int Block::capacity() const { return capacity_; }

int Block::leftCapacity() const {
  return memoryInOrder() ? (start_ + capacity_ - end_)
                         : (end_ + capacity_ - start_);
}

namespace detail {

class BlockComparator {
public:
};

} // namespace detail

bool Block::operator==(const Block &s) const { return compare(s) == 0; }

bool Block::operator!=(const Block &s) const { return compare(s) != 0; }

bool Block::operator>(const Block &s) const { return compare(s) > 0; }

bool Block::operator>=(const Block &s) const { return compare(s) >= 0; }

bool Block::operator<(const Block &s) const { return compare(s) < 0; }

bool Block::operator<=(const Block &s) const { return compare(s) <= 0; }

int Block::compare(const Block &s) const {
  if (size() != s.size()) {
    return size() - s.size();
  }
  return std::memcmp(head(), s.head(), size());
}

bool Block::memoryInOrder() const { return end_ >= start_; }

int Block::tailCapacity() const {
  return memoryInOrder() ? capacity_ - end_ : start_ - end_;
}

int Block::headCapacity() const {
  return memoryInOrder() ? start_ : start_ - end_;
}

int Block::tailDistance() const {
  return memoryInOrder() ? capacity_ - end_ : end_;
}

int Block::headDistance() const {
  return memoryInOrder() ? start_ : capacity_ - start_;
}

} // namespace fastype

#undef F_ALLOC_UNIT
