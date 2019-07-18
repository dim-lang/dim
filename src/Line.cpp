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

Line::Line() : lineNumber_(-1), buffer_() {}

Line::Line(int lineNumber) : lineNumber_(lineNumber), buffer_() {}

Line::Line(int lineNumber, char16_t c) : lineNumber_(lineNumber), buffer_(c) {}

Line::Line(int lineNumber, const Line &src)
    : lineNumber_(lineNumber), buffer_(src.buffer_) {}

Line::Line(int lineNumber, const Line &src, int start2, int length2)
    : lineNumber_(lineNumber), buffer_(src.buffer_, start2, length2) {}

Line::Line(int lineNumber, char16_t *src, int start2, int length2)
    : lineNumber_(lineNumber), buffer_(src, start2, length2) {}

Line::Line(const Line &other)
    : lineNumber_(other.lineNumber_), buffer_(other.buffer_) {}

Line &Line::operator=(const Line &other) {
  if (this == &other) {
    return *this;
  }
  lineNumber_ = other.lineNumber_;
  buffer_ = other.buffer_;
  return *this;
}

Line &Line::operator=(char16_t other) {
  if (this == &other) {
    return *this;
  }
  buffer_ = other;
  return *this;
}

Line &Line::operator=(UChar32 other) {
  if (this == &other) {
    return *this;
  }
  buffer_ = other;
  return *this;
}

Line &Line::assignFrom(const Line &src) {
  if (this == &other) {
    return *this;
  }
  lineNumber_ = src.lineNumber_;
  buffer_.setTo(src.buffer_);
  return *this;
}

Line &assignFrom(const Line &src, int start2, int length2) {
  if (this == &other) {
    return *this;
  }
  lineNumber_ = src.lineNumber_;
  buffer_.setTo(src.buffer_, start2, length2);
  return *this;
}

Line &Line::assignFrom(const char16_t *src, int start2, int length2) {
  if (this == &other) {
    return *this;
  }
  lineNumber_ = src.lineNumber_;
  buffer_.setTo(src, start2, length2);
  return *this;
}

Line &Line::assignFrom(char16_t src) {
  if (this == &other) {
    return *this;
  }
  lineNumber_ = src.lineNumber_;
  buffer_.setTo(src);
  return *this;
}
Line &Line::assignFrom(UChar32 src) {
  if (this == &other) {
    return *this;
  }
  lineNumber_ = src.lineNumber_;
  buffer_.setTo(src);
  return *this;
}

Line::Line(Line &&other)
    : lineNumber_(std::move(other.lineNumber_)),
      buffer_(std::move(other.buffer_)) {}

Line &Line::operator=(Line &&other) {
  if (this == &other) {
    return *this;
  }
  std::swap(lineNumber_, other.lineNumber_);
  std::swap(buffer_, other.buffer_);
  return *this;
}

bool Line::operator==(const Line &src) const {
  return this == &src ? true : (buffer_ == src.buffer_);
}

bool Line::operator!=(const Line &src) const { return !(*this == src); }

bool Line::operator>(const Line &src) const {
  return this == &src ? false : (buffer_ > src.buffer_);
}

bool Line::operator>=(const Line &src) const {
  return *this == src || *this > src;
}

bool Line::operator<(const Line &src) const {
  return this == &src ? false : (buffer_ < src.buffer_);
}

bool Line::operator<=(const Line &src) const {
  return *this == src || *this < src;
}

int Line::compare(const Line &src) const {
  return buffer_.compare(src.buffer_);
}

int Line::compare(const Line &src, int start2, int length2) const {
  return buffer_.compare(0, length(), src.buffer_, start2, length2);
}

int Line::compare(const char16_t *src, int start2, int length2) const {
  return buffer_.compare(0, length(), src, start2, length2);
}

int Line::compare(int start, const Line &src) const {
  return buffer_.compare(start, -1, src.buffer_);
}

int Line::compare(int start, const Line &src, int start2, int length2) const {
  return buffer_.compare(start, -1, src.buffer_, start2, length2);
}

int Line::compare(int start, const char16_t *src, int start2,
                  int length2) const {
  return buffer_.compare(start, -1, src, start2, length2);
}

