// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "container/CycleBuffer.h"
#include "Exception.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <tuple>

#define CBT std::tuple<char *, int>
#define CBT_BUF(cbt) std::get<0>(cbt)
#define CBT_CAP(cbt) std::get<1>(cbt)

#define POSITIVE_DIRECTION (tail_ >= head_)
#define NEGATIVE_DIRECTION (tail_ < head_)
#define BUF_END (buf_ + capacity_)

// when positive direction
#define P_SIZE (tail_ - head_)
#define P_IN(p) (p >= head_ && p < tail_)

// when negative direction
#define N_LEFT_SIZE (tail_ - buf_)
#define N_RIGHT_SIZE (BUF_END - head_)
#define N_LEFT_IN(p) (p >= buf_ && p < tail_)
#define N_RIGHT_IN(p) (p >= head_ && p < BUF_END)

#define ALIGN(n) (n < 8 ? 8 : (n % 8 == 0 ? n : ((n / 8 + 1) * 8)))

namespace detail {

CycleBuffer::CycleBuffer()
    : buf_(nullptr), head_(nullptr), tail_(nullptr), capacity_(0) {}

CycleBuffer::~CycleBuffer() {
  release();
  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = 0;
}

int CycleBuffer::capacity() const { return capacity_; }

int CycleBuffer::size() const {
  return POSITIVE_DIRECTION ? P_SIZE : (N_LEFT_SIZE + N_RIGHT_SIZE);
}

bool CycleBuffer::empty() const { return head_ == tail_; }

bool CycleBuffer::full() const {
  return (head_ == buf_ && tail_ == BUF_END) || (tail_ + 1 == head_);
}

char *CycleBuffer::nextImpl(char *position) const {
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  if (NEGATIVE_DIRECTION && N_RIGHT_IN(position)) {
    return position + 1 == BUF_END ? buf_ : position + 1;
  } else {
    return position + 1;
  }
}

char *CycleBuffer::next(char *position) const { return nextImpl(position); }

const char *CycleBuffer::next(const char *position) const {
  return nextImpl((char *)position);
}

char *CycleBuffer::prevImpl(char *position) const {
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  if (NEGATIVE_DIRECTION && N_LEFT_IN(position)) {
    return position == buf_ ? BUF_END - 1 : position - 1;
  } else {
    return position - 1;
  }
}

char *CycleBuffer::prev(char *position) const { return prevImpl(position); }

const char *CycleBuffer::prev(const char *position) const {
  return prevImpl((char *)position);
}

char *CycleBuffer::begin() { return head_; }

const char *CycleBuffer::begin() const { return head_; }

char *CycleBuffer::rbegin() { return tail_ == buf_ ? BUF_END - 1 : tail_ - 1; }

const char *CycleBuffer::rbegin() const {
  return tail_ == buf_ ? BUF_END - 1 : tail_ - 1;
}

char *CycleBuffer::end() { return tail_; }

const char *CycleBuffer::end() const { return tail_; }

char *CycleBuffer::rend() { return head_ - 1; }

const char *CycleBuffer::rend() const { return head_ - 1; }

bool CycleBuffer::contain(const char *position) const {
  return POSITIVE_DIRECTION ? P_IN(position)
                            : (N_LEFT_IN(position) || N_RIGHT_IN(position));
}

void CycleBuffer::release() {
  if (buf_) {
    std::free(buf_);
    buf_ = nullptr;
  }
}

} // namespace detail

DynamicBuffer::DynamicBuffer(int capacity) {
  if (capacity > 0) {
    expand(ALIGN(capacity));
  }
}

int DynamicBuffer::write(const char *buf, int n) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!buf || !n) {
    return 0;
  }
  if (capacity() - size() <= n) {
    expand(ALIGN(capacity_ + n + 1) * 2);
  }
  if (full()) {
    return 0;
  }
  int writen = 0;
  if (POSITIVE_DIRECTION) {
    int fn = std::min(BUF_END - tail_, n);
    std::memcpy(tail_, buf, fn);
    tail_ = buf_;
    writen += fn;
    if (n > writen) {
      int sn = std::min(n - writen, head_ - buf_);
      std::memcpy(tail_, buf, sn);
      tail_ += sn;
      writen += sn;
    }
  } else {
    int fn = std::min(n, head_ - tail_);
    std::memcpy(tail_, buf, fn);
    tail_ = tail_ + fn;
    writen += fn;
  }
  return writen;
}

int DynamicBuffer::read(char *buf, int n) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!buf || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int readn = 0;
  if (POSITIVE_DIRECTION) {
    int fn = std::min(size(), n);
    std::memcpy(buf, head_, fn);
    head_ += fn;
    readn += fn;
  } else {
    int fn = std::min(BUF_END - head_, n);
    std::memcpy(buf, head_, fn);
    head_ = buf_;
    readn += fn;
    if (n > readn) {
      int sn = std::min(n - readn, size());
      std::memcpy(buf + readn, head_, sn);
      head_ += sn;
      readn += sn;
    }
  }
  return readn;
}

int DynamicBuffer::writeFile(FILE *fp, int n) {}

int DynamicBuffer::readFile(FILE *fp, int n) {}

int DynamicBuffer::expand(int n) {
  EX_ASSERT(n >= 0, "n {} >= 0", n);
  if (n <= capacity_) {
    return 0;
  }
  char *newbuf = (char *)std::malloc(n);
  if (!newbuf) {
    return -1;
  }
  if (POSITIVE_DIRECTION) {
    std::memcpy(newbuf, head_, P_SIZE);
    newbuf[P_SIZE] = 0;
  } else {
    std::memcpy(newbuf, head_, N_RIGHT_SIZE);
    std::memcpy(newbuf + N_RIGHT_SIZE, buf_, N_LEFT_SIZE);
    newbuf[N_LEFT_SIZE + N_RIGHT_SIZE] = 0;
  }
  release();
  buf_ = newbuf;
  capacity_ = n;
  head_ = buf_;
  tail_ = buf_ + size();
  return 0;
}
