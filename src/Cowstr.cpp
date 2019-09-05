// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "Logging.h"
#include "Profile.h"
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
  F_DEBUGF("Constructor:{}", toString());
}

Cowstr::Cowstr(int capacity)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, capacity))) {
  F_DEBUGF("Capacity Constructor:{}", toString());
}

Cowstr::Cowstr(const char *s, int n)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, n))) {
  std::memcpy(dataImpl(), s, n);
  sizeImpl() = n;
  F_DEBUGF("raw char pointer Constructor:{}", toString());
}

Cowstr::Cowstr(char c)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, 1))) {
  *dataImpl() = c;
  sizeImpl() = 1;
  F_DEBUGF("single char Constructor:{}", toString());
}

Cowstr::Cowstr(const std::string &s)
    : impl_(std::shared_ptr<Cowstr::CowStrImpl>(alloc(nullptr, s.length()))) {
  std::memcpy(dataImpl(), s.data(), s.length());
  sizeImpl() = s.length();
  F_DEBUGF("std::string Constructor:{}", toString());
}

Cowstr::~Cowstr() {
  if (impl_.use_count() <= 1) {
    F_CHECKF(impl_.use_count() == 1, "impl_#use_count {} == 1",
             impl_.use_count());
    F_CHECKF(impl_.get() != nullptr, "impl_#get {} != nullptr",
             (void *)impl_.get());
    release(impl_.get());
    impl_.reset();
  }
}

void Cowstr::copyOnWrite() {
  // if has multiple references, allocate memory and deep copy value
  // else modify this value directly
  if (impl_.use_count() > 1) {
    Cowstr::CowStrImpl *r = alloc(nullptr, sizeImpl());
    std::memcpy(r->data, dataImpl(), sizeImpl());
    impl_ = std::shared_ptr<Cowstr::CowStrImpl>(r);
  }
}

Cowstr::Cowstr(const Cowstr &other) : impl_(other.impl_) {
  F_DEBUGF("Copy Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(const Cowstr &other) {
  if (this == &other) {
    return *this;
  }
  impl_ = other.impl_;
  F_DEBUGF("Copy Assign: {}", toString());
  return *this;
}

Cowstr::Cowstr(Cowstr &&other) : impl_(other.impl_) {
  F_DEBUGF("Move Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(Cowstr &&other) {
  if (this == &other) {
    return *this;
  }
  impl_ = other.impl_;
  F_DEBUGF("Copy Move : {}", toString());
  return *this;
}

Cowstr Cowstr::concat(const Cowstr &s) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, sizeImpl() + s.sizeImpl());
  std::memcpy(p->data, dataImpl(), sizeImpl());
  std::memcpy(p->data + sizeImpl(), s.dataImpl(), s.sizeImpl());
  return Cowstr(p);
}

Cowstr Cowstr::concat(const char *s, int n) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, sizeImpl() + n);
  std::memcpy(p->data, dataImpl(), sizeImpl());
  std::memcpy(p->data + sizeImpl(), s, n);
  return Cowstr(p);
}

Cowstr Cowstr::concat(const std::string &s) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, sizeImpl() + s.length());
  std::memcpy(p->data, dataImpl(), sizeImpl());
  std::memcpy(p->data + sizeImpl(), s.data(), s.length());
  return Cowstr(p);
}

Cowstr Cowstr::operator+(const Cowstr &s) const {
  return concat(s.dataImpl(), s.sizeImpl());
}

Cowstr Cowstr::operator+(const std::string &s) const { return concat(s); }

Cowstr &Cowstr::operator+=(const Cowstr &s) {
  copyOnWrite();
  Cowstr::CowStrImpl *np = alloc(impl_.get(), sizeImpl() + s.sizeImpl());
  impl_.reset(np);
  std::memcpy(dataImpl() + sizeImpl(), s.dataImpl(), s.sizeImpl());
  return *this;
}

