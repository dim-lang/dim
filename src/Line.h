// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "unicode/unistr.h"
#include "unicode/ustring.h"
#include <memory>

namespace fastype {

class File;
class Line;

class Line : public Logging, Stringify {
public:
  /**
   * Api Definition
   * parameter `start` means range [start, length())
   * parameter `start`, `length` means range [start, start+length)
   * parameter `src` means src[0, src.length())
   * parameter `src`, `start2`, `length2` means src[start2, start2+length2)
   */

  /**
   * constructor
   */

  // empty
  Line();
  Line(int lineNumber);
  // single char
  Line(int lineNumber, char16_t c);
  // copy from src
  Line(int lineNumber, const Line &src);
  Line(int lineNumber, const Line &src, int start2, int length2);
  Line(int lineNumber, char16_t *src, int start2, int length2);

  // copy
  Line(const Line &other);
  Line &operator=(const Line &other);
  Line &operator=(char16_t other);
  Line &operator=(UChar32 other);

  Line &assignFrom(const Line &src);
  Line &assignFrom(const Line &src, int start2, int length2);
  Line &assignFrom(const char16_t *src, int start2, int length2);

  Line &assignFrom(char16_t src);
  Line &assignFrom(UChar32 src);
  // set to invalid string, then line will be true with `bogus()`
  // Line &assignBogus();

  // move
  Line(Line &&other);
  Line &operator=(Line &&other);

  /**
   * destructor
   */

  virtual ~Line();

  /**
   * compare
   */

  // bitwise compare, e.g char16_t/UChar comparation
  bool operator==(const Line &src) const;
  bool operator!=(const Line &src) const;
  bool operator>(const Line &src) const;
  bool operator>=(const Line &src) const;
  bool operator<(const Line &src) const;
  bool operator<=(const Line &src) const;

  // @return 0 if equal, -1 if less, +1 if greater
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
  // @return 0 if equal, negative if less, positive if greater
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

  // @return index of src, -1 if not found
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

  // @return index of char c, -1 if not found
  int indexOf(char16_t c) const;
  int indexOf(UChar32 c) const;

  int indexOf(int start, char16_t c) const;
  int indexOf(int start, UChar32 c) const;

  int indexOf(int start, int length, char16_t c) const;
  int indexOf(int start, int length, UChar32 c) const;

  // @return last index of src, -1 if not found
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

  // @return last index of char c, -1 if not found
  int lastIndexOf(char16_t c) const;
  int lastIndexOf(UChar32 c) const;

  int lastIndexOf(int start, char16_t c) const;
  int lastIndexOf(int start, UChar32 c) const;

  int lastIndexOf(int start, int length, char16_t c) const;
  int lastIndexOf(int start, int length, UChar32 c) const;

  /**
   * indexing
   */

  char16_t charAt(int offset) const;
  char16_t operator[](int offset) const;
  Line &setCharAt(int offset, char16_t c);
  UChar32 char32At(int offset) const;
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
  std::string toUTF8() const;
  virtual std::string toString() const;

  /**
   * attribute
   */

  // @return string length, not unicode unit count
  int length() const;
  int capacity() const;
  bool empty() const;
  // @return true if line contains invalid string, false if all valid
  // bool bogus() const;

  // @return char32 count, e.g unicode unit count
  int countChar32() const;
  int countChar32(int start) const;
  int countChar32(int start, int length) const;

  // check if has more than number unicode unit
  bool moreChar32Than(int number) const;
  bool moreChar32Than(int start, int number) const;
  bool moreChar32Than(int start, int length, int number) const;

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

  Line &replace(int start, int length, char16_t c);
  Line &replace(int start, int length, UChar32 c);
  Line &replace(int start, int length, const Line &src);
  Line &replace(int start, int length, const Line &src, int start2,
                int length2);
  Line &replace(int start, int length, const char16_t *src, int start2,
                int length2);

private:
  int32_t lineNumber_;
  icu::UnicodeString buffer_;

  friend class File;
};

} // namespace fastype
