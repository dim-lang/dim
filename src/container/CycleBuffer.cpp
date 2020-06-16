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
  return buf_ + capacity_;
}

template <unsigned int D> const char *CycleBuffer<D>::bufEnd() const {
  return buf_ + capacity_;
}

// is positive direction
template <unsigned int D> bool CycleBuffer<D>::positive() const {
  return tail_ >= head_;
}

template <unsigned int D> long CycleBuffer<D>::pSize() const {
  EX_ASSERT(positive(), "tail_ {} >= head_ {}, positive: {}, toString: {}",
            (void *)tail_, (void *)head_, positive(), toString());
  return tail_ - head_;
}

template <unsigned int D>
bool CycleBuffer<D>::pContain(const char *position) const {
  EX_ASSERT(positive(), "tail_ {} >= head_ {}, positive: {}, toString: {}",
            (void *)tail_, (void *)head_, positive(), toString());
  return position >= head_ && position < tail_;
}

template <unsigned int D> long CycleBuffer<D>::nLeftSize() const {
  EX_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, toString: {}",
            (void *)tail_, (void *)head_, positive(), toString());
  EX_ASSERT(tail_ >= buf_, "tail_ {} >= buf_ {}", (void *)tail_, (void *)buf_);
  return tail_ - buf_;
}

template <unsigned int D> long CycleBuffer<D>::nRightSize() const {
  EX_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, toString: {}",
            (void *)tail_, (void *)head_, positive(), toString());
  EX_ASSERT(bufEnd() >= head_, "bufEnd {} >= head_ {}", (void *)bufEnd(),
            (void *)head_);
  return bufEnd() - head_;
}

template <unsigned int D>
bool CycleBuffer<D>::nLeftContain(const char *position) const {
  EX_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, toString: {}",
            (void *)tail_, (void *)head_, positive(), toString());
  EX_ASSERT(tail_ >= buf_, "tail_ {} >= buf_ {}", (void *)tail_, (void *)buf_);
  return position >= buf_ && position < tail_;
}

template <unsigned int D>
bool CycleBuffer<D>::nRightContain(const char *position) const {
  EX_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, toString: {}",
            (void *)tail_, (void *)head_, positive(), toString());
  EX_ASSERT(bufEnd() >= head_, "bufEnd {} >= head_ {}", (void *)bufEnd(),
            (void *)head_);
  return position >= head_ && position < bufEnd();
}

template <unsigned int D>
CycleBuffer<D>::CycleBuffer()
    : buf_(nullptr), head_(nullptr), tail_(nullptr), capacity_(0) {}

template <unsigned int D> CycleBuffer<D>::~CycleBuffer() { reset(); }

template <unsigned int D> int CycleBuffer<D>::capacity() const {
  if (buf_) {
    EX_ASSERT(capacity_ - 1 >= 0, "capacity_ - 1 {} >= 0", capacity_ - 1);
  } else {
    EX_ASSERT(capacity_ == 0 && size() == 0, "capacity_ {} == size {} == 0",
              capacity_, size());
  }
  return capacity_ > 0 ? capacity_ - 1 : 0;
}

template <unsigned int D> int CycleBuffer<D>::size() const {
  return positive() ? pSize() : (nLeftSize() + nRightSize());
}

template <unsigned int D> bool CycleBuffer<D>::empty() const {
  return size() == 0;
}

template <unsigned int D> bool CycleBuffer<D>::full() const {
  return size() == capacity();
}

template <unsigned int D> void CycleBuffer<D>::reset() {
  EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
  release();
  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = 0;
}