Cowstr &Cowstr::operator+=(const std::string &s) {
  copyOnWrite();
  Cowstr::CowStrImpl *np = alloc(impl_.get(), sizeImpl() + s.length());
  impl_.reset(np);
  std::memcpy(dataImpl() + sizeImpl(), s.data(), s.length());
  return *this;
}

Cowstr Cowstr::concatHead(const Cowstr &s) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, sizeImpl() + s.sizeImpl());
  std::memcpy(p->data, s.dataImpl(), s.sizeImpl());
  std::memcpy(p->data + s.sizeImpl(), dataImpl(), sizeImpl());
  return Cowstr(p);
}

Cowstr Cowstr::concatHead(const char *s, int n) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, sizeImpl() + n);
  std::memcpy(p->data, s, n);
  std::memcpy(p->data + n, dataImpl(), sizeImpl());
  return Cowstr(p);
}

Cowstr Cowstr::concatHead(const std::string &s) const {
  Cowstr::CowStrImpl *p = alloc(nullptr, sizeImpl() + s.length());
  std::memcpy(p->data, s.data(), s.length());
  std::memcpy(p->data + s.length(), dataImpl(), sizeImpl());
  return Cowstr(p);
}

Cowstr Cowstr::replace(char target, const Cowstr &s) const {
  Cowstr p = alloc(nullptr, sizeImpl());
  int last = 0;
  for (int i = 0; i < sizeImpl(); i++) {
    char c = dataImpl()[i];
    if (c != target) {
      continue;
    }
    if (i == 0) {
    }
  }
  std::memcpy(p->data, s.data(), s.length());
  std::memcpy(p->data + s.length(), dataImpl(), sizeImpl());
  return Cowstr(p);
}

Cowstr Cowstr::replace(char target, const std::string &s) const {}
Cowstr Cowstr::replace(char target, const char *s, int n) const {}
Cowstr Cowstr::replace(const Cowstr &target, const Cowstr &s) const {}
Cowstr Cowstr::replace(const Cowstr &target, const std::string &s) const {}
Cowstr Cowstr::replace(const Cowstr &target, const char *s, int n) const {}

Cowstr Cowstr::replaceFirst(char target, const Cowstr &s) const {}
Cowstr Cowstr::replaceFirst(char target, const std::string &s) const {}
Cowstr Cowstr::replaceFirst(char target, const char *s, int n) const {}
Cowstr Cowstr::replaceFirst(const Cowstr &target, const Cowstr &s) const {}
Cowstr Cowstr::replaceFirst(const Cowstr &target, const std::string &s) const {}
Cowstr Cowstr::replaceFirst(const Cowstr &target, const char *s, int n) const {}

std::vector<Cowstr> Cowstr::split(const Cowstr &s) const {}
std::vector<Cowstr> Cowstr::split(const std::string &s) const {}
std::vector<Cowstr> Cowstr::split(const char *s, int n) const {}

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
  return Cowstr(dataImpl() + start, startn);
}

Cowstr Cowstr::upperCase() const {
  Cowstr s(dataImpl(), sizeImpl());
  std::for_each(s.dataImpl(), s.dataImpl() + s.sizeImpl(), [](char &c) {
    if (std::isupper(c)) {
      c = std::toupper(c);
    }
  });
  return s;
}

Cowstr Cowstr::lowerCase() const {
  Cowstr s(dataImpl(), sizeImpl());
  std::for_each(s.dataImpl(), s.dataImpl() + s.sizeImpl(), [](char &c) {
    if (std::islower(c)) {
      c = std::tolower(c);
    }
  });
  return s;
}

static bool matchWhitespace(char a, char _) { return std::isspace(a); }
static bool matchChar(char a, char b) { return a == b; }

Cowstr Cowstr::trim() const {
  Cowstr s(dataImpl(), sizeImpl());
  trimLeftImpl(s, matchWhitespace, F_ANY_CHAR);
  trimRightImpl(s, matchWhitespace, F_ANY_CHAR);
  return s;
}

Cowstr Cowstr::trim(char c) const {
  Cowstr s(dataImpl(), sizeImpl());
  trimLeftImpl(s, matchChar, c);
  trimRightImpl(s, matchChar, c);
  return s;
}

