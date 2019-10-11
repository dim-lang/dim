// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Block.h"
#include "SmartPointer.h"
#include "Stringify.h"
#include <fmt/format.h>
#include <limits>
#include <string>
#include <vector>

namespace fastype {

/**
 * copy-on-write string
 */
class Cowstr : public Stringify {
public:
  Cowstr();
  Cowstr(int capacity);
  virtual ~Cowstr();

  /* copy */
  Cowstr(const Cowstr &s);
  Cowstr(char c);
  Cowstr(const char *s, int n);
  Cowstr(const std::string &s);
  Cowstr &operator=(const Cowstr &s);
  Cowstr &operator=(const std::string &s);

  /* move */
  Cowstr(Cowstr &&s);
  Cowstr &operator=(Cowstr &&s);

  void swap(Cowstr &s);

  /* append/concatenate */

  // concatenate s at tail
  Cowstr concat(const Cowstr &s) const;
  Cowstr concat(const char *s, int n) const;
  Cowstr concat(const std::string &s) const;
  Cowstr operator+(const Cowstr &s) const;
  Cowstr operator+(const std::string &s) const;
  // same with concat, but modify and return itself
  Cowstr &operator+=(const Cowstr &s);
  Cowstr &operator+=(const std::string &s);

  // concatenate s at head
  Cowstr concatHead(const Cowstr &s) const;
  Cowstr concatHead(const char *s, int n) const;
  Cowstr concatHead(const std::string &s) const;

  /* modify */

  // replace target with s in arbitrary position
  Cowstr replace(char target, const Cowstr &s) const;
  Cowstr replace(char target, const std::string &s) const;
  Cowstr replace(char target, const char *s, int n) const;
  Cowstr replace(const Cowstr &target, const Cowstr &s) const;
  Cowstr replace(const Cowstr &target, const std::string &s) const;
  Cowstr replace(const Cowstr &target, const char *s, int n) const;
  Cowstr replace(const std::string &target, const Cowstr &s) const;
  Cowstr replace(const std::string &target, const std::string &s) const;
  Cowstr replace(const std::string &target, const char *s, int n) const;

  // replace first target with s in arbitrary position
  Cowstr replaceFirst(char target, const Cowstr &s) const;
  Cowstr replaceFirst(char target, const std::string &s) const;
  Cowstr replaceFirst(char target, const char *s, int n) const;
  Cowstr replaceFirst(const Cowstr &target, const Cowstr &s) const;
  Cowstr replaceFirst(const Cowstr &target, const std::string &s) const;
  Cowstr replaceFirst(const Cowstr &target, const char *s, int n) const;
  Cowstr replaceFirst(const std::string &target, const Cowstr &s) const;
  Cowstr replaceFirst(const std::string &target, const std::string &s) const;
  Cowstr replaceFirst(const std::string &target, const char *s, int n) const;

  // split into multiple parts via s
  std::vector<Cowstr> split(const Cowstr &s) const;
  std::vector<Cowstr> split(const std::string &s) const;
  std::vector<Cowstr> split(const char *s, int n) const;

  // sub string of [start, ...) or [start, start+startn)
  // if cowstr has not enough length, use most it can.
  Cowstr subString(int start) const;
  Cowstr subString(int start, int startn) const;

  // to upper/lower case
  Cowstr upperCase() const;
  Cowstr lowerCase() const;

  // remove all whitespaces/target on both left/right side
  Cowstr trim() const;
  Cowstr trim(char target) const;
  // remove all whitespaces/target on left side
  Cowstr trimLeft() const;
  Cowstr trimLeft(char target) const;
  // remove all whitespaces/target on right side
  Cowstr trimRight() const;
  Cowstr trimRight(char target) const;

  /* read api */

  // get stringify snapshot
  virtual std::string toString() const;

  // raw data pointer at head
  char *head();
  const char *head() const;

  // raw data pointer at tail
  char *tail();
  const char *tail() const;

  // raw string pointer at position
  char *rawstr(int pos = 0);
  const char *rawstr(int pos = 0) const;
  std::string stdstr(int pos = 0) const;

