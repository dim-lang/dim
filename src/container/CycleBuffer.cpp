// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "container/CycleBuffer.h"
#include "Exception.h"
#include "fmt/format.h"
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
#define P_LEFT_SIZE (head_ - buf_)
#define P_RIGHT_SIZE (BUF_END - tail_)
#define P_IN(p) (p >= head_ && p < tail_)

// when negative direction
#define N_LEFT_SIZE (tail_ - buf_)
#define N_RIGHT_SIZE (BUF_END - head_)
#define N_LEFT_IN(p) (p >= buf_ && p < tail_)
#define N_RIGHT_IN(p) (p >= head_ && p < BUF_END)

#define ALIGN(n) (n < 8 ? 8 : (n % 8 == 0 ? n : ((n / 8 + 1) * 8)))
#define MIN(a, b) (std::min<int>(a, b))

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

char *CycleBuffer::nextImpl(char *position, int distance) const {
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  EX_ASSERT(distance >= 0, "distance {} < 0", distance);
  char *np = position + distance;
  if (POSITIVE_DIRECTION) {
    return np >= tail_ ? (char *)end() : np;
  } else {
    if (N_LEFT_IN(position)) {
      return np >= tail_ ? (char *)end() : np;
    } else {
      EX_ASSERT(N_RIGHT_IN(position), "position {} must in right part {}",
                (void *)position, toString());
      if (np < BUF_END) {
        return np;
      }
      np = (char *)buf_ + (distance - (BUF_END - position));
      return np >= tail_ ? (char *)end() : np;
    }
  }
}

char *CycleBuffer::next(char *position, int distance) const {
  return nextImpl(position, distance);
}

const char *CycleBuffer::next(const char *position, int distance) const {
  return nextImpl((char *)position, distance);
}

char *CycleBuffer::prevImpl(char *position, int distance) const {
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  EX_ASSERT(distance >= 0, "distance {} < 0", distance);
  char *np = position - distance;
  if (POSITIVE_DIRECTION) {
    return np < head_ ? (char *)rend() : np;
  } else {
    if (N_LEFT_IN(position)) {
      if (np < buf_) {
        return np;
      }
      np = (char *)BUF_END - (distance - (position - buf_));
      return np < head_ ? (char *)rend() : np;
    } else {
      EX_ASSERT(N_RIGHT_IN(position), "position {} must in right part {}",
                (void *)position, toString());
      return np < head_ ? (char *)rend() : np;
    }
  }
}

char *CycleBuffer::prev(char *position, int distance) const {
  return prevImpl(position, distance);
}

const char *CycleBuffer::prev(const char *position, int distance) const {
  return prevImpl((char *)position, distance);
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

std::string CycleBuffer::toString() const {
  return fmt::format("buf_:{}, head_:{}, tail_:{}, capacity_:{}", (void *)buf_,
                     (void *)head_, (void *)tail_, capacity_);
}

void CycleBuffer::release() {
  if (buf_) {
    std::free(buf_);
    buf_ = nullptr;
  }
}

int CycleBuffer::write(char *buf, int n) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!buf || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int writen = 0;
  if (POSITIVE_DIRECTION) {
    int fn = MIN(size(), n);
    std::memcpy(buf, head_, fn);
    head_ = next(head_, fn);
    writen += fn;
  } else {
    int fn = MIN(BUF_END - head_, n);
    std::memcpy(buf, head_, fn);
    head_ = next(head_, fn);
    writen += fn;
    if (n > writen) {
      int sn = MIN(n - writen, N_LEFT_SIZE);
      std::memcpy(buf + writen, head_, sn);
      head_ = next(head_, fn);
      writen += sn;
    }
  }
  return writen;
}