template <unsigned int D>
char *CycleBuffer<D>::nextImpl(char *position, int distance) const {
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  EX_ASSERT(distance >= 0, "distance {} < 0", distance);
  if (position == end() || !position) {
    return (char *)end();
  }
  char *np = position + distance;
  if (positive()) {
    return np > tail_ ? (char *)end() : np;
  } else {
    if (nLeftContain(position)) {
      return np > tail_ ? (char *)end() : np;
    } else {
      EX_ASSERT(nRightContain(position), "position {} must in right part {}",
                (void *)position, toString());
      if (np < bufEnd()) {
        return np;
      }
      np = (char *)buf_ + (distance - (bufEnd() - position));
      return np > tail_ ? (char *)end() : np;
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
  EX_ASSERT(contain(position), "position {} out of range {}", (void *)position,
            toString());
  EX_ASSERT(distance >= 0, "distance {} >= 0", distance);
  if (position == rend() || !position) {
    return (char *)rend();
  }
  char *np = position - distance;
  if (positive()) {
    return np < head_ ? (char *)rend() : np;
  } else {
    if (nLeftContain(position)) {
      if (np >= buf_) {
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

template <unsigned int D> char *CycleBuffer<D>::rend() { return head_ - 1; }

template <unsigned int D> const char *CycleBuffer<D>::rend() const {
  return head_ - 1;
}

template <unsigned int D>
bool CycleBuffer<D>::contain(const char *position) const {
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

static int writeMemHandler(void *src, void *buf, int n) {
  std::memcpy(src, buf, n);
  return n;
}

static int writeFileHandler(void *src, void *buf, int n) {
  size_t r = std::fwrite(buf, 1, n, (FILE *)src);
  return (int)r;
}

#define WINC(n)                                                                \
  do {                                                                         \
    head_ = next(head_, n);                                                    \
    writen += n;                                                               \
  } while (0)

template <unsigned int D>
int CycleBuffer<D>::writeImpl(void *src, int n,
                              int (*writeHandler)(void *, void *, int)) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!src || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int writen = 0;
  if (positive()) {
    int fn = MIN(size(), n);
    int fnr = writeHandler(src, head_, fn);
    WINC(fnr);
    EX_ASSERT(positive(), "positive:{}", positive());
    EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
  } else {
    int fn = MIN(bufEnd() - head_, n);
    int fnr = writeHandler(src, head_, fn);
    WINC(fnr);
    EX_ASSERT(head_ == buf_ || head_ < bufEnd(),
              "head_ {} == buf_ {} or head_ {} < bufEnd {}", (void *)head_,
              (void *)buf_, (void *)head_, (void *)bufEnd());
    EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
    if (n > writen) {
      EX_ASSERT(fnr == fn, "fnr {} == fn {}", fnr, fn);
      EX_ASSERT(head_ == buf_, "head_ {} == buf_ {}", (void *)head_,
                (void *)buf_);
      int sn = MIN(n - writen, pSize());
      int snr = writeHandler(src, head_, sn);
      WINC(snr);
      EX_ASSERT(positive(), "positive:{}", positive());
      EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
    }
  }
  return writen;
}

template <unsigned int D> int CycleBuffer<D>::write(char *buf, int n) {
  return writeImpl(buf, n, writeMemHandler);
}

template <unsigned int D> int CycleBuffer<D>::writefile(FILE *fp, int n) {
  return writeImpl(fp, n, writeFileHandler);
}

template <unsigned int D> int CycleBuffer<D>::writefile(FILE *fp) {
  int n = 0;
  int tmp;
  do {
    tmp = writefile(fp, BUF_SIZE);
    n += tmp;
  } while (tmp > 0);
  return n;
}

#define RINC(n)                                                                \
  do {                                                                         \
    tail_ += n;                                                                \
    readn += n;                                                                \
  } while (0)

static int readMemHandler(void *src, void *buf, int n, int readn) {
  char *src2 = (char *)src + readn;
  std::memcpy(buf, src2, n);
  return n;
}

static int readFileHandler(void *src, void *buf, int n, int readn) {
  size_t r = std::fread(buf, 1, n, (FILE *)src);
  return (int)r;
}

template <unsigned int D>
int CycleBuffer<D>::readImpl(void *src, int n,
                             int (*readHandler)(void *, void *, int, int)) {
  EX_ASSERT(n >= 0, "n {} < 0", n);
  if (!src || !n) {
    return 0;
  }
  if (D) {
    if (capacity() - size() < n) {
      int e1 = capacity() + n + 1;
      int e2 = capacity() * 2 + 1;
      expand(e1 > e2 ? ALIGN(e1) : ALIGN(e2));
    }
  }
  if (full()) {
    return 0;
  }
  int readn = 0;
  if (positive()) {
    int fn = MIN(bufEnd() - tail_ - (head_ == buf_ ? 1 : 0), n);
    int fnr = readHandler(src, tail_, fn, readn);
    RINC(fnr);
    EX_ASSERT(positive(), "positive: {}", positive());
    if (tail_ == bufEnd()) {
      tail_ = buf_;
    }
    EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
    if (n > readn && head_ != buf_) {
      EX_ASSERT(fnr == fn, "fnr {} == fn {}", fnr, fn);
      EX_ASSERT(tail_ == buf_, "tail_ {} == buf_ {}", (void *)tail_,
                (void *)buf_);
      int sn = MIN(n - readn, head_ - tail_ - 1);
      int snr = readHandler(src, tail_, sn, readn);
      RINC(snr);
      EX_ASSERT(!positive(), "!positive: {}", !positive());
      EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
    }
  } else {
    int fn = MIN(n, head_ - tail_ - 1);
    int fnr = readHandler(src, tail_, fn, readn);
    RINC(fnr);
    EX_ASSERT(!positive(), "!positive: {}", !positive());
    EX_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", toString());
  }
  return readn;
}
template <unsigned int D> int CycleBuffer<D>::read(const char *buf, int n) {
  return readImpl((void *)buf, n, readMemHandler);
}

template <unsigned int D> int CycleBuffer<D>::readfile(FILE *fp, int n) {
  return readImpl(fp, n, readFileHandler);
}

template <unsigned int D> int CycleBuffer<D>::readfile(FILE *fp) {
  int n = 0;
  int tmp;
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
    expand(ALIGN(capacity + 1));
  }
}

std::string DynamicBuffer::toString() const {
  return fmt::format("[@DynamicBuffer {}]", detail::CycleBuffer<1>::toString());
}

FixedBuffer::FixedBuffer(int capacity) {
  if (capacity > 0) {
    // precise capacity
    expand(capacity + 1);
  }
}

std::string FixedBuffer::toString() const {
  return fmt::format("[@FixedBuffer {}]", detail::CycleBuffer<0>::toString());
}