  // indexing
  char &operator[](int pos);
  const char &operator[](int pos) const;
  char &at(int pos);
  const char &at(int pos) const;

  // test if cowstr has no data
  bool empty() const;

  // cowstr string length
  int size() const;

  // allocated memory size
  int capacity() const;

  /* comparation */
  bool operator==(const Cowstr &s) const;
  bool operator!=(const Cowstr &s) const;
  int compare(const Cowstr &s) const;
  int compare(const std::string &s) const;
  int compare(const char *s, int n) const;

  /* search */

  // test if contains s
  // caseXXX api means compare string ignore case
  bool contains(const Cowstr &s, bool caseSensitive = true) const;
  bool contains(const std::string &s, bool caseSensitive = true) const;
  bool contains(const char *s, int n, bool caseSensitive = true) const;

  // test if starts with s
  bool startsWith(const Cowstr &s, bool caseSensitive = true) const;
  bool startsWith(const std::string &s, bool caseSensitive = true) const;
  bool startsWith(const char *s, int n, bool caseSensitive = true) const;

  // test if ends with s
  bool endsWith(const Cowstr &s, bool caseSensitive = true) const;
  bool endsWith(const std::string &s, bool caseSensitive = true) const;
  bool endsWith(const char *s, int n, bool caseSensitive = true) const;

  // search position of s
  // @param fromIndex   search start from `fromIndex`
  int indexOf(const Cowstr &s) const;
  int indexOf(const std::string &s) const;
  int indexOf(const char *s, int n) const;
  int indexOf(const Cowstr &s, int fromIndex) const;
  int indexOf(const std::string &s, int fromIndex) const;
  int indexOf(const char *s, int n, int fromIndex) const;

  // reverse search position of s
  // @param fromIndex   search start from `fromIndex`
  // NOTICE:            fromIndex can be negative, then the real from position
  //                    will add `size()`. for example, when fromIndex = -1, the
  //                    real from position is `size() - 1`.
  int lastIndexOf(const Cowstr &s) const;
  int lastIndexOf(const std::string &s) const;
  int lastIndexOf(const char *s, int n) const;
  int lastIndexOf(const Cowstr &s, int fromIndex) const;
  int lastIndexOf(const std::string &s, int fromIndex) const;
  int lastIndexOf(const char *s, int n, int fromIndex) const;

  /* format */

  template <typename... Args>
  static Cowstr format(const Cowstr &s, const Args &... args) {
    std::string ff = fmt::format(s.stdstr(), args...);
    return Cowstr(ff);
  }

  template <typename... Args>
  static Cowstr format(const std::string &s, const Args &... args) {
    std::string ff = fmt::format(s, args...);
    return Cowstr(ff);
  }

  template <typename... Args>
  static Cowstr format(const char *s, const Args &... args) {
    std::string ff = fmt::format(s, args...);
    return Cowstr(ff);
  }

  static std::vector<int> searchAll(const char *haystack, int h,
                                    const char *needle, int n);

private:
  // helper constructor
  Cowstr(Block *b);

  // trim implementation
  static void trimLeftImpl(Cowstr &s, bool (*match)(char, char), char t);
  static void trimRightImpl(Cowstr &s, bool (*match)(char, char), char t);

  // search implementation
  static std::vector<int> kmpPrefix(const char *needle, int n);

  static char *kmp(const char *haystack, int h, const char *needle, int n,
                   bool caseSensitive);

  static char *reverseSearch(const char *haystack, int h, const char *needle,
                             int n, bool caseSensitive);

  // replace implementation
  // search `target` in `src`,
  // if find any, replace the first `target` with `repl`.
  //
  // @param src      source string
  // @param target   target string
  // @param t        target string length
  // @param repl     replace string
  // @param r        replace string length
  static Cowstr replaceImpl(const Cowstr &src, const char *target, int t,
                            const char *repl, int r);
  static Cowstr replaceFirstImpl(const Cowstr &src, const char *target, int t,
                                 const char *repl, int r);

  void copyOnWrite();

  Sptr<Block> buf_;
};

} // namespace fastype
