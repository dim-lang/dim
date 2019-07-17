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
  return this == &src
             ? true
             : (lineNumber_ == src.lineNumber_ && buffer_ == src.buffer_);
}

bool Line::operator!=(const Line &src) const { return !(*this == src); }

bool Line::operator>(const Line &src) const {
  return this == &src
             ? false
             : (lineNumber_ == src.lineNumber_ && buffer_ == src.buffer_);
}

bool operator>=(const Line &src) const;
bool operator<(const Line &src) const;
bool operator<=(const Line &src) const;

// @return 0 if equal
//         -1 if less
//         +1 if greater
int compare(const Line &src) const;
int compare(const Line &src, int start2, int length2) const;
int compare(const char16_t *src, int start2, int length2) const;

int compare(int start, const Line &src) const;
int compare(int start, const Line &src, int start2, int length2) const;
int compare(int start, const char16_t *src, int start2, int length2) const;

int compare(int start, int length, const Line &src) const;
int compare(int start, int length, const Line &src, int start2,
            int length2) const;
int compare(int start, int length, const char16_t *src, int start2,
            int length2) const;

// code point compare, e.g unicode unit compare
// @return 0 if equal
//         negative if less
//         positive if greater
int compareCodePointOrder(const Line &src) const;
int compareCodePointOrder(const Line &src, int start2, int length2) const;
int compareCodePointOrder(const char16_t *src, int start2, int length2) const;

int compareCodePointOrder(int start, const Line &src) const;
int compareCodePointOrder(int start, const Line &src, int start2,
                          int length2) const;
int compareCodePointOrder(int start, const char16_t *src, int start2,
                          int length2) const;

int compareCodePointOrder(int start, int length, const Line &src) const;
int compareCodePointOrder(int start, int length, const Line &src, int start2,
                          int length2) const;
  int compareCodePointOrder(int start, int length,
  int compareCodePointOrder(int start, int length, const char16_t *src,
                            int start2, int length2) const;

  /**
   * string match
   */

  bool startsWith(const Line &src) const;
  bool startsWith(const Line &src, int start2, int length2) const;
  bool startsWith(const char16_t *src, int start2, int length2) const;

  bool endsWith(const Line &src) const;
  bool endsWith(const Line &src, int start2, int length2) const;
  bool endsWith(const char16_t *src, int start2, int length2) const;

  // @return index of src if found
  //         -1 if not found
  int indexOf(const Line &src) const;
  int indexOf(const Line &src, int start2, int length2) const;
  int indexOf(const char16_t *src, int start, int length) const;

  int indexOf(int start, const Line &src) const;
  int indexOf(int start, const Line &src, int start2, int length2) const;
  int indexOf(int start, const char16_t *src, int start2, int length2) const;

  int indexOf(int start, int length, const Line &src) const;
  int indexOf(int start, int length, const Line &src, int start2,
              int length2) const;
  int indexOf(int start, int length, const char16_t *src, int start2,
              int length2) const;

  // @return index of char c if found
  //         -1 if not found
  int indexOf(char16_t c) const;
  int indexOf(UChar32 c) const;

  int indexOf(int start, char16_t c) const;
  int indexOf(int start, UChar32 c) const;

  int indexOf(int start, int length, char16_t c) const;
  int indexOf(int start, int length, UChar32 c) const;

  // @return last index of src if found
  //         -1 if not found
  int lastIndexOf(const Line &src) const;
  int lastIndexOf(const Line &src, int start2, int length2) const;
  int lastIndexOf(const char16_t *src, int start2, int length2) const;

  int lastIndexOf(int start, const Line &src) const;
  int lastIndexOf(int start, const Line &src, int start2, int length2) const;
  int lastIndexOf(int start, const char16_t *src, int start2,
                  int length2) const;

  int lastIndexOf(int start, int length, const Line &src) const;
  int lastIndexOf(int start, int length, const Line &src, int start2,
                  int length2) const;
  int lastIndexOf(int start, int length, const char16_t *src, int start2,
                  int length2) const;

  // @return last index of char c if found
  //         -1 if not found
  int lastIndexOf(char16_t c) const;
  int lastIndexOf(UChar32 c) const;

  int lastIndexOf(int start, char16_t c) const;
  int lastIndexOf(int start, UChar32 c) const;

  int lastIndexOf(int start, int length, char16_t c) const;
  int lastIndexOf(int start, int length, UChar32 c) const;

  /**
   * indexing
   */

  char16_t getCharAt(int offset) const;
  Line &setCharAt(int offset, char16_t c);
  char16_t &operator[](int offset);
  char16_t operator[](int offset) const;

  UChar32 getChar32At(int offset) const;
  Line &setChar32At(int offset, UChar32 c);
  // @return begin of a unicode char at offset
  int getChar32Start(int offset) const;
  // @return end of a unicode char at offset
  int getChar32Limit(int offset) const;
  // @return new index moved delta by unicode unit, start from index
  int moveIndex32(int index, int delta) const;

  /**
   * transform
   */

  // copy [start, start+length) to dest[start2, start2+length2)
  // @return copyied chars
  int extract(int start, char16_t *dest, int start2, int length2) const;
  int extract(int start, Line &dest) const;
  int extract(int start, Line &dest, int start2, int length2) const;

  int extract(int start, int length, Line &dest) const;
  int extract(int start, int length, Line &dest, int start2, int length2) const;
  int extract(int start, int length, char16_t *dest, int start2,
              int length2) const;

  // @return range [start, start+length)
  Line subString(int start) const;
  Line subString(int start, int length) const;

  std::string &toUTF8(std::string &dest) const;
  char *toUTF8(char *dest, int capacity, UErrorCode &errCode) const;

  std::u32string &toUTF32(std::u32string &dest, UErrorCode &errCode) const;
  UChar32 *toUTF32(UChar32 *dest, int capacity, UErrorCode &errCode) const;

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
  // it will allocate new memory with `capacity`, copy original data, and returns to user
  // it is *opened* after `getBuffer(int capacity)`
  // must *close* via `releaseBuffer(int length)` before next *open*
  char16_t *getBuffer(int capacity);
  // save modifies data after user modifies this new memory, and release original memory
  // parameter `length` is new line length
  void releaseBuffer(int length);

  // @return readonly null-terminated buffer if success
  //         nullptr if allocation fail
  // normally Line define a unicode string via buffer/length pair, without null at the end
  // this method will append null at the end of buffer, e.g `buffer_[length()]`
  // extra memory is allocated if needed, and fail if allocation failure
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
  Line &replace(int start, int length, const Line &src, int start2,
                int length2);
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