int Line::compare(int start, int length, const Line &src) const {
  return buffer_.compare(start, length, src.buffer_);
}

int Line::compare(int start, int length, const Line &src, int start2,
                  int length2) const {
  return buffer_.compare(start, length, src.buffer_, start2, length2);
}

int Line::compare(int start, int length, const char16_t *src, int start2,
                  int length2) const {
  return buffer_.compare(start, length, src, start2, length2);
}

int Line::compareCodePointOrder(const Line &src) const {
  return buffer_.compareCodePointOrder(src.buffer_);
}

int Line::compareCodePointOrder(const Line &src, int start2,
                                int length2) const {
  return buffer_.compareCodePointOrder(0, length(), src.buffer_, start2,
                                       length2);
}

int Line::compareCodePointOrder(const char16_t *src, int start2,
                                int length2) const {
  return buffer_.compareCodePointOrder(0, length(), src, start2, length2);
}

int Line::compareCodePointOrder(int start, const Line &src) const {
  return buffer_.compareCodePointOrder(start, -1, src.buffer_);
}

int Line::compareCodePointOrder(int start, const Line &src, int start2,
                                int length2) const {
  return buffer_.compareCodePointOrder(start, -1, src.buffer_, start2, length2);
}

int Line::compareCodePointOrder(int start, const char16_t *src, int start2,
                                int length2) const {
  return buffer_.compareCodePointOrder(start, -1, src, start2, length2);
}

int Line::compareCodePointOrder(int start, int length, const Line &src) const {
  return buffer_.compareCodePointOrder(start, length, src.buffer_);
}

int Line::compareCodePointOrder(int start, int length, const Line &src,
                                int start2, int length2) const {
  return buffer_.compareCodePointOrder(start, length, src.buffer_, start2,
                                       length2);
}
int Line::compareCodePointOrder(int start, int length, const char16_t *src,
                                int start2, int length2) const {
  return buffer_.compareCodePointOrder(start, length, src, start2, length2);
}

bool Line::startsWith(const Line &src) const {
  return buffer_.startsWith(src.buffer_);
}

bool Line::startsWith(const Line &src, int start2, int length2) const {
  return buffer_.startsWith(src.buffer_, start2, length2);
}

bool Line::startsWith(const char16_t *src, int start2, int length2) const {
  return buffer_.startsWith(src, start2, length2);
}

bool Line::endsWith(const Line &src) const {
  return buffer_.endsWith(src.buffer_);
}

bool Line::endsWith(const Line &src, int start2, int length2) const {
  return buffer_.endsWith(src.buffer_, start2, length2);
}

bool Line::endsWith(const char16_t *src, int start2, int length2) const {
  return buffer_.endsWith(src, start2, length2);
}

int Line::indexOf(const Line &src) const {
  return buffer_.indexOf(src.buffer_);
}

int Line::indexOf(const Line &src, int start2, int length2) const {
  return buffer_.indexOf(0, length(), src.buffer_, start2, length2);
}

int Line::indexOf(const char16_t *src, int start2, int length2) const {
  return buffer_.indexOf(0, length(), src, start2, length2);
}

int Line::indexOf(int start, const Line &src) const {
  return buffer_.indexOf(src.buffer_, start);
}

int Line::indexOf(int start, const Line &src, int start2, int length2) const {
  return buffer_.indexOf(src.buffer_, start2, length2, start);
}

int Line::indexOf(int start, const char16_t *src, int start2,
                  int length2) const {
  return buffer_.indexOf(src, start2, length2, start, -1);
}

int Line::indexOf(int start, int length, const Line &src) const {
  return buffer_.indexOf(src.buffer_, start, length);
}

int Line::indexOf(int start, int length, const Line &src, int start2,
                  int length2) const {
  return buffer_.indexOf(start, length, src.buffer_, start2, length2);
}

int Line::indexOf(int start, int length, const char16_t *src, int start2,
                  int length2) const {
  return buffer_.indexOf(start, length, src, start2, length2);
}

int Line::indexOf(char16_t c) const { return buffer_.indexOf(c); }

