// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "Logging.h"
#include <algorithm>
#include <boost/align/align_up.hpp>
#include <cctype>
#include <cstring>
#include <fmt/format.h>
#include <utility>
#include <vector>

#define F_ALIGN_UP 8
#define F_ANY_CHAR '\0'

namespace fastype {

Cowstr::Cowstr()
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, 0))) {
  F_INFOF("Constructor:{}", toString());
}

Cowstr::Cowstr(int capacity)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, capacity))) {
  F_INFOF("Capacity Constructor:{}", toString());
}

Cowstr::Cowstr(const char *s, int n)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, n))) {
  std::memcpy(dataImpl(), s, n);
  sizeImpl() = n;
  F_INFOF("raw char pointer Constructor:{}", toString());
}

Cowstr::Cowstr(char c)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, 1))) {
  *dataImpl() = c;
  sizeImpl() = 1;
  F_INFOF("single char Constructor:{}", toString());
}

Cowstr::Cowstr(const std::string &s)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, s.length()))) {
  std::memcpy(dataImpl(), s.data(), s.length());
  sizeImpl() = s.length();
  F_INFOF("std::string Constructor:{}", toString());
}

Cowstr::~Cowstr() {}

void Cowstr::copyOnWrite() {
  // if has multiple references, allocate memory and deep copy value
  // else modify this value directly
  if (impl_.use_count() > 1) {
    Cowstr::CowStrImpl *r = alloc(nullptr, size());
    memoryCopy(r, head(), size());
    impl_ = std::shared_ptr<Cowstr::CowStrImpl>(r);
  }
}

