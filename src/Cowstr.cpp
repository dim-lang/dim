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

#define F_ALIGN_UP 16
#define F_ANY_CHAR '\0'

namespace fastype {

Cowstr::CowStrImpl::~CowStrImpl() {
  if (data) {
    delete[] data;
    data = nullptr;
  }
  size = 0;
  capacity = 0;
}

Cowstr::Cowstr() : impl_(create(0)) { F_DEBUGF("Constructor:{}", toString()); }

Cowstr::Cowstr(int capacity) : impl_(create(capacity)) {
  F_DEBUGF("Capacity Constructor:{}", toString());
}

Cowstr::Cowstr(const char *s, int n) : impl_(create(n)) {
  std::memcpy(dataImpl(), s, n);
  sizeImpl() = n;
  F_DEBUGF("raw char pointer Constructor:{}", toString());
}

Cowstr::Cowstr(const std::string &s) : impl_(create(s.length())) {
  std::memcpy(dataImpl(), s.data(), s.length());
  sizeImpl() = s.length();
  F_DEBUGF("std::string Constructor:{}", toString());
}

Cowstr::~Cowstr() { release(impl_); }

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

bool Cowstr::empty() const { return sizeImpl() <= 0; }

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

int Cowstr::size() const { retur sizeImpl(); }

int Cowstr::capacity() const { return capacityImpl(); }

bool Cowstr::operator==(const Cowstr &s) const { return compare() == 0; }

bool Cowstr::operator!=(const Cowstr &s) const { return compare() != 0; }

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

bool Cowstr::startsWith(const char *s, int n) const {
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
  char *p = kmp(dataImpl() + fromIndex, sizeImpl() - fromIndex, s, n, true);
  return p ? p - dataImpl() : -1;
}

int Cowstr::lastIndexOf(const std::string &s, int fromIndex) const {}
int Cowstr::lastIndexOf(const char *s, int n, int fromIndex) const {}

std::shared_ptr<Cowstr::CowStrImpl> Cowstr::create(int capacity) {
  Cowstr::CowStrImpl *impl = new Cowstr::CowStrImpl();
  if (!impl) {
    return nullptr;
  }

  std::memset(impl, 0, sizeof(CowStrImpl));
  if (capacity <= 0) {
    return std::shared_ptr<Cowstr::CowStrImpl>(impl);
  }

  F_CHECKF(capacity > 0, "capacity {} > 0", capacity);
  capacity = std::max<int>(
      F_ALIGN_UP, (int)boost::alignment::align_up(capacity + 1, F_ALIGN_UP));
  F_DEBUGF("capacity:{}", capacity);
  F_CHECKF(capacity >= F_ALIGN_UP, "capacity {} >= F_ALIGN_UP", capacity);
  impl->data = (char *)malloc(capacity * sizeof(char));
  if (!impl->data) {
    return std::shared_ptr<Cowstr::CowStrImpl>(impl);
  }
  std::memset(impl->data, 0, capacity * sizeof(char));
  impl->capacity = capacity;
  return std::shared_ptr<Cowstr::CowStrImpl>(impl);
}

void Cowstr::release(std::shared_ptr<Cowstr::CowStrImpl> p) { p.reset(); }

std::shared_ptr<Cowstr::CowStrImpl>
Cowstr::copy(std::shared_ptr<Cowstr::CowStrImpl> p) {
  std::shared_ptr<Cowstr::CowStrImpl> r = create(p->size);
  std::memcpy(r->data, p->data, p->size);
  return r;
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

void Cowstr::copyOnWrite() {
  // if has multiple references, allocate memory and deep copy value
  // else modify this value directly
  if (impl_.use_count() > 1) {
    std::shared_ptr<Cowstr::CowStrImpl> p = copy(impl_);
    impl_ = p;
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
        return haystack + (i - n + 1);
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
    if ()
  }
}

} // namespace fastype

#undef F_ALIGN_UP
#undef F_ANY_CHAR