int Line::indexOf(UChar32 c) const { return buffer_.indexOf(c); }

int Line::indexOf(int start, char16_t c) const {
  return buffer_.indexOf(c, start);
}

int Line::indexOf(int start, UChar32 c) const {
  return buffer_.indexOf(c, start);
}

int Line::indexOf(int start, int length, char16_t c) const {
  return buffer_.indexOf(c, start, length);
}

int Line::indexOf(int start, int length, UChar32 c) const {
  return buffer_.indexOf(c, start, length);
}

int Line::lastIndexOf(const Line &src) const {
  return buffer_.lastIndexOf(src.buffer_);
}

int Line::lastIndexOf(const Line &src, int start2, int length2) const {
  return buffer_.lastIndexOf(src.buffer_, start2, length2, 0, length());
}

int Line::lastIndexOf(const char16_t *src, int start2, int length2) const {
  return buffer_.lastIndexOf(src, start2, length2, 0, length());
}

int Line::lastIndexOf(int start, const Line &src) const {
  return buffer_.lastIndexOf(src.buffer_, start);
}

int Line::lastIndexOf(int start, const Line &src, int start2,
                      int length2) const {
  return buffer_.lastIndexOf(src.buffer_, start2, length2, start);
}

int Line::lastIndexOf(int start, const char16_t *src, int start2,
                      int length2) const {
  return buffer_.lastIndexOf(src, start2, length2, start, -1);
}

int Line::lastIndexOf(int start, int length, const Line &src) const {
  return buffer_.lastIndexOf(src.buffer_, start, length);
}

int Line::lastIndexOf(int start, int length, const Line &src, int start2,
                      int length2) const {
  return buffer_.lastIndexOf(src.buffer_, start2, length2, start, length);
}

int Line::lastIndexOf(int start, int length, const char16_t *src, int start2,
                      int length2) const {
  return buffer_.lastIndexOf(src.buffer_, start2, length2, start, length);
}

int Line::lastIndexOf(char16_t c) const { return buffer_.lastIndexOf(c); }

int Line::lastIndexOf(UChar32 c) const { return buffer_.lastIndexOf(c); }

int Line::lastIndexOf(int start, char16_t c) const {
  return buffer_.lastIndexOf(c, start);
}

int Line::lastIndexOf(int start, UChar32 c) const {
  return buffer_.lastIndexOf(c, start);
}

int Line::lastIndexOf(int start, int length, char16_t c) const {
  return buffer_.lastIndexOf(c, start, length);
}

int Line::lastIndexOf(int start, int length, UChar32 c) const {
  return buffer_.lastIndexOf(c, start, length);
}

char16_t Line::charAt(int offset) const { return buffer_.charAt(offset); }

Line &Line::setCharAt(int offset, char16_t c) {
  buffer_.setCharAt(offset, c);
  return *this;
}

char16_t &Line::operator[](int offset) { return buffer_[offset]; }

char16_t Line::operator[](int offset) const { return buffer_[offset]; }

UChar32 Line::char32At(int offset) const { return buffer_.char32At(offset); }

Line &Line::setChar32At(int offset, UChar32 c) {
  buffer_.setChar32At(offset, c);
  return *this;
}

int Line::getChar32Start(int offset) const {
  return buffer_.getChar32Start(offset);
}

int Line::getChar32Limit(int offset) const {
  return buffer_.getChar32Limit(offset);
}

int Line::moveIndex32(int index, int delta) const {
  return buffer_.moveIndex32(index, delta);
}

// @return range [start, start+length)
Line Line::subString(int start) const {
  return Line(lineNumber_, *this, start, length() - start);
}

Line Line::subString(int start, int length) const {
  return Line(lineNumber_, *this, start, length);
}

std::string &Line::toUTF8(std::string &dest) const {
  return buffer_.toUTF8String(dest);
}

