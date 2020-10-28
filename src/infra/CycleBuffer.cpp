// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "infra/CycleBuffer.h"
#include "fmt/format.h"
#include "infra/Log.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <tuple>

#define ALIGN(n) (n < 8 ? 8 : (n % 8 == 0 ? n : ((n / 8 + 1) * 8)))
#define MIN(a, b) (std::min<int>(a, b))
#define BUF_SIZE 4096

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
  LOG_ASSERT(positive(), "tail_ {} >= head_ {}, positive: {}, str: {}",
             (void *)tail_, (void *)head_, positive(), str());
  return tail_ - head_;
}

template <unsigned int D>
bool CycleBuffer<D>::pContain(const char *position) const {
  LOG_ASSERT(positive(), "tail_ {} >= head_ {}, positive: {}, str: {}",
             (void *)tail_, (void *)head_, positive(), str());
  return position >= head_ && position < tail_;
}

template <unsigned int D> long CycleBuffer<D>::nLeftSize() const {
  LOG_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, str: {}",
             (void *)tail_, (void *)head_, positive(), str());
  LOG_ASSERT(tail_ >= buf_, "tail_ {} >= buf_ {}", (void *)tail_, (void *)buf_);
  return tail_ - buf_;
}

template <unsigned int D> long CycleBuffer<D>::nRightSize() const {
  LOG_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, str: {}",
             (void *)tail_, (void *)head_, positive(), str());
  LOG_ASSERT(bufEnd() >= head_, "bufEnd {} >= head_ {}", (void *)bufEnd(),
             (void *)head_);
  return bufEnd() - head_;
}

template <unsigned int D>
bool CycleBuffer<D>::nLeftContain(const char *position) const {
  LOG_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, str: {}",
             (void *)tail_, (void *)head_, positive(), str());
  LOG_ASSERT(tail_ >= buf_, "tail_ {} >= buf_ {}", (void *)tail_, (void *)buf_);
  return position >= buf_ && position < tail_;
}

template <unsigned int D>
bool CycleBuffer<D>::nRightContain(const char *position) const {
  LOG_ASSERT(!positive(), "tail_ {} >= head_ {}, positive: {}, str: {}",
             (void *)tail_, (void *)head_, positive(), str());
  LOG_ASSERT(bufEnd() >= head_, "bufEnd {} >= head_ {}", (void *)bufEnd(),
             (void *)head_);
  return position >= head_ && position < bufEnd();
}

template <unsigned int D>
CycleBuffer<D>::CycleBuffer()
    : buf_(nullptr), head_(nullptr), tail_(nullptr), capacity_(0) {}

template <unsigned int D> CycleBuffer<D>::~CycleBuffer() { reset(); }

