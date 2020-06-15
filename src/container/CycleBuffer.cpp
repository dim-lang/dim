// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "container/CycleBuffer.h"
#include "Exception.h"
#include "fmt/format.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <tuple>

#define ALIGN(n) (n < 8 ? 8 : (n % 8 == 0 ? n : ((n / 8 + 1) * 8)))
#define MIN(a, b) (std::min<int>(a, b))
#define BUF_SIZE 1024

namespace detail {

template <unsigned int D> char *CycleBuffer<D>::bufEnd() {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return buf_ + capacity();
}

template <unsigned int D> const char *CycleBuffer<D>::bufEnd() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return buf_ + capacity();
}

// is positive direction
template <unsigned int D> bool CycleBuffer<D>::positive() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return capacity() > 0 && !full() && tail_ >= head_;
}

template <unsigned int D> long CycleBuffer<D>::pSize() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return tail_ - head_;
}

template <unsigned int D>
bool CycleBuffer<D>::pContain(const char *position) const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return position >= head_ && position < tail_;
}

template <unsigned int D> long CycleBuffer<D>::nLeftSize() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return tail_ - buf_;
}

template <unsigned int D> long CycleBuffer<D>::nRightSize() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return bufEnd() - head_;
}

template <unsigned int D>
bool CycleBuffer<D>::nLeftContain(const char *position) const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return position >= buf_ && position < tail_;
}

template <unsigned int D>
bool CycleBuffer<D>::nRightContain(const char *position) const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return position >= head_ && position < bufEnd();
}

template <unsigned int D>
CycleBuffer<D>::CycleBuffer()
    : buf_(nullptr), head_(nullptr), tail_(nullptr), capacity_(0), size_(0) {}

template <unsigned int D> CycleBuffer<D>::~CycleBuffer() { reset(); }

template <unsigned int D> int CycleBuffer<D>::capacity() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return capacity_;
}

template <unsigned int D> int CycleBuffer<D>::size() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return size_;
}

template <unsigned int D> bool CycleBuffer<D>::empty() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return size() == 0;
}

template <unsigned int D> bool CycleBuffer<D>::full() const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return size() == capacity();
}

template <unsigned int D> void CycleBuffer<D>::reset() {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  release();
  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = 0;
  size_ = 0;
}

template <unsigned int D>
char *CycleBuffer<D>::nextImpl(char *position, int distance) const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  EX_ASSERT(distance >= 0, "distance {} < 0", distance);
  char *np = position + distance;
  if (positive()) {
    return np >= tail_ ? (char *)end() : np;
  } else {
    if (nLeftContain(position)) {
      return np >= tail_ ? (char *)end() : np;
    } else {
      EX_ASSERT(nRightContain(position), "position {} must in right part {}",
                (void *)position, toString());
      if (np < bufEnd()) {
        return np;
      }
      np = (char *)buf_ + (distance - (bufEnd() - position));
      return np >= tail_ ? (char *)end() : np;
    }
  }
}

template <unsigned int D>
char *CycleBuffer<D>::next(char *position, int distance) const {
  return nextImpl(position, distance);
}

template <unsigned int D>
const char *CycleBuffer<D>::next(const char *position, int distance) const {
  return nextImpl((char *)position, distance);
}

template <unsigned int D>
char *CycleBuffer<D>::prevImpl(char *position, int distance) const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  EX_ASSERT(distance >= 0, "distance {} < 0", distance);
  char *np = position - distance;
  if (positive()) {
    return np < head_ ? (char *)rend() : np;
  } else {
    if (nLeftContain(position)) {
      if (np < buf_) {
        return np;
      }
      np = (char *)bufEnd() - (distance - (position - buf_));
      return np < head_ ? (char *)rend() : np;
    } else {
      EX_ASSERT(nRightContain(position), "position {} must in right part {}",
                (void *)position, toString());
      return np < head_ ? (char *)rend() : np;
    }
  }
}

template <unsigned int D>
char *CycleBuffer<D>::prev(char *position, int distance) const {
  return prevImpl(position, distance);
}

template <unsigned int D>
const char *CycleBuffer<D>::prev(const char *position, int distance) const {
  return prevImpl((char *)position, distance);
}

template <unsigned int D> char *CycleBuffer<D>::begin() { return head_; }

template <unsigned int D> const char *CycleBuffer<D>::begin() const {
  return head_;
}

template <unsigned int D> char *CycleBuffer<D>::rbegin() {
  return tail_ == buf_ ? bufEnd() - 1 : tail_ - 1;
}

template <unsigned int D> const char *CycleBuffer<D>::rbegin() const {
  return tail_ == buf_ ? bufEnd() - 1 : tail_ - 1;
}