Cowstr Cowstr::trimLeft() const {
  Cowstr s(dataImpl(), sizeImpl());
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
  return dataImpl();
}

const char *Cowstr::head() const { return dataImpl(); }

char *Cowstr::tail() {
  copyOnWrite();
  return dataImpl();
}

const char *Cowstr::tail() const { return dataImpl() + sizeImpl(); }

char *Cowstr::rawstr(int pos) {
  copyOnWrite();
  pos = pos < 0 ? (sizeImpl() + pos) : pos;
  return dataImpl() + pos;
}

const char *Cowstr::rawstr(int pos) const {
  pos = pos < 0 ? (sizeImpl() + pos) : pos;
  return dataImpl() + pos;
}

std::string Cowstr::stdstr(int pos) const {
  pos = pos < 0 ? (sizeImpl() + pos) : pos;
  return std::string(dataImpl() + pos, dataImpl() + sizeImpl() - pos);
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
  return std::memcmp(dataImpl(), s.dataImpl(), sizeImpl());
}

int Cowstr::compare(const std::string &s) const {
  if (sizeImpl() != s.length()) {
    return sizeImpl() - s.length();
  }
  return std::memcmp(dataImpl(), s.data(), sizeImpl());
}

int Cowstr::compare(const char *s, int n) const {
  if (sizeImpl() != n) {
    return sizeImpl() - n;
  }
  return std::memcmp(dataImpl(), s, sizeImpl());
}

std::string Cowstr::toString() const {
  return fmt::format("[ @Cowstr data_:{} size_:{} capacity_:{} refcount:{} ]",
                     (void *)dataImpl(), sizeImpl(), capacityImpl(),
                     impl_.use_count());
}

bool Cowstr::contains(const Cowstr &s, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s.dataImpl(), s.sizeImpl(),
             caseSensitive) != nullptr;
}

bool Cowstr::contains(const char *s, int n, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s, n, caseSensitive) != nullptr;
}

bool Cowstr::contains(const std::string &s, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s.data(), s.length(), caseSensitive) !=
         nullptr;
}

bool Cowstr::startsWith(const Cowstr &s, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s.dataImpl(), s.sizeImpl(),
             caseSensitive) == dataImpl();
}

bool Cowstr::startsWith(const char *s, int n, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s, n, caseSensitive) == dataImpl();
}

bool Cowstr::startsWith(const std::string &s, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s.data(), s.length(), caseSensitive) ==
         dataImpl();
}

bool Cowstr::endsWith(const Cowstr &s, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s.dataImpl(), s.sizeImpl(),
             caseSensitive) == tail() - s.sizeImpl();
}

bool Cowstr::endsWith(const std::string &s, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s.data(), s.length(), caseSensitive) ==
         tail() - s.length();
}

bool Cowstr::endsWith(const char *s, int n, bool caseSensitive) const {
  return kmp(dataImpl(), sizeImpl(), s, n, caseSensitive) == tail() - n;
}

int Cowstr::indexOf(const Cowstr &s) const {
  char *p = kmp(dataImpl(), sizeImpl(), s.dataImpl(), s.sizeImpl(), true);
  return p ? p - dataImpl() : -1;
}

int Cowstr::indexOf(const std::string &s) const {
  char *p = kmp(dataImpl(), sizeImpl(), s.data(), s.length(), true);
  return p ? p - dataImpl() : -1;
}

int Cowstr::indexOf(const char *s, int n) const { return indexOf(s, n, 0); }

int Cowstr::indexOf(const Cowstr &s, int fromIndex) const {
  char *p = kmp(dataImpl() + fromIndex, sizeImpl() - fromIndex, s.dataImpl(),
                s.sizeImpl(), true);
  return p ? p - dataImpl() : -1;
}

int Cowstr::indexOf(const std::string &s, int fromIndex) const {
  char *p = kmp(dataImpl() + fromIndex, sizeImpl() - fromIndex, s.data(),
                s.length(), true);
  return p ? p - dataImpl() : -1;
}

