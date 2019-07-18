// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "unicode/unistr.h"
#include "unicode/ustring.h"
#include <memory>
#include <string>

namespace fastype {

class File;
class Line;

class Line : public Logging, Stringify {
public:
  /**
   * Api Parameter Definition
   * `start` means range [start, length())
   * `start`, `length` means range [start, start+length)
   * `src` means src[0, src.length())
   * `src`, `start2`, `length2` means src[start2, start2+length2)
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

  virtual ~Line() = default;

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
  int indexOf(const char16_t *src, int start2, int length2) const;

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

  char16_t charAt(int offset) const;
  Line &setCharAt(int offset, char16_t c);
  char16_t &operator[](int offset);
  char16_t operator[](int offset) const;

  UChar32 char32At(int offset) const;
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

  // @return range [start, start+length)
  Line subString(int start) const;
  Line subString(int start, int length) const;

  std::string &toUTF8(std::string &dest) const;
  // @return char length
  // if capacity > utf8 string, null will append at the end of utf8 string
  //                            err = U_ZERO_ERROR
  // if capacity == utf8 string, null will not append at the end of utf8 string
  //                             err = U_STRING_NOT_TERMINATED
  // if capacity > utf8 string, nothing will copy to dest
  //                            err = U_BUFFER_OVERFLOW_ERROR
  int toUTF8(char *dest, int capacity, UErrorCode &err) const;

  std::u32string &toUTF32(std::u32string &dest, UErrorCode &err) const;
  // @return copied chars, including null at the end
  int toUTF32(UChar32 *dest, int capacity, UErrorCode &err) const;

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

  int lineNumber() const;
  Line &setLineNumber(int lineNumber);

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

  // @return writable raw memory pointer based on copy-on-write policy if
  // success
  //         nullptr if allocation fail
  // it will allocate new memory with `capacity`, copy original data, and
  // returns to user it is *opened* after `getBuffer(int capacity)` must *close*
  // via `releaseBuffer(int length)` before next *open*
  char16_t *getBuffer(int capacity);
  // save modifies data after user modifies this new memory, and release
  // original memory parameter `length` is new line length
  void releaseBuffer(int length);

  // @return readonly null-terminated buffer if success
  //         nullptr if allocation fail
  // normally Line define a unicode string via buffer/length pair, without null
  // at the end this method will append null at the end of buffer, e.g
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

private:
  int lineNumber_;
  icu::UnicodeString buffer_;
};

} // namespace fastype
