// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "Logging.h"
#include "exception/ParseException.h"
#include <algorithm>
#include <boost/align/align_up.hpp>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <string>
#include <utility>
#include <vector>

namespace fastype {

Cowstr::Cowstr() : buf_(new Block()) { F_INFO("Constructor:{}", toString()); }

Cowstr::Cowstr(int capacity) : buf_(new Block(capacity)) {
  F_INFO("Capacity Constructor:{}", toString());
}

Cowstr::Cowstr(const char *s, int n) : buf_(new Block(s, n)) {
  F_INFO("raw char pointer Constructor:{}", toString());
}

Cowstr::Cowstr(char c) : buf_(new Block(c)) {
  F_INFO("single char Constructor:{}", toString());
}

Cowstr::Cowstr(const std::string &s) : buf_(new Block(s)) {
  F_INFO("std::string Constructor:{}", toString());
}

Cowstr::~Cowstr() { F_INFO("Destructor:{}", toString()); }

void Cowstr::copyOnWrite() {
  // if has multiple references, allocate memory and deep copy value
  // else modify this value directly
  if (buf_.useCount() > 1) {
    Block *b = new Block(*buf_);
    F_CHECK(b != nullptr, "b {} != nullptr", (void *)b);
    buf_.reset(b);
  }
}

Cowstr::Cowstr(const Cowstr &other) : buf_(other.buf_) {
  F_INFO("Copy Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(const Cowstr &other) {
  if (this == &other) {
    return *this;
  }
  buf_ = other.buf_;
  F_INFO("Copy Assign: {}", toString());
  return *this;
}

Cowstr::Cowstr(Cowstr &&other) : Cowstr() {
  swap(other);
  F_INFO("Move Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(Cowstr &&other) {
  if (this == &other) {
    return *this;
  }
  swap(other);
  F_INFO("Copy Move : {}", toString());
  return *this;
}

void Cowstr::swap(Cowstr &s) { std::swap(buf_, s.buf_); }

Cowstr Cowstr::concat(const Cowstr &s) const {
  return concat(s.head(), s.size());
}

Cowstr Cowstr::concat(const std::string &s) const {
  return concat(s.data(), s.length());
}

Cowstr Cowstr::concat(const char *s, int n) const {
  F_CHECK(n >= 0, "n {} >= 0", n);
  if (n == 0 || !s) {
    return *this;
  }
  Block *b = new Block(size() + n);
  b->concat(head(), size());
  b->concat(s, n);
  return Cowstr(b);
}

Cowstr Cowstr::operator+(const Cowstr &s) const { return concat(s); }

Cowstr Cowstr::operator+(const std::string &s) const { return concat(s); }

Cowstr &Cowstr::operator+=(const Cowstr &s) {
  copyOnWrite();
  buf_->concat(*s.buf_);
  return *this;
}

Cowstr &Cowstr::operator+=(const std::string &s) {
  copyOnWrite();
  buf_->concat(s);
  return *this;
}

Cowstr Cowstr::concatHead(const Cowstr &s) const {
  return concatHead(s.head(), s.size());
}

Cowstr Cowstr::concatHead(const std::string &s) const {
  return concatHead(s.data(), s.length());
}

Cowstr Cowstr::concatHead(const char *s, int n) const {
  Block *b = new Block(head(), size());
  b->concatHead(s, n);
  return Cowstr(b);
}

Cowstr Cowstr::replace(char target, const Cowstr &s) const {
  return replaceImpl(*this, &target, 1, s.head(), s.size());
}

Cowstr Cowstr::replace(char target, const std::string &s) const {
  return replaceImpl(*this, &target, 1, s.data(), s.length());
}

Cowstr Cowstr::replace(char target, const char *s, int n) const {
  return replaceImpl(*this, &target, 1, s, n);
}

Cowstr Cowstr::replace(const Cowstr &target, const Cowstr &s) const {
  return replaceImpl(*this, target.head(), target.size(), s.head(), s.size());
}

Cowstr Cowstr::replace(const Cowstr &target, const std::string &s) const {
  return replaceImpl(*this, target.head(), target.size(), s.data(), s.length());
}

Cowstr Cowstr::replace(const Cowstr &target, const char *s, int n) const {
  return replaceImpl(*this, target.head(), target.size(), s, n);
}

Cowstr Cowstr::replace(const std::string &target, const Cowstr &s) const {
  return replaceImpl(*this, target.data(), target.length(), s.head(), s.size());
}

Cowstr Cowstr::replace(const std::string &target, const std::string &s) const {
  return replaceImpl(*this, target.data(), target.length(), s.data(),
                     s.length());
}

Cowstr Cowstr::replace(const std::string &target, const char *s, int n) const {
  return replaceImpl(*this, target.data(), target.length(), s, n);
}

Cowstr Cowstr::replaceFirst(char target, const Cowstr &s) const {
  return replaceFirstImpl(*this, &target, 1, s.head(), s.size());
}

Cowstr Cowstr::replaceFirst(char target, const std::string &s) const {
  return replaceFirstImpl(*this, &target, 1, s.data(), s.length());
}

Cowstr Cowstr::replaceFirst(char target, const char *s, int n) const {
  return replaceFirstImpl(*this, &target, 1, s, n);
}

Cowstr Cowstr::replaceFirst(const Cowstr &target, const Cowstr &s) const {
  return replaceFirstImpl(*this, target.head(), target.size(), s.head(),
                          s.size());
}

Cowstr Cowstr::replaceFirst(const Cowstr &target, const std::string &s) const {
  return replaceFirstImpl(*this, target.head(), target.size(), s.data(),
                          s.length());
}

Cowstr Cowstr::replaceFirst(const Cowstr &target, const char *s, int n) const {
  return replaceFirstImpl(*this, target.head(), target.size(), s, n);
}

Cowstr Cowstr::replaceFirst(const std::string &target, const Cowstr &s) const {
  return replaceFirstImpl(*this, target.data(), target.length(), s.head(),
                          s.size());
}

Cowstr Cowstr::replaceFirst(const std::string &target,
                            const std::string &s) const {
  return replaceFirstImpl(*this, target.data(), target.length(), s.data(),
                          s.length());
}

Cowstr Cowstr::replaceFirst(const std::string &target, const char *s,
                            int n) const {
  return replaceFirstImpl(*this, target.data(), target.length(), s, n);
}

Cowstr Cowstr::replaceImpl(const Cowstr &src, const char *target, int t,
                           const char *repl, int r) {
  if (!target || t <= 0) {
    return Cowstr(src);
  }
  if (t > src.size()) {
    return Cowstr(src);
  }
  std::vector<int> pos = searchAll(src.head(), src.size(), target, t);
  if (pos.empty()) {
    return Cowstr(src);
  }
  Block *b = new Block((int)(src.size() + pos.size() * r));

  int u = 0, v = pos[0];
  for (int i = 0; i < (int)pos.size(); i++) {
    b->concat(src.head() + u, v - u);
    b->concat(repl, r);
    u = v + t;
    v = pos[i + 1];
  }
  if (u < src.size()) {
    b->concat(src.head() + u, src.size() - u);
  }
  return Cowstr(b);
}

Cowstr Cowstr::replaceFirstImpl(const Cowstr &src, const char *target, int t,
                                const char *repl, int r) {
  if (!target || t <= 0) {
    return Cowstr(src);
  }
  const char *k = kmpSearch(src.head(), src.size(), target, t, true);
  if (!k) {
    return Cowstr(src);
  }

  Block *b = new Block(src.size() + r - 1);
  int diff1 = k - src.head();
  b->concat(src.head(), diff1);
  b->concat(repl, r);
  int diff2 = src.size() - diff1 - t;
  b->concat(src.head() + diff1 + t, diff2);
  return Cowstr(b);
}

std::vector<Cowstr> Cowstr::split(const Cowstr &s) const {
  return split(s.head(), s.size());
}

std::vector<Cowstr> Cowstr::split(const std::string &s) const {
  return split(s.data(), s.length());
}

std::vector<Cowstr> Cowstr::split(const char *s, int n) const {
  std::vector<Cowstr> ret;

  // if split string is a empty string, split into single char
  if (!s || n <= 0) {
    for (int i = 0; i < size(); i++) {
      ret.push_back(Cowstr(at(i)));
    }
    return ret;
  }

  std::vector<int> pos = searchAll(head(), size(), s, n);
  F_INFO("split searchAll position#size:{}", pos.size());
  for (int i = 0; i < (int)pos.size(); i++) {
    F_INFO("split searchAll, pos[{}]:{}", i, pos[i]);
  }
  if (pos.empty()) {
    ret.push_back(*this);
    return ret;
  }

  int i;
  for (i = 0; i < (int)pos.size(); i++) {
    if (i == 0) {
      ret.push_back(pos[i] > 0 ? subString(0, pos[i]) : Cowstr());
    } else {
      ret.push_back(subString(pos[i - 1] + n, pos[i] - pos[i - 1] - n));
    }
  }
  if (pos[pos.size() - 1] < size()) {
    ret.push_back(subString(pos[pos.size() - 1] + n));
  }
  return ret;
}

Cowstr Cowstr::subString(int start) const {
  return subString(start, size() - start);
}

Cowstr Cowstr::subString(int start, int startn) const {
  F_CHECK(start >= 0, "start {} >= 0", start);
  F_CHECK(startn >= 0, "startn {} >= 0", startn);
  if (size() < start) {
    return Cowstr();
  }
  if (startn == 0) {
    return Cowstr();
  }
  startn = std::min<int>(startn, size() - start);
  return Cowstr(head() + start, startn);
}

Cowstr Cowstr::upperCase() const {
  Cowstr s(head(), size());
  std::transform(
      s.head(), s.head() + s.size(), s.head(),
      [](unsigned char c) -> unsigned char { return std::toupper(c); });
  return s;
}

Cowstr Cowstr::lowerCase() const {
  Cowstr s(head(), size());
  std::transform(
      s.head(), s.head() + s.size(), s.head(),
      [](unsigned char c) -> unsigned char { return std::tolower(c); });
  return s;
}

Cowstr Cowstr::trim() const {
  if (empty()) {
    return *this;
  }
  if (!std::isspace(at(0)) && !std::isspace(at(size() - 1))) {
    return *this;
  }
  Cowstr s(head(), size());
  trimLeftImpl(s);
  trimRightImpl(s);
  return s;
}

Cowstr Cowstr::trimLeft() const {
  if (empty()) {
    return *this;
  }
  if (!std::isspace(at(0))) {
    return *this;
  }
  Cowstr s(head(), size());
  trimLeftImpl(s);
  return s;
}

Cowstr Cowstr::trimRight() const {
  if (empty()) {
    return *this;
  }
  if (!std::isspace(at(size() - 1))) {
    return *this;
  }
  Cowstr s(head(), size());
  trimRightImpl(s);
  return s;
}

char *Cowstr::head() {
  copyOnWrite();
  return buf_->head() ? buf_->head() : nullptr;
}

const char *Cowstr::head() const { return buf_->head(); }

char *Cowstr::tail() {
  copyOnWrite();
  return buf_->head() ? (buf_->head() + buf_->size()) : nullptr;
}

const char *Cowstr::tail() const {
  return buf_->head() ? (buf_->head() + buf_->size()) : nullptr;
}

char *Cowstr::rawstr(int pos) {
  copyOnWrite();
  pos = pos < 0 ? (buf_->size() + pos) : pos;
  return buf_->head() ? (buf_->head() + pos) : nullptr;
}

const char *Cowstr::rawstr(int pos) const {
  pos = pos < 0 ? (buf_->size() + pos) : pos;
  return buf_->head() ? (buf_->head() + pos) : nullptr;
}

std::string Cowstr::stdstr(int pos) const {
  pos = pos < 0 ? (buf_->size() + pos) : pos;
  return buf_->size() - pos > 0 ? std::string(buf_->head() + pos,
                                              buf_->head() + buf_->size() - pos)
                                : "";
}

// indexing
char &Cowstr::operator[](int pos) { return at(pos); }

const char &Cowstr::operator[](int pos) const { return at(pos); }

char &Cowstr::at(int pos) {
  copyOnWrite();
  return *(buf_->head() + pos);
}

const char &Cowstr::at(int pos) const { return *(buf_->head() + pos); }

bool Cowstr::empty() const { return buf_->size() <= 0; }

int Cowstr::size() const { return buf_->size(); }

int Cowstr::capacity() const { return buf_->capacity(); }

bool Cowstr::operator==(const Cowstr &s) const { return compare(s) == 0; }
bool Cowstr::operator==(const std::string &s) const { return compare(s) == 0; }

bool Cowstr::operator!=(const Cowstr &s) const { return compare(s) != 0; }
bool Cowstr::operator!=(const std::string &s) const { return compare(s) != 0; }

bool Cowstr::operator<(const Cowstr &s) const { return compare(s) < 0; }
bool Cowstr::operator<(const std::string &s) const { return compare(s) < 0; }

bool Cowstr::operator<=(const Cowstr &s) const { return compare(s) <= 0; }
bool Cowstr::operator<=(const std::string &s) const { return compare(s) <= 0; }

bool Cowstr::operator>(const Cowstr &s) const { return compare(s) > 0; }
bool Cowstr::operator>(const std::string &s) const { return compare(s) > 0; }

bool Cowstr::operator>=(const Cowstr &s) const { return compare(s) >= 0; }
bool Cowstr::operator>=(const std::string &s) const { return compare(s) >= 0; }

int Cowstr::compare(const Cowstr &s) const {
  if (size() != s.size()) {
    return size() - s.size();
  }
  return std::memcmp(head(), s.head(), size());
}

int Cowstr::compare(const std::string &s) const {
  if (size() != (int)s.length()) {
    return size() - s.length();
  }
  return std::memcmp(head(), s.data(), size());
}

int Cowstr::compare(const char *s, int n) const {
  if (size() != n) {
    return size() - n;
  }
  return std::memcmp(head(), s, size());
}

std::string Cowstr::toString() const {
  return fmt::format(
      "[ @Cowstr buf_:{} head:{} size:{} capacity:{} useCount:{} ]",
      buf_ ? buf_->toString() : "null", (void *)head(), size(), capacity(),
      buf_.useCount());
}

int Cowstr::toInt(int base) const {
  std::string tmpStr = stdstr();
  std::size_t pos;
  return std::stoi(tmpStr, &pos, base);
}

long Cowstr::toLong(int base) const {
  std::string tmpStr = stdstr();
  std::size_t pos;
  return std::stol(tmpStr, &pos, base);
}

long long Cowstr::toLLong(int base) const {
  std::string tmpStr = stdstr();
  std::size_t pos;
  return std::stoll(tmpStr, &pos, base);
}

unsigned int Cowstr::toUInt(int base) const {
  std::string tmpStr = stdstr();
  std::size_t pos;
  return (int)std::stoul(tmpStr, &pos, base);
}

unsigned long Cowstr::toULong(int base) const {
  std::string tmpStr = stdstr();
  std::size_t pos;
  return std::stoul(tmpStr, &pos, base);
}

unsigned long long Cowstr::toULLong(int base) const {
  std::string tmpStr = stdstr();
  std::size_t pos;
  return std::stoull(tmpStr, &pos, base);
}

float Cowstr::toFloat() const { return std::stof(stdstr()); }

double Cowstr::toDouble() const { return std::stod(tmpStr, stdstr()); }

bool Cowstr::contains(const Cowstr &s, bool caseSensitive) const {
  return kmpSearch(head(), size(), s.head(), s.size(), caseSensitive) !=
         nullptr;
}

bool Cowstr::contains(const char *s, int n, bool caseSensitive) const {
  return kmpSearch(head(), size(), s, n, caseSensitive) != nullptr;
}

bool Cowstr::contains(const std::string &s, bool caseSensitive) const {
  return kmpSearch(head(), size(), s.data(), s.length(), caseSensitive) !=
         nullptr;
}

bool Cowstr::startsWith(const Cowstr &s, bool caseSensitive) const {
  return kmpSearch(head(), size(), s.head(), s.size(), caseSensitive) == head();
}

bool Cowstr::startsWith(const char *s, int n, bool caseSensitive) const {
  return kmpSearch(head(), size(), s, n, caseSensitive) == head();
}

bool Cowstr::startsWith(const std::string &s, bool caseSensitive) const {
  return kmpSearch(head(), size(), s.data(), s.length(), caseSensitive) ==
         head();
}

bool Cowstr::endsWith(const Cowstr &s, bool caseSensitive) const {
  return kmpSearch(head() + size() - s.size(), s.size(), s.head(), s.size(),
                   caseSensitive) == head() + size() - s.size();
}

bool Cowstr::endsWith(const std::string &s, bool caseSensitive) const {
  return kmpSearch(head() + size() - s.length(), s.length(), s.data(),
                   s.length(), caseSensitive) == head() + size() - s.length();
}

bool Cowstr::endsWith(const char *s, int n, bool caseSensitive) const {
  return kmpSearch(head() + size() - n, n, s, n, caseSensitive) ==
         head() + size() - n;
}

int Cowstr::indexOf(const Cowstr &s) const {
  char *p = kmpSearch(head(), size(), s.head(), s.size(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const std::string &s) const {
  char *p = kmpSearch(head(), size(), s.data(), s.length(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const char *s, int n) const { return indexOf(s, n, 0); }

int Cowstr::indexOf(const Cowstr &s, int fromIndex) const {
  char *p = kmpSearch(head() + fromIndex, size() - fromIndex, s.head(),
                      s.size(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const std::string &s, int fromIndex) const {
  char *p = kmpSearch(head() + fromIndex, size() - fromIndex, s.data(),
                      s.length(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const char *s, int n, int fromIndex) const {
  char *p = kmpSearch(head() + fromIndex, size() - fromIndex, s, n, true);
  return p ? p - head() : -1;
}

int Cowstr::lastIndexOf(const Cowstr &s) const {
  return lastIndexOf(s, size() - 1);
}

int Cowstr::lastIndexOf(const std::string &s) const {
  return lastIndexOf(s, size() - 1);
}

int Cowstr::lastIndexOf(const char *s, int n) const {
  return lastIndexOf(s, n, size() - 1);
}

int Cowstr::lastIndexOf(const Cowstr &s, int fromIndex) const {
  if (s.size() <= 0 || size() <= 0) {
    return -1;
  }
  if (size() < s.size()) {
    return -1;
  }
  fromIndex = fromIndex < 0 ? (fromIndex + size()) : fromIndex;
  char *p = reverseSearch(head(), fromIndex, s.head(), s.size(), true);
  return p ? p - head() : -1;
}

int Cowstr::lastIndexOf(const std::string &s, int fromIndex) const {
  if (s.empty() || size() <= 0) {
    return -1;
  }
  if (size() < s.length()) {
    return -1;
  }
  fromIndex = fromIndex < 0 ? (fromIndex + size()) : fromIndex;
  char *p = reverseSearch(head(), fromIndex, s.data(), s.length(), true);
  return p ? p - head() : -1;
}
int Cowstr::lastIndexOf(const char *s, int n, int fromIndex) const {
  if (!s || n <= 0 || size() <= 0) {
    return -1;
  }
  if (size() < n) {
    return -1;
  }
  fromIndex = fromIndex < 0 ? (fromIndex + size()) : fromIndex;
  char *p = reverseSearch(head(), fromIndex, s, n, true);
  return p ? p - head() : -1;
}

Cowstr::Cowstr(Block *b) : buf_(b) {
  F_INFO("Internal shared_ptr Constructor:{}", toString());
}

void Cowstr::trimLeftImpl(Cowstr &s) {
  int cnt = 0;
  for (int i = 0; i < s.size(); i++) {
    char c = s[i];
    if (!std::isspace(c)) {
      break;
    }
    cnt++;
  }
  if (cnt > 0) {
    s.buf_->removeHead(cnt);
  }
}

void Cowstr::trimRightImpl(Cowstr &s) {
  int cnt = 0;
  for (int i = s.size() - 1; i >= 0; i--) {
    char c = s[i];
    if (!std::isspace(c)) {
      break;
    }
    cnt++;
  }
  if (cnt > 0) {
    s.buf_->removeTail(cnt);
  }
}

std::vector<int> Cowstr::kmpSearchPrefix(const char *needle, int n) {
  std::vector<int> pi(n);
  int k = 0;
  for (int i = 1; i < n; i++) {
    while (k > 0 && needle[k] != needle[i])
      k = pi[k - 1];
    if (needle[k] == needle[i])
      k = k + 1;
    pi[i] = k;
  }
  return pi;
}

char *Cowstr::kmpSearch(const char *haystack, int h, const char *needle, int n,
                        bool caseSensitive) {
  if (!haystack || h <= 0 || !needle || n <= 0) {
    return nullptr;
  }
  if (n > h) {
    return nullptr;
  }

  std::vector<int> prefix = kmpSearchPrefix(needle, n);
  int i = 0, j = 0;
  while (i < h) {
    char hc = caseSensitive ? haystack[i] : std::tolower(haystack[i]);
    char nc = caseSensitive ? needle[j] : std::tolower(needle[j]);
    if (hc == nc) {
      if (j == n - 1)
        return (char *)haystack + (i - n + 1);
      j++;
      i++;
    } else if (j == 0)
      i++;
    else
      j = prefix[j - 1];
  }
  return nullptr;
}

char *Cowstr::reverseSearch(const char *haystack, int fromIndex,
                            const char *needle, int n, bool caseSensitive) {
  if (!haystack || fromIndex < 0 || !needle || n <= 0) {
    return nullptr;
  }
  for (int i = fromIndex - n + 1; i >= 0; i--) {
    bool match = true;
    for (int j = 0; j < n; j++) {
      char hc = caseSensitive ? haystack[i] : std::tolower(haystack[i]);
      char nc = caseSensitive ? needle[j] : std::tolower(needle[j]);
      if (hc != nc) {
        match = false;
        break;
      }
    }
    if (match) {
      return (char *)haystack + i;
    }
  }
  return nullptr;
}

std::vector<int> Cowstr::searchAll(const char *haystack, int h,
                                   const char *needle, int n) {
  std::vector<int> ret;
  if (!haystack || h <= 0 || !needle || n <= 0) {
    return ret;
  }
  for (int i = 0; i < h - n + 1; i++) {
    if (std::memcmp(haystack + i, needle, n) == 0) {
      ret.push_back(i);
    }
  }
  return ret;
}

} // namespace fastype