int Line::toUTF8(char *dest, int capacity, UErrorCode &err) const {
  std::string tmp;
  tmp = toUTF8(tmp);
  if (tmp.length() < capacity) {
    err = U_ZERO_ERROR;
    std::memcpy(dest, tmp.data(), tmp.length());
    dest[tmp.length()] = 0;
    return tmp.length();
  } else if (tmp.length() == capacity) {
    err = U_STRING_NOT_TERMINATED;
    std::memcpy(dest, tmp.data(), tmp.length());
    return tmp.length();
  } else {
    err = U_BUFFER_OVERFLOW_ERROR;
    return 0;
  }
}

std::u32string &Line::toUTF32(std::u32string &dest, UErrorCode &err) const {}

UChar32 *Line::toUTF32(UChar32 *dest, int capacity, UErrorCode &err) const {
  int n = buffer_.toUTF32(dest, capacity, err);
}

virtual std::string toString() const;

static Line fromUTF8(const std::string &src);
static Line fromUTF8(const std::string &src, int start2, int length2);
static Line fromUTF8(const char *src, int start2, int length2);

static Line fromUTF32(const std::u32string &src);
static Line fromUTF32(const std::u32string &src, int start2, int length2);
static Line fromUTF32(const UChar32 *src, int start2, int length2);

/**
 * attribute
 */

// @return string length, not unicode unit count
int length() const;
int capacity() const;
bool empty() const;
// @return true if line contains invalid string
//         false if all valid
// bool bogus() const;

// @return char32 count, e.g unicode unit count
int countChar32() const;
int countChar32(int start) const;
int countChar32(int start, int length) const;

// check if has more than number unicode unit
bool moreChar32Than(int number) const;
bool moreChar32Than(int start, int number) const;
bool moreChar32Than(int start, int length, int number) const;

// @return readonly raw memory pointer inside with no extra work
const char16_t *getBuffer() const;

// @return writable raw memory pointer based on copy-on-write policy if success
//         nullptr if allocation fail
// it will allocate new memory with `capacity`, copy original data, and returns
// to user it is *opened* after `getBuffer(int capacity)` must *close* via
// `releaseBuffer(int length)` before next *open*
char16_t *getBuffer(int capacity);
// save modifies data after user modifies this new memory, and release original
// memory parameter `length` is new line length
void releaseBuffer(int length);

// @return readonly null-terminated buffer if success
//         nullptr if allocation fail
// normally Line define a unicode string via buffer/length pair, without null at
// the end this method will append null at the end of buffer, e.g
// `buffer_[length()]` extra memory is allocated if needed, and fail if
// allocation failure
const char16_t *getTerminatedBuffer();

/**
 * modify
 */

Line &operator+=(char16_t c);
Line &operator+=(UChar32 c);
Line &operator+=(const Line &src);

Line &append(char16_t c);
Line &append(UChar32 c);
Line &append(const Line &src);
Line &append(const Line &src, int start2, int length2);
Line &append(const char16_t *src, int start2, int length2);

Line &insert(int start, char16_t c);
Line &insert(int start, UChar32 c);
Line &insert(int start, const Line &src);
Line &insert(int start, const Line &src, int start2, int length2);
Line &insert(int start, const char16_t *src, int start2, int length2);

// replace [start, start+length) with char c
Line &replace(int start, int length, char16_t c);
Line &replace(int start, int length, UChar32 c);
Line &replace(int start, int length, const Line &src);
Line &replace(int start, int length, const Line &src, int start2, int length2);
Line &replace(int start, int length, const char16_t *src, int start2,
              int length2);

// find all oldText and replace them with newText
Line &findAndReplace(const Line &oldText, const Line &newText);
Line &findAndReplace(int start, const Line &oldText, const Line &newText);
Line &findAndReplace(int start, int length, const Line &oldText,
                     const Line &newText);

// remove all string
Line &remove();
Line &remove(int start);
Line &remove(int start, int length);

// overwrite length of beginning with space (U+0020)
bool padLeading(int length);
bool padLeading(int length, char16_t c);
// overwrite length of end with space (U+0020)
bool padTailing(int length);
bool padTailing(int length, char16_t c);

bool truncate(int length);
Line &trim();
Line &reverse();
Line &reverse(int start);
Line &reverse(int start, int length);
Line &toUpper();
Line &toLower();

} // namespace fastype

#undef ALLOC_ALIGN