Cowstr::Cowstr(const Cowstr &other) : impl_(other.impl_) {
  F_INFOF("Copy Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(const Cowstr &other) {
  if (this == &other) {
    return *this;
  }
  impl_ = other.impl_;
  F_INFOF("Copy Assign: {}", toString());
  return *this;
}

Cowstr::Cowstr(Cowstr &&other) : Cowstr() {
  swap(other);
  F_INFOF("Move Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(Cowstr &&other) {
  if (this == &other) {
    return *this;
  }
  swap(other);
  F_INFOF("Copy Move : {}", toString());
  return *this;
}

void Cowstr::swap(Cowstr &s) { std::swap(impl_, s.impl_); }

Cowstr Cowstr::concat(const Cowstr &s) const {
  return concat(s.head(), s.size());
}

Cowstr Cowstr::concat(const std::string &s) const {
  return concat(s.data(), s.length());
}

Cowstr Cowstr::concat(const char *s, int n) const {
  F_CHECKF(s != nullptr, "s {} != nullptr", (void *)s);
  F_CHECKF(n >= 0, "n {} >= 0", n);
  Cowstr::CowStrImpl *p = alloc(nullptr, size() + n);
  memoryCopy(p, head(), size());
  memoryCopy(p, s, n);
  return Cowstr(p);
}

Cowstr Cowstr::operator+(const Cowstr &s) const { return concat(s); }

Cowstr Cowstr::operator+(const std::string &s) const { return concat(s); }

Cowstr &Cowstr::operator+=(const Cowstr &s) {
  copyOnWrite();
  Cowstr::CowStrImpl *p = alloc(impl_.get(), size() + s.size());
  F_CHECKF(p != nullptr, "p {} != nullptr", (void *)p);
  if (!p) {
    F_ERRORF("+= alloc failure:{}", toString());
  }
  std::memcpy(dataImpl() + sizeImpl(), s.head(), s.size());
  sizeImpl() += s.size();
  return *this;
}

Cowstr &Cowstr::operator+=(const std::string &s) {
  copyOnWrite();
  Cowstr::CowStrImpl *p = alloc(impl_.get(), size() + s.length());
  F_CHECKF(p != nullptr, "p {} != nullptr", (void *)p);
  if (!p) {
    F_ERRORF("+= alloc failure:{}", toString());
  }
  std::memcpy(dataImpl() + sizeImpl(), s.data(), s.length());
  sizeImpl() += s.length();
  return *this;
}

Cowstr Cowstr::concatHead(const Cowstr &s) const {
  return concatHead(s.head(), s.size());
}

Cowstr Cowstr::concatHead(const std::string &s) const {
  return concatHead(s.data(), s.length());
}

Cowstr Cowstr::concatHead(const char *s, int n) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, size() + n);
  std::memcpy(p->data, s, n);
  std::memcpy(p->data + n, head(), size());
  return Cowstr(p);
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

Cowstr Cowstr::replaceImpl(const Cowstr &src, const char *target, int t,
                           const char *repl, int r) {
  if (target == nullptr || t <= 0) {
    return Cowstr(src);
  }
  if (t > src.size()) {
    return Cowstr(src);
  }
  std::vector<int> pos = searchAll(src.head(), src.size(), target, t);
  if (pos.empty()) {
    return Cowstr(src);
  }
  Cowstr::CowStrImpl *p = alloc(nullptr, src.size() + pos.size() * r);
  for (int i = 0; i < (int)pos.size(); i++) {
    if (i != 0) {
      int diff = pos[i] - pos[i - 1];
      memoryCopy(p, src.head() + pos[i - 1], diff);
    }
    memoryCopy(p, repl, r);
  }
  return Cowstr(p);
}

Cowstr Cowstr::replaceFirstImpl(const Cowstr &src, const char *target, int t,
                                const char *repl, int r) {
  if (target == nullptr || t <= 0) {
    return Cowstr(src);
  }
  const char *k = kmp(src.head(), src.size(), target, t, true);
  if (!k) {
    return Cowstr(src);
  }

  Cowstr::CowStrImpl *p = alloc(nullptr, src.size() + r - 1);
  int diff1 = k - src.head();
  memoryCopy(p, src.head(), diff1);
  memoryCopy(p, repl, r);
  int diff2 = src.head() + src.size() - k - 1;
  memoryCopy(p, src.head(), diff2);
  return Cowstr(p);
}

std::vector<Cowstr> Cowstr::split(const Cowstr &s) const {
  return split(s.head(), s.size());
}

std::vector<Cowstr> Cowstr::split(const std::string &s) const {
  return split(s.data(), s.length());
}

std::vector<Cowstr> Cowstr::split(const char *s, int n) const {
  std::vector<Cowstr> ret;
  std::vector<int> pos = searchAll(head(), size(), s, n);
  for (int i = 0; i < (int)pos.size(); i++) {
    if (i == 0 && pos[i] > 0) {
      ret.push_back(subString(0, pos[i]));
    } else {
      ret.push_back(subString(pos[i - 1] + n, pos[i]));
    }
  }
  return ret;
}

Cowstr Cowstr::subString(int start) const {
  return subString(start, size() - start);
}

Cowstr Cowstr::subString(int start, int startn) const {
  F_CHECKF(start >= 0, "start {} >= 0", start);
  F_CHECKF(startn > 0, "startn {} > 0", startn);
  if (sizeImpl() < start) {
    return Cowstr();
  }
  startn = std::min<int>(startn, sizeImpl() - start);
  return Cowstr(head() + start, startn);
}

Cowstr Cowstr::upperCase() const {
  Cowstr s(head(), sizeImpl());
  std::for_each(s.head(), s.head() + s.sizeImpl(), [](char &c) {
    if (std::isupper(c)) {
      c = std::toupper(c);
    }
  });
  return s;
}

Cowstr Cowstr::lowerCase() const {
  Cowstr s(head(), sizeImpl());
  std::for_each(s.head(), s.head() + s.size(), [](char &c) {
    if (std::islower(c)) {
      c = std::tolower(c);
    }
  });
  return s;
}

static bool matchWhitespace(char a, char _) { return std::isspace(a); }
static bool matchChar(char a, char b) { return a == b; }

Cowstr Cowstr::trim() const {
  Cowstr s(head(), sizeImpl());
  trimLeftImpl(s, matchWhitespace, F_ANY_CHAR);
  trimRightImpl(s, matchWhitespace, F_ANY_CHAR);
  return s;
}

Cowstr Cowstr::trim(char c) const {
  Cowstr s(head(), sizeImpl());
  trimLeftImpl(s, matchChar, c);
  trimRightImpl(s, matchChar, c);
  return s;
}

Cowstr Cowstr::trimLeft() const {
  Cowstr s(head(), sizeImpl());
  trimLeftImpl(s, matchWhitespace, F_ANY_CHAR);
  return s;
}

Cowstr Cowstr::trimLeft(char c) const {
  Cowstr s(dataImpl(), sizeImpl());
  trimLeftImpl(s, matchChar, c);
  return s;
}

Cowstr Cowstr::trimRight() const {
  Cowstr s(dataImpl(), sizeImpl());
  trimRightImpl(s, matchWhitespace, F_ANY_CHAR);
  return s;
}

Cowstr Cowstr::trimRight(char c) const {
  Cowstr s(dataImpl(), sizeImpl());
  trimRightImpl(s, matchChar, c);
  return s;
}

char *Cowstr::head() {
  copyOnWrite();
  return dataImpl() ? dataImpl() : nullptr;
}

const char *Cowstr::head() const { return dataImpl() ? dataImpl() : nullptr; }

char *Cowstr::tail() {
  copyOnWrite();
  return dataImpl() ? (dataImpl() + sizeImpl()) : nullptr;
}

const char *Cowstr::tail() const {
  return dataImpl() ? (dataImpl() + sizeImpl()) : nullptr;
}

char *Cowstr::rawstr(int pos) {
  copyOnWrite();
  pos = pos < 0 ? (sizeImpl() + pos) : pos;
  return dataImpl() ? (dataImpl() + pos) : nullptr;
}

const char *Cowstr::rawstr(int pos) const {
  pos = pos < 0 ? (sizeImpl() + pos) : pos;
  return dataImpl() ? (dataImpl() + pos) : nullptr;
}

std::string Cowstr::stdstr(int pos) const {
  pos = pos < 0 ? (sizeImpl() + pos) : pos;
  return sizeImpl() - pos > 0
             ? std::string(dataImpl() + pos, dataImpl() + sizeImpl() - pos)
             : "";
}

// indexing
char &Cowstr::operator[](int pos) { return at(pos); }

const char &Cowstr::operator[](int pos) const { return at(pos); }

char &Cowstr::at(int pos) {
  copyOnWrite();
  return *(dataImpl() + pos);
}

const char &Cowstr::at(int pos) const { return *(dataImpl() + pos); }

bool Cowstr::empty() const { return sizeImpl() <= 0; }

int Cowstr::size() const { return sizeImpl(); }

int Cowstr::capacity() const { return capacityImpl(); }

bool Cowstr::operator==(const Cowstr &s) const { return compare(s) == 0; }

bool Cowstr::operator!=(const Cowstr &s) const { return compare(s) != 0; }

int Cowstr::compare(const Cowstr &s) const {
  if (sizeImpl() != s.sizeImpl()) {
    return sizeImpl() - s.sizeImpl();
  }
  return std::memcmp(head(), s.head(), sizeImpl());
}

int Cowstr::compare(const std::string &s) const {
  if (sizeImpl() != (int)s.length()) {
    return sizeImpl() - s.length();
  }
  return std::memcmp(head(), s.data(), sizeImpl());
}

int Cowstr::compare(const char *s, int n) const {
  if (sizeImpl() != n) {
    return sizeImpl() - n;
  }
  return std::memcmp(head(), s, sizeImpl());
}

std::string Cowstr::toString() const {
  return fmt::format(
      "[ @Cowstr impl_:{} head_:{} size_:{} capacity_:{} refcount:{} ]",
      (void *)impl_.get(), (void *)head(), size(), capacity(),
      impl_.use_count());
}

bool Cowstr::contains(const Cowstr &s, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s.head(), s.sizeImpl(), caseSensitive) !=
         nullptr;
}