int CycleBuffer::fwrite(FILE *fp, int n) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!fp || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int writen = 0;
  if (POSITIVE_DIRECTION) {
    int fn = MIN(size(), n);
    size_t fnr = std::fwrite(head_, 1, fn, fp);
    head_ = next(head_, fnr);
    writen += fnr;
  } else {
    int fn = MIN(BUF_END - head_, n);
    size_t fnr = std::fwrite(head_, 1, fn, fp);
    head_ = next(head_, fnr);
    writen += fnr;
    // fnr == fn indicate write success
    if (fnr == fn && n > writen) {
      int sn = MIN(n - writen, N_LEFT_SIZE);
      size_t snr = std::fwrite(buf_, 1, sn, fp);
      head_ = next(head_, snr);
      writen += snr;
    }
  }
  return writen;
}

int CycleBuffer::fwrite(FILE *fp) {
  int n = 0;
  int tmp = 0;
  do {
    tmp = fwrite(fp, 1024);
    n += tmp;
  } while (tmp > 0);
  return n;
}

int CycleBuffer::read(const char *buf, int n) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!buf || !n) {
    return 0;
  }
  if (dynamic()) {
    if (capacity() - size() < n) {
      expand(ALIGN(capacity_ + n + 1) * 2);
    }
  }
  if (full()) {
    return 0;
  }
  int readn = 0;
  if (POSITIVE_DIRECTION) {
    int fn = MIN(BUF_END - tail_, n);
    std::memcpy(tail_, buf, fn);
    tail_ = buf_;
    readn += fn;
    if (n > readn) {
      int sn = MIN(n - readn, head_ - buf_);
      std::memcpy(tail_, buf, sn);
      tail_ += sn;
      readn += sn;
    }
  } else {
    int fn = MIN(n, head_ - tail_);
    std::memcpy(tail_, buf, fn);
    tail_ += fn;
    readn += fn;
  }
  return readn;
}

int CycleBuffer::fread(FILE *fp, int n) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!fp || !n) {
    return 0;
  }
  if (dynamic()) {
    if (capacity() - size() < n) {
      expand(ALIGN(capacity_ + n + 1) * 2);
    }
  }
  if (full()) {
    return 0;
  }
  int readn = 0;
  if (POSITIVE_DIRECTION) {
    int fn = MIN(BUF_END - tail_, n);
    size_t fnr = std::fread(tail_, 1, fn, fp);
    tail_ += fnr;
    readn += fnr;
    if (fn == fnr && n > readn) {
      EX_ASSERT(tail_ == BUF_END, "tail_ {} == BUF_END {}", (void *)tail_,
                (void *)BUF_END);
      int sn = MIN(n - readn, head_ - buf_);
      size_t snr = std::fread(buf_, 1, sn, fp);
      tail_ = buf_ + snr;
      readn += snr;
    }
  } else {
    int fn = MIN(n, head_ - tail_);
    size_t fnr = std::fread(tail_, 1, fn, fp);
    tail_ += fnr;
    readn += fnr;
  }
  return readn;
}

int CycleBuffer::fread(FILE *fp) {
  int n = 0;
  int tmp = 0;
  do {
    tmp = fread(fp, 1024);
    n += tmp;
  } while (tmp > 0);
  return n;
}

} // namespace detail

DynamicBuffer::DynamicBuffer(int capacity) {
  if (capacity > 0) {
    expand(ALIGN(capacity));
  }
}

std::string DynamicBuffer::toString() const {
  return fmt::format("[@DynamicBuffer {}]", detail::CycleBuffer::toString());
}

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

bool DynamicBuffer::dynamic() const { return true; }

FixedBuffer::FixedBuffer(int capacity) {
  capacity = ALIGN(capacity);
  buf_ = (char *)std::malloc(capacity);
  if (!buf_) {
    return;
  }
  capacity_ = capacity;
  head_ = buf_;
  tail_ = buf_;
}

std::string FixedBuffer::toString() const {
  return fmt::format("[@FixedBuffer {}]", detail::CycleBuffer::toString());
}

int FixedBuffer::expand(int n) { return 0; }

bool FixedBuffer::dynamic() const { return false; }
