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
   * constructor
   */

  // empty
  Line();
  // line number
  Line(int32_t lineNumber);
  // single char
  Line(int32_t lineNumber, char c);
  // allocate memory and copy from string
  Line(int32_t lineNumber, char *src, int32_t offset);
  Line(int32_t lineNumber, char *src, int32_t offset, int32_t n);
  Line(int32_t lineNumber, char *first, char *last);

  /**
   * copy
   */

  Line &operator=(const Line &other);
  Line &operator=(char16_t other);
  Line &operator=(UChar32 other);

  /**
   * move
   */

  Line &operator=(Line &&other);

  /**
   * destructor
   */

  virtual ~Line();

  /**
   * comparation
   */

  // bitwise comparation, e.g char16_t/UChar comparation
  bool operator==(const Line &src) const;
  bool operator!=(const Line &src) const;
  bool operator>(const Line &src) const;
  bool operator>=(const Line &src) const;
  bool operator<(const Line &src) const;
  bool operator<=(const Line &src) const;
  // @return 0 if equal, -1 if less, +1 if greater
  int compare(const Line &src) const;
  // compare [start, start+length) with src
  int compare(int start, int length, const Line &src) const;
  // compare [start, start+length) with src[start2, start2+length2)
  int compare(int start, int length, const Line &src, int start2,
              int length2) const;
  int compare(const char16_t *src) const;
  int compare(int start, int length, const char16_t *src) const;
  int compare(int start, int length, const char16_t *src, int start2,
              int length2) const;

  // code point comparation, e.g unicode unit comparation
  // @return 0 if equal, negative if less, positive if greater
  int compareCodePointOrder(const Line &src) const;
  // compare [start, start+length) with src
  int compareCodePointOrder(int start, int length, const Line &src) const;
  // compare [start, start+length) with src[start2, start2+length2)
  int compareCodePointOrder(int start, int length, const Line &src, int start2,
                            int length2) const;
  int compareCodePointOrder(const char16_t *src) const;
  int compareCodePointOrder(int start, int length, const char16_t *src) const;
  int compareCodePointOrder(int start, int length, const char16_t *src,
                            int start2, int length2) const;

  /**
   * string match
   */

  // check if start with src
  bool startsWith(const Line &src) const;
  // check if start with src[start, start+length)
  bool startsWith(const Line &src, int start, int length) const;
  bool startsWith(const char16_t *src, int start, int length) const;

  // check if end with src
  bool endsWith(const Line &src) const;
  // check if end with src[start, start+length)
  bool endsWith(const Line &src, int start, int length) const;
  bool endsWith(const char16_t *src, int start, int length) const;

  // @return index of src, -1 if not found
  int indexOf(const Line &src) const;
  // @return index of src[start, start+length), -1 if not found
  int indexOf(const Line &src, int start, int length) const;
  // @return index of src[start2, start2+length2) in [start, start+length)
  //         -1 if not found
  int indexOf(int start, int length, const Line &src, int start2,
              int length2) const;
  int indexOf(const char16_t *src) const;
  int indexOf(const char16_t *src, int start, int length) const;
  int indexOf(int start, int length, const char16_t *src, int start2,
              int length2) const;

  // @return index of char c, -1 if not found
  int indexOf(char16_t c) const;
  // @return index of char c in [start, ...), -1 if not found
  int indexOf(int start, char16_t c) const;
  // @return index of char c in [start, start+length), -1 if not found
  int indexOf(int start, int length, char16_t c) const;
  int indexOf(UChar32 c) const;
  int indexOf(int start, UChar32 c) const;
  int indexOf(int start, int length, UChar32 c) const;

  // @return last index of src, -1 if not found
  int lastIndexOf(const Line &src) const;
  // @return last index of src[start, start+length), -1 if not found
  int lastIndexOf(const Line &src, int start, int length) const;
  // @return last index of src[start2, start2+length2) in [start, start+length)
  //         -1 if not found
  int lastIndexOf(int start, int length, const Line &src, int start2,
                  int length2) const;
  int lastIndexOf(const char16_t *src) const;
  int lastIndexOf(const char16_t *src, int start, int length) const;
  int lastIndexOf(int start, int length, const char16_t *src, int start2,
                  int length2) const;

  // @return last index of char c, -1 if not found
  int lastIndexOf(char16_t c) const;
  // @return last index of char c in [start, ...), -1 if not found
  int lastIndexOf(int start, char16_t c) const;
  // @return last index of char c in [start, start+length), -1 if not found
  int lastIndexOf(int start, int length, char16_t c) const;
  int lastIndexOf(UChar32 c) const;
  int lastIndexOf(int start, UChar32 c) const;
  int lastIndexOf(int start, int length, UChar32 c) const;

  /**
   * indexing
   */

  char16_t charAt(int offset) const;
  char16_t operator[](int offset) const;
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
  // copy [start, start+length) to dest
  // @return copyied chars
  int extract(int start, int length, char16_t *dest) const;
  // copy [start, start+length) to dest[start2, start2+length2)
  int extract(int start, int length, char16_t *dest, int start2,
              int length2) const;
  int extract(int start, int length, Line &dest) const;
  // @return range [start, ...)
  Line subString(int start) const;
  // @return range [start, start+length)
  Line subString(int start, int length) const;
  std::string toUTF8() const;

  /**
   * attribute
   */
  // @return string length, not unicode unit count
  int length() const;
  int capacity() const;
  // @return char32 count, e.g unicode unit count
  int countChar32() const;
  // @return range [start, ...) char32 count
  int countChar32(int start) const;
  // @return range [start, start+length) char32 count
  int countChar32(int start, int length) const;
  bool empty() const;
  bool bogus() const;

  virtual std::string toString() const;

private:
  int32_t lineNumber_;
  icu::UnicodeString buffer_;

  friend class File;
};

} // namespace fastype