int Cowstr::indexOf(const char *s, int n, int fromIndex) const {
  char *p = kmp(dataImpl() + fromIndex, sizeImpl() - fromIndex, s, n, true);
  return p ? p - dataImpl() : -1;
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
  char *p = reverseSearch(dataImpl() + fromIndex, sizeImpl() - fromIndex,
                          s.dataImpl(), s.sizeImpl(), true);
  return p ? p - dataImpl() : -1;
}

int Cowstr::lastIndexOf(const std::string &s, int fromIndex) const {
  fromIndex = fromIndex < 0 ? (fromIndex + sizeImpl()) : fromIndex;
  char *p = reverseSearch(dataImpl() + fromIndex, sizeImpl() - fromIndex,
                          s.data(), s.length(), true);
  return p ? p - dataImpl() : -1;
}
int Cowstr::lastIndexOf(const char *s, int n, int fromIndex) const {
  fromIndex = fromIndex < 0 ? (fromIndex + sizeImpl()) : fromIndex;
  char *p =
      reverseSearch(dataImpl() + fromIndex, sizeImpl() - fromIndex, s, n, true);
  return p ? p - dataImpl() : -1;
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
  F_CHECK(p != nullptr, "p is null");
  if (!p) {
    F_ERRORF("CowStrImpl new failure, capacity:{}", capacity);
  }
  if (capacity <= 0) {
    return p;
  }

  F_CHECKF(capacity > 0, "capacity {} > 0", capacity);
  capacity = std::max<int>(
      F_ALIGN_UP, (int)boost::alignment::align_up(capacity, F_ALIGN_UP));
  F_DEBUGF("capacity:{}", capacity);
  F_CHECKF(capacity >= F_ALIGN_UP, "capacity {} >= F_ALIGN_UP", capacity);
  char *pd = (char *)realloc(p->data, capacity * sizeof(char));
  if (!pd) {
    return p;
  }
  p->data = pd;
  p->capacity = capacity;
  return p;
}

void Cowstr::release(Cowstr::CowStrImpl *p) {
  if (!p) {
    return;
  }
  if (p->data) {
    std::free(p->data);
    p->data = nullptr;
  }
  p->size = 0;
  p->capacity = 0;
  std::free(p);
}

Cowstr::Cowstr(Cowstr::CowStrImpl *p) : impl_(p) {
  F_DEBUGF("Internal shared_ptr Constructor:{}", toString());
}

void Cowstr::trimLeftImpl(Cowstr &s, bool (*match)(char, char), char t) {
  int count = 0;
  for (int i = 0; i < s.sizeImpl(); i++) {
    char c = s.dataImpl()[i];
    if (!match(c, t)) {
      break;
    }
    count++;
  }
  if (count > 0) {
    std::memmove(s.dataImpl(), s.dataImpl() + count, s.sizeImpl() - count);
    std::memset(s.dataImpl() + s.sizeImpl() - count, 0,
                s.capacityImpl() - s.sizeImpl() + count);
    s.sizeImpl() -= count;
  }
}

void Cowstr::trimRightImpl(Cowstr &s, bool (*match)(char, char), char t) {
  int count = 0;
  for (int i = s.sizeImpl() - 1; i >= 0; i--) {
    char c = s.dataImpl()[i];
    if (!match(c, t)) {
      break;
    }
    count++;
  }
  if (count > 0) {
    std::memset(s.dataImpl() + s.sizeImpl() - count, 0,
                s.capacityImpl() - s.sizeImpl() + count);
    s.sizeImpl() -= count;
  }
}

char *&Cowstr::dataImpl() { return impl_->data; }

int &Cowstr::sizeImpl() { return impl_->size; }

int &Cowstr::capacityImpl() { return impl_->capacity; }

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
  if (n <= 0 || h <= 0)
    return nullptr;
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
  if (h <= 0 || n <= 0)
    return nullptr;

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

} // namespace fastype

#undef F_ALIGN_UP
#undef F_ANY_CHAR