template <unsigned int D> int CycleBuffer<D>::capacity() const {
  if (buf_) {
    LOG_ASSERT(capacity_ - 1 >= 0, "capacity_ - 1 {} >= 0", capacity_ - 1);
  } else {
    LOG_ASSERT(capacity_ == 0 && size() == 0, "capacity_ {} == size {} == 0",
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
  LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
  release();
  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = 0;
}

template <unsigned int D>
char *CycleBuffer<D>::nextImpl(char *position, int distance) const {
  LOG_ASSERT(contain(position), "position {} out of range {}", (void *)position,
             str());
  LOG_ASSERT(distance >= 0, "distance {} < 0", distance);
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
      LOG_ASSERT(nRightContain(position), "position {} must in right part {}",
                 (void *)position, str());
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
  LOG_ASSERT(contain(position), "position {} out of range {}", (void *)position,
             str());
  LOG_ASSERT(distance >= 0, "distance {} >= 0", distance);
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
      LOG_ASSERT(nRightContain(position), "position {} must in right part {}",
                 (void *)position, str());
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

template <unsigned int D> Cowstr CycleBuffer<D>::str() const {
  return fmt::format("buf_:{}, head_:{}, tail_:{}, capacity_:{}", (void *)buf_,
                     (void *)head_, (void *)tail_, capacity_);
}

template <unsigned int D> void CycleBuffer<D>::release() {
  if (buf_) {
    std::free(buf_);
    buf_ = nullptr;
  }
}

static int writeMemHandler(void *source, void *buffer, int n,
                           int alreadyWrite) {
  char *source2 = (char *)source + alreadyWrite;
  std::memcpy(source2, buffer, n);
  return n;
}

static int writeFileHandler(void *source, void *buffer, int n,
                            int alreadyWrite) {
  size_t r = std::fwrite(buffer, 1, n, (FILE *)source);
  return (int)r;
}

static int readMemHandler(void *source, void *buffer, int n, int alreadyRead) {
  char *source2 = (char *)source + alreadyRead;
  std::memcpy(buffer, source2, n);
  return n;
}

static int readFileHandler(void *source, void *buffer, int n, int alreadyRead) {
  size_t r = std::fread(buffer, 1, n, (FILE *)source);
  return (int)r;
}

#define WRITE_MOVE(n)                                                          \
  do {                                                                         \
    head_ = next(head_, n);                                                    \
    writen += n;                                                               \
  } while (0)

template <unsigned int D>
int CycleBuffer<D>::writeImpl(void *src, int n,
                              int (*writeHandler)(void *, void *, int, int)) {
  LOG_ASSERT(n >= 0, "n {} < 0", n);
  if (!src || !n) {
    return 0;
  }
  if (empty()) {
    return 0;
  }
  int writen = 0;
  if (positive()) {
    int fn = MIN(size(), n);
    int fnr = writeHandler(src, head_, fn, writen);
    WRITE_MOVE(fnr);
    LOG_ASSERT(positive(), "positive:{}", positive());
    LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
  } else {
    int fn = MIN(bufEnd() - head_, n);
    int fnr = writeHandler(src, head_, fn, writen);
    WRITE_MOVE(fnr);
    LOG_ASSERT(head_ == buf_ || head_ < bufEnd(),
               "head_ {} == buf_ {} or head_ {} < bufEnd {}", (void *)head_,
               (void *)buf_, (void *)head_, (void *)bufEnd());
    LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
    if (n > writen && fn == fnr) {
      LOG_ASSERT(head_ == buf_, "head_ {} == buf_ {}", (void *)head_,
                 (void *)buf_);
      int sn = MIN(n - writen, pSize());
      int snr = writeHandler(src, head_, sn, writen);
      WRITE_MOVE(snr);
      LOG_ASSERT(positive(), "positive:{}", positive());
      LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
    }
  }
  return writen;
}

#define READ_MOVE(n)                                                           \
  do {                                                                         \
    tail_ += n;                                                                \
    readn += n;                                                                \
  } while (0)

template <unsigned int D>
int CycleBuffer<D>::readImpl(void *src, int n,
                             int (*readHandler)(void *, void *, int, int)) {
  LOG_ASSERT(n >= 0, "n {} < 0", n);
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
    READ_MOVE(fnr);
    LOG_ASSERT(positive(), "positive: {}", positive());
    if (tail_ == bufEnd()) {
      tail_ = buf_;
    }
    LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
    if (n > readn && head_ != buf_ && fn == fnr) {
      LOG_ASSERT(tail_ == buf_, "tail_ {} == buf_ {}", (void *)tail_,
                 (void *)buf_);
      int sn = MIN(n - readn, head_ - tail_ - 1);
      int snr = readHandler(src, tail_, sn, readn);
      READ_MOVE(snr);
      LOG_ASSERT(!positive(), "!positive: {}", !positive());
      LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
    }
  } else {
    int fn = MIN(n, head_ - tail_ - 1);
    int fnr = readHandler(src, tail_, fn, readn);
    READ_MOVE(fnr);
    LOG_ASSERT(!positive(), "!positive: {}", !positive());
    LOG_ASSERT(size() == 0 || size() > 0 && head_ != tail_, "{}", str());
  }
  return readn;
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
  LOG_ASSERT(n >= 0, "n {} >= 0", n);
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

template <unsigned int D> char *CycleBuffer<D>::search(char c) {
  if (size() == 0) {
    return nullptr;
  }
  for (char *i = begin(); i != end(); i = next(i)) {
    LOG_ASSERT(i, "i is null");
    if (*i == c) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D> const char *CycleBuffer<D>::search(char c) const {
  if (size() == 0) {
    return nullptr;
  }
  for (const char *i = begin(); i != end(); i = next(i)) {
    LOG_ASSERT(i, "i is null");
    if (*i == c) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D> char *CycleBuffer<D>::search(char *s, int n) {
  if (size() == 0 || !s) {
    return nullptr;
  }
  for (char *i = begin(); i != end(); i = next(i)) {
    LOG_ASSERT(i, "i is null");
    int k = 0;
    bool match = true;
    for (char *j = i; j != end() && k < n; j = next(j), k++) {
      if (*j != s[k]) {
        match = false;
        break;
      }
    }
    if (match) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D>
const char *CycleBuffer<D>::search(const char *s, int n) const {
  if (size() == 0 || !s) {
    return nullptr;
  }
  for (const char *i = begin(); i != end(); i = next(i)) {
    LOG_ASSERT(i, "i is null");
    int k = 0;
    bool match = true;
    for (const char *j = i; j != end() && k < n; j = next(j), k++) {
      if (*j != s[k]) {
        match = false;
        break;
      }
    }
    if (match) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D> char *CycleBuffer<D>::rsearch(char c) {
  if (size() == 0) {
    return nullptr;
  }
  for (char *i = rbegin(); i != rend(); i = prev(i)) {
    LOG_ASSERT(i, "i is null");
    if (*i == c) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D> const char *CycleBuffer<D>::rsearch(char c) const {
  if (size() == 0) {
    return nullptr;
  }
  for (const char *i = rbegin(); i != rend(); i = prev(i)) {
    LOG_ASSERT(i, "i is null");
    if (*i == c) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D> char *CycleBuffer<D>::rsearch(char *s, int n) {
  if (size() == 0 || !s) {
    return nullptr;
  }
  for (char *i = rbegin(); i != rend(); i = prev(i)) {
    LOG_ASSERT(i, "i is null");
    int k = 0;
    bool match = true;
    for (char *j = i; j != rend() && k < n; j = prev(j), k++) {
      if (*j != s[k]) {
        match = false;
        break;
      }
    }
    if (match) {
      return i;
    }
  }
  return nullptr;
}

template <unsigned int D>
const char *CycleBuffer<D>::rsearch(const char *s, int n) const {
  if (size() == 0 || !s) {
    return nullptr;
  }
  for (const char *i = rbegin(); i != rend(); i = prev(i)) {
    LOG_ASSERT(i, "i is null");
    int k = 0;
    bool match = true;
    for (const char *j = i; j != rend() && k < n; j = prev(j), k++) {
      if (*j != s[k]) {
        match = false;
        break;
      }
    }
    if (match) {
      return i;
    }
  }
  return nullptr;
}

} // namespace detail

DynamicCycleBuffer::DynamicCycleBuffer(int capacity) {
  if (capacity > 0) {
    expand(ALIGN(capacity + 1));
  }
}

Cowstr DynamicCycleBuffer::str() const {
  return fmt::format("[@DynamicCycleBuffer {}]", detail::CycleBuffer<1>::str());
}

FixedCycleBuffer::FixedCycleBuffer(int capacity) {
  if (capacity > 0) {
    // precise capacity
    expand(capacity + 1);
  }
}

Cowstr FixedCycleBuffer::str() const {
  return fmt::format("[@FixedCycleBuffer {}]", detail::CycleBuffer<0>::str());
}