template <unsigned int D> char *CycleBuffer<D>::end() { return tail_; }

template <unsigned int D> const char *CycleBuffer<D>::end() const {
  return tail_;
}

template <unsigned int D> char *CycleBuffer<D>::rend() {
  return head_ == buf_ ? bufEnd() - 1 : head_ - 1;
}

template <unsigned int D> const char *CycleBuffer<D>::rend() const {
  return head_ == buf_ ? bufEnd() - 1 : head_ - 1;
}

template <unsigned int D>
bool CycleBuffer<D>::contain(const char *position) const {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  return positive() ? pContain(position)
                    : (nLeftContain(position) || nRightContain(position));
}

template <unsigned int D> std::string CycleBuffer<D>::toString() const {
  return fmt::format("buf_:{}, head_:{}, tail_:{}, capacity_:{}", (void *)buf_,
                     (void *)head_, (void *)tail_, capacity_);
}

template <unsigned int D> void CycleBuffer<D>::release() {
  if (buf_) {
    std::free(buf_);
    buf_ = nullptr;
  }
}

#define writeinc(n)                                                            \
  do {                                                                         \
    head_ = next(head_, n);                                                    \
    writen += n;                                                               \
    size_ -= n;                                                                \
  } while (0)

template <unsigned int D> int CycleBuffer<D>::write(char *buf, int n) {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!buf || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int writen = 0;
  if (positive()) {
    int fn = MIN(size(), n);
    std::memcpy(buf, head_, fn);
    writeinc(fn);
    EX_ASSERT(head_ <= tail_, "head_ {} <= tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  } else {
    int fn = MIN(bufEnd() - head_, n);
    std::memcpy(buf, head_, fn);
    writeinc(fn);
    EX_ASSERT(head_ == buf_ || head_ < bufEnd(),
              "head_ {} == buf_ {} or head_ {} < bufEnd {}", (void *)head_,
              (void *)buf_, (void *)head_, (void *)bufEnd());
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
    if (n > writen) {
      EX_ASSERT(head_ == buf_, "head_ {} == buf_ {}", (void *)head_,
                (void *)buf_);
      int sn = MIN(n - writen, nLeftSize());
      std::memcpy(buf + writen, head_, sn);
      writeinc(fn);
      EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
      EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_,
                capacity_);
      EX_ASSERT(head_ <= tail_, "head_ {} <= tail_ {}", (void *)head_,
                (void *)tail_);
    }
  }
  return writen;
}

template <unsigned int D> int CycleBuffer<D>::writefile(FILE *fp, int n) {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!fp || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int writen = 0;
  if (positive()) {
    int fn = MIN(size(), n);
    size_t fnr = std::fwrite(head_, 1, fn, fp);
    writeinc(fnr);
    EX_ASSERT(head_ <= tail_, "head_ {} <= tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  } else {
    int fn = MIN(bufEnd() - head_, n);
    size_t fnr = std::fwrite(head_, 1, fn, fp);
    writeinc(fnr);
    EX_ASSERT(head_ == buf_ || head_ < bufEnd(),
              "head_ {} == buf_ {} or head_ {} < bufEnd {}", (void *)head_,
              (void *)buf_, (void *)head_, (void *)bufEnd());
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
    if (n > writen) {
      // fnr == fn indicate write success
      EX_ASSERT(fnr == fn, "fnr {} == fn {}", fnr, fn);
      EX_ASSERT(head_ == buf_, "head_ {} == buf_ {}", (void *)head_,
                (void *)buf_);
      int sn = MIN(n - writen, nLeftSize());
      size_t snr = std::fwrite(buf_, 1, sn, fp);
      writeinc(snr);
      EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
      EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_,
                capacity_);
      EX_ASSERT(head_ <= tail_, "head_ {} <= tail_ {}", (void *)head_,
                (void *)tail_);
    }
  }
  return writen;
}

template <unsigned int D> int CycleBuffer<D>::writefile(FILE *fp) {
  int n = 0;
  int tmp = 0;
  do {
    tmp = writefile(fp, BUF_SIZE);
    n += tmp;
  } while (tmp > 0);
  return n;
}

#define readinc(n)                                                             \
  do {                                                                         \
    tail_ += n;                                                                \
    readn += n;                                                                \
    size_ += n;                                                                \
  } while (0)