bool Cowstr::contains(const char *s, int n, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s, n, caseSensitive) != nullptr;
}

bool Cowstr::contains(const std::string &s, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s.data(), s.length(), caseSensitive) !=
         nullptr;
}

bool Cowstr::startsWith(const Cowstr &s, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s.head(), s.sizeImpl(), caseSensitive) ==
         head();
}

bool Cowstr::startsWith(const char *s, int n, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s, n, caseSensitive) == head();
}

bool Cowstr::startsWith(const std::string &s, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s.data(), s.length(), caseSensitive) == head();
}

bool Cowstr::endsWith(const Cowstr &s, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s.head(), s.sizeImpl(), caseSensitive) ==
         tail() - s.sizeImpl();
}

bool Cowstr::endsWith(const std::string &s, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s.data(), s.length(), caseSensitive) ==
         tail() - s.length();
}

bool Cowstr::endsWith(const char *s, int n, bool caseSensitive) const {
  return kmp(head(), sizeImpl(), s, n, caseSensitive) == tail() - n;
}

int Cowstr::indexOf(const Cowstr &s) const {
  char *p = kmp(head(), sizeImpl(), s.head(), s.sizeImpl(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const std::string &s) const {
  char *p = kmp(head(), sizeImpl(), s.data(), s.length(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const char *s, int n) const { return indexOf(s, n, 0); }

int Cowstr::indexOf(const Cowstr &s, int fromIndex) const {
  char *p = kmp(head() + fromIndex, sizeImpl() - fromIndex, s.head(),
                s.sizeImpl(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const std::string &s, int fromIndex) const {
  char *p = kmp(head() + fromIndex, sizeImpl() - fromIndex, s.data(),
                s.length(), true);
  return p ? p - head() : -1;
}

int Cowstr::indexOf(const char *s, int n, int fromIndex) const {
  char *p = kmp(head() + fromIndex, sizeImpl() - fromIndex, s, n, true);
  return p ? p - head() : -1;
}

int Cowstr::lastIndexOf(const Cowstr &s) const {
  return lastIndexOf(s, sizeImpl());
}

int Cowstr::lastIndexOf(const std::string &s) const {
  return lastIndexOf(s, sizeImpl());
}

int Cowstr::lastIndexOf(const char *s, int n) const {
  return lastIndexOf(s, n, sizeImpl());
}

int Cowstr::lastIndexOf(const Cowstr &s, int fromIndex) const {
  fromIndex = fromIndex < 0 ? (fromIndex + sizeImpl()) : fromIndex;
  char *p = reverseSearch(head() + fromIndex, sizeImpl() - fromIndex, s.head(),
                          s.size(), true);
  return p ? p - head() : -1;
}

int Cowstr::lastIndexOf(const std::string &s, int fromIndex) const {
  fromIndex = fromIndex < 0 ? (fromIndex + sizeImpl()) : fromIndex;
  char *p = reverseSearch(head() + fromIndex, sizeImpl() - fromIndex, s.data(),
                          s.length(), true);
  return p ? p - head() : -1;
}
int Cowstr::lastIndexOf(const char *s, int n, int fromIndex) const {
  fromIndex = fromIndex < 0 ? (fromIndex + sizeImpl()) : fromIndex;
  char *p =
      reverseSearch(head() + fromIndex, sizeImpl() - fromIndex, s, n, true);
  return p ? p - head() : -1;
}

Cowstr::CowStrImpl::CowStrImpl() : data(nullptr), size(0), capacity(0) {}

Cowstr::CowStrImpl::~CowStrImpl() {
  if (data) {
    std::free(data);
    data = nullptr;
  }
  size = 0;
  capacity = 0;
}

Cowstr::CowStrImpl *Cowstr::alloc(Cowstr::CowStrImpl *p, int capacity) {
  if (!p) {
    p = new Cowstr::CowStrImpl();
  }
  F_CHECKF(p != nullptr, "p {} != nullptr", (void *)p);
  if (!p) {
    F_ERRORF("CowStrImpl new p failure, capacity:{}", capacity);
  }
  if (capacity <= 0) {
    return p;
  }

  F_CHECKF(capacity > 0, "capacity {} > 0", capacity);
  capacity = std::max<int>(
      F_ALIGN_UP, (int)boost::alignment::align_up(capacity, F_ALIGN_UP));
  F_INFOF("capacity:{}", capacity);
  F_CHECKF(capacity >= F_ALIGN_UP, "capacity {} >= F_ALIGN_UP", capacity);

  char *pd = new char[capacity];
  F_CHECKF(pd != nullptr, "pd {} != nullptr", (void *)pd);
  if (!pd) {
    F_ERRORF("CowStrImpl new pd failure, capacity:{}", capacity);
    return p;
  }
  std::memset(pd, 0, capacity * sizeof(char));
  if (p->data) {
    std::memcpy(pd, p->data, p->capacity);
    delete[] p->data;
    p->data = nullptr;
  } else {
    F_CHECKF(p->data == nullptr, "p->data {} == nullptr", (void *)p->data);
    F_CHECKF(p->capacity == 0, "p->capacity {} == 0", p->capacity);
    F_CHECKF(p->size == 0, "p->size {} == 0", p->size);
  }
  p->data = pd;
  p->capacity = capacity;
  return p;
}

Cowstr::Cowstr(Cowstr::CowStrImpl *p) : impl_(p) {
  F_INFOF("Internal shared_ptr Constructor:{}", toString());
}

void Cowstr::memoryCopy(Cowstr::CowStrImpl *p, const void *src, int n) {
  if (n > 0) {
    std::memcpy(p->data + p->size, src, n);
    p->size += n;
  }
}

void Cowstr::trimLeftImpl(Cowstr &s, bool (*match)(char, char), char t) {
  int cnt = 0;
  for (int i = 0; i < s.sizeImpl(); i++) {
    char c = s.head()[i];
    if (!match(c, t)) {
      break;
    }
    cnt++;
  }
  if (cnt > 0) {
    std::memmove(s.head(), s.head() + cnt, s.size() - cnt);
    std::memset(s.head() + s.size() - cnt, 0, s.capacity() - s.size() + cnt);
    s.sizeImpl() -= cnt;
  }
}

void Cowstr::trimRightImpl(Cowstr &s, bool (*match)(char, char), char t) {
  int cnt = 0;
  for (int i = s.size() - 1; i >= 0; i--) {
    char c = s.head()[i];
    if (!match(c, t)) {
      break;
    }
    cnt++;
  }
  if (cnt > 0) {
    std::memset(s.head() + s.size() - cnt, 0, s.capacity() - s.size() + cnt);
    s.sizeImpl() -= cnt;
  }
}

char *&Cowstr::dataImpl() { return impl_->data; }

const char *Cowstr::dataImpl() const { return impl_->data; }

int &Cowstr::sizeImpl() { return impl_->size; }

const int &Cowstr::sizeImpl() const { return impl_->size; }

int &Cowstr::capacityImpl() { return impl_->capacity; }

const int &Cowstr::capacityImpl() const { return impl_->capacity; }

std::vector<int> Cowstr::kmpPrefix(const char *needle, int n) {
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

char *Cowstr::kmp(const char *haystack, int h, const char *needle, int n,
                  bool caseSensitive) {
  if (!haystack || h <= 0 || !needle || n <= 0) {
    return nullptr;
  }
  if (n > h) {
    return nullptr;
  }

  std::vector<int> prefix = kmpPrefix(needle, n);
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

char *Cowstr::reverseSearch(const char *haystack, int h, const char *needle,
                            int n, bool caseSensitive) {
  if (!haystack || h <= 0 || !needle || n <= 0) {
    return nullptr;
  }
  for (int i = h - n; i >= 0; i--) {
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
  for (int i = 0; i < h - n; i++) {
    if (std::strncmp(haystack + i, needle, n) == 0) {
      ret.push_back(i);
    }
  }
  return ret;
}

} // namespace fastype

#undef F_ALIGN_UP
#undef F_ANY_CHAR