template <unsigned int D> int CycleBuffer<D>::read(const char *buf, int n) {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!buf || !n) {
    return 0;
  }
  if (D) {
    if (capacity() - size() < n) {
      expand(ALIGN(((capacity() + n) > (capacity() * 2)) ? (capacity() + n)
                                                         : (capacity() * 2)));
    }
  }
  if (full()) {
    return 0;
  }
  int readn = 0;
  if (positive()) {
    int fn = MIN(bufEnd() - tail_, n);
    std::memcpy(tail_, buf, fn);
    readinc(fn);
    if (tail_ == bufEnd()) {
      tail_ = buf_;
    }
    EX_ASSERT(head_ <= tail_, "head_ {} <= tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
    if (n > readn) {
      EX_ASSERT(tail_ == buf_, "tail_ {} == buf_ {}", (void *)tail_,
                (void *)buf_);
      int sn = MIN(n - readn, head_ - tail_);
      std::memcpy(tail_, buf, sn);
      readinc(sn);
      EX_ASSERT(head_ >= tail_, "head_ {} >= tail_ {}", (void *)head_,
                (void *)tail_);
      EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
      EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_,
                capacity_);
    }
  } else {
    int fn = MIN(n, head_ - tail_);
    std::memcpy(tail_, buf, fn);
    readinc(fn);
    EX_ASSERT(head_ >= tail_, "head_ {} >= tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  }
  return readn;
}

template <unsigned int D> int CycleBuffer<D>::readfile(FILE *fp, int n) {
  EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!fp || !n) {
    return 0;
  }
  if (D) {
    if (capacity() - size() <= n) {
      expand(ALIGN(((capacity() + n) > (capacity() * 2)) ? (capacity() + n)
                                                         : (capacity() * 2)));
    }
  }
  if (full()) {
    return 0;
  }
  int readn = 0;
  if (positive()) {
    int fn = MIN(bufEnd() - tail_ - 1, n);
    size_t fnr = std::fread(tail_, 1, fn, fp);
    readinc(fnr);
    EX_ASSERT(head_ != tail_, "head_ {} != tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(head_ >= tail_, "head_ {} >= tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
    if (tail_ == bufEnd()) {
      tail_ = buf_;
    }
    EX_ASSERT(head_ <= tail_, "head_ {} <= tail_ {}", (void *)head_,
              (void *)tail_);
    if (n > readn) {
      EX_ASSERT(fn == fnr, "fn {} == fnr {}", fn, fnr);
      EX_ASSERT(tail_ == buf_, "tail_ {} == buf_ {}", (void *)tail_,
                (void *)buf_);
      int sn = MIN(n - readn, head_ - tail_);
      size_t snr = std::fread(tail_, 1, sn, fp);
      readinc(snr);
      EX_ASSERT(head_ >= tail_, "head_ {} >= tail_ {}", (void *)head_,
                (void *)tail_);
      EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
      EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_,
                capacity_);
    }
  } else {
    int fn = MIN(n, head_ - tail_);
    size_t fnr = std::fread(tail_, 1, fn, fp);
    readinc(fnr);
    EX_ASSERT(head_ >= tail_, "head_ {} >= tail_ {}", (void *)head_,
              (void *)tail_);
    EX_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    EX_ASSERT(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
    EX_ASSERT(size_ <= capacity_, "size_ {} <= capacity_ {}", size_, capacity_);
  }
  return readn;
}

template <unsigned int D> int CycleBuffer<D>::readfile(FILE *fp) {
  int n = 0;
  int tmp = 0;
  do {
    tmp = readfile(fp, BUF_SIZE);
    n += tmp;
  } while (tmp > 0);
  return n;
}

template <unsigned int D> int CycleBuffer<D>::expand(int n) {
  EX_ASSERT(n >= 0, "n {} >= 0", n);
  if (n <= capacity_) {
    return 0;
  }
  char *newbuf = (char *)std::malloc(n);
  if (!newbuf) {
    return -1;
  }
  int sz = size();
  if (positive()) {
    std::memcpy(newbuf, head_, pSize());
  } else {
    std::memcpy(newbuf, head_, nRightSize());
    std::memcpy(newbuf + nRightSize(), buf_, nLeftSize());
  }
  release();
  buf_ = newbuf;
  capacity_ = n;
  head_ = buf_;
  tail_ = buf_ + sz;
  return 0;
}

} // namespace detail

DynamicBuffer::DynamicBuffer(int capacity) {
  if (capacity > 0) {
    expand(ALIGN(capacity));
  }
}

std::string DynamicBuffer::toString() const {
  return fmt::format("[@DynamicBuffer {}]", detail::CycleBuffer<1>::toString());
}

FixedBuffer::FixedBuffer(int capacity) {
  // precise capacity
  if (capacity > 0) {
    expand(capacity);
  }
}

std::string FixedBuffer::toString() const {
  return fmt::format("[@FixedBuffer {}]", detail::CycleBuffer<0>::toString());
}
