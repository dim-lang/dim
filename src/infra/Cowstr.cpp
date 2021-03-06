// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "infra/Cowstr.h"
#include <cstring>

using blsp = Cowstr::blsp;

static blsp dupblsp(blsp s) {
  return (s && !s->empty()) ? blsp(new std::string(*s))
                            : blsp(new std::string());
}

static std::string *dupstd(const std::string &s) {
  return s.empty() ? new std::string() : new std::string(s);
}

static std::string *dupraw(const char *s, int n) {
  return (s && n > 0) ? new std::string(s, n) : new std::string();
}

static std::string *dupraw(const char *s) { return dupraw(s, std::strlen(s)); }

Cowstr::Cowstr() : value_(new std::string()) {}

Cowstr::Cowstr(const std::string &s) : value_(dupstd(s)) {}

Cowstr::Cowstr(const char *s) : value_(dupraw(s)) {}

Cowstr::Cowstr(const char *s, int n) : value_(dupraw(s, n)) {}

const std::string &Cowstr::str() const { return *value_; }

const char *Cowstr::rawstr() const {
  return empty() ? nullptr : value_->c_str();
}

int Cowstr::length() const { return (int)value_->length(); }

bool Cowstr::empty() const { return value_->empty(); }

const char &Cowstr::operator[](int index) const { return (*value_)[index]; }

const char &Cowstr::front() const { return value_->front(); }

const char &Cowstr::back() const { return value_->back(); }

const char *Cowstr::begin() const { return value_->c_str(); }

const char *Cowstr::end() const { return value_->c_str() + value_->length(); }

const char *Cowstr::rbegin() const {
  return value_->c_str() + value_->length() - 1;
}

const char *Cowstr::rend() const { return value_->c_str() - 1; }

Cowstr &Cowstr::insert(int index, const char &c, int count) {
  blsp nv = dupblsp(value_);
  nv->insert(index, count, c);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::insert(int index, const Cowstr &s) {
  blsp nv = dupblsp(value_);
  nv->insert(index, s.str());
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::erase(int index, int count) {
  blsp nv = dupblsp(value_);
  nv->erase(index, count);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::popend(int count) {
  count = std::min<int>(count, length());
  blsp nv = dupblsp(value_);
  nv->erase(length() - count, count);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::append(const char &c, int count) {
  blsp nv = dupblsp(value_);
  nv->append(count, c);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::append(const Cowstr &s) {
  blsp nv = dupblsp(value_);
  nv->append(s.str());
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::operator+=(const Cowstr &s) {
  blsp nv = dupblsp(value_);
  *nv += s.str();
  value_ = nv;
  return *this;
}

Cowstr Cowstr::operator+(const char &c) const {
  std::string r = str();
  r.append(1, c);
  return Cowstr(r);
}

Cowstr Cowstr::operator+(const Cowstr &s) const {
  std::string r = str();
  r.append(s.str());
  return Cowstr(r);
}

int Cowstr::compare(const Cowstr &s) const { return value_->compare(s.str()); }

bool Cowstr::startWith(const char &c) const { return value_->find(c) == 0; }

bool Cowstr::startWith(const Cowstr &s) const {
  return value_->find(s.str()) == 0;
}

bool Cowstr::endWith(const char &c) const {
  size_t pos = value_->rfind(c);
  int target = length() - 1;
  return pos != std::string::npos && target >= 0 && pos == (size_t)target;
  // return value_->rfind(c) == length() - 1;
}

bool Cowstr::endWith(const Cowstr &s) const {
  size_t pos = value_->rfind(s.str());
  int target = length() - s.length();
  return pos != std::string::npos && target >= 0 && pos == (size_t)target;
}

Cowstr &Cowstr::replace(const char &from, const char &to) {
  return replace(Cowstr(&from, 1), Cowstr(&to, 1));
}

Cowstr &Cowstr::replace(const Cowstr &from, const Cowstr &to) {
  if (from.empty()) {
    return *this;
  }
  std::stringstream ss;
  int i = 0;
  while (i < length()) {
    if (length() - i >= from.length() &&
        std::strncmp(value_->c_str() + i, from.rawstr(), from.length()) == 0) {
      if (!to.empty()) {
        ss << to.str();
      }
      i += from.length();
    } else {
      ss << (*value_)[i++];
    }
  }
  blsp nv(dupstd(ss.str()));
  value_ = nv;
  return *this;
}

template <typename T>
std::string replaceMap(const T &mapping, const std::string &value) {
  std::stringstream ss;
  for (int i = 0; i < value.length(); i++) {
    if (mapping.find(value[i]) != mapping.end()) {
      ss << mapping.find(value[i])->second.str();
    } else {
      ss << value[i];
    }
  }
  return ss.str();
}

Cowstr &Cowstr::replace(const std::unordered_map<char, Cowstr> &mapping) {
  if (mapping.empty()) {
    return *this;
  }
  blsp nv(dupstd(replaceMap(mapping, *value_)));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(const std::map<char, Cowstr> &mapping) {
  if (mapping.empty()) {
    return *this;
  }
  blsp nv(dupstd(replaceMap(mapping, *value_)));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(int index, int count, const char &c) {
  blsp nv = dupblsp(value_);
  nv->replace(index, count, 1, c);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(int index, int count, const Cowstr &s) {
  blsp nv = dupblsp(value_);
  nv->replace(index, count, s.str());
  value_ = nv;
  return *this;
}

Cowstr Cowstr::subString(int index) const {
  return Cowstr(value_->substr(index));
}

Cowstr Cowstr::subString(int index, int count) const {
  return Cowstr(value_->substr(index, count));
}

int Cowstr::find(const char &c, int index) const {
  size_t r = value_->find(c, index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::find(const Cowstr &s, int index) const {
  size_t r = value_->find(s.str(), index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::rfind(const char &c, int index) const {
  size_t r = value_->rfind(c, index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::rfind(const Cowstr &s, int index) const {
  size_t r = value_->rfind(s.str(), index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findFirstOf(const char &c, int index) const {
  size_t r = value_->find_first_of(c, index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findFirstOf(const Cowstr &s, int index) const {
  size_t r = value_->find_first_of(s.str(), index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findLastOf(const char &c, int index) const {
  size_t r = value_->find_last_of(c, index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findLastOf(const Cowstr &s, int index) const {
  size_t r = value_->find_last_of(s.str(), index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findFirstNotOf(const char &c, int index) const {
  size_t r = value_->find_first_not_of(c, index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findFirstNotOf(const Cowstr &s, int index) const {
  size_t r = value_->find_first_not_of(s.str(), index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findLastNotOf(const char &c, int index) const {
  size_t r = value_->find_last_not_of(c, index);
  return r == std::string::npos ? (-1) : (int)r;
}

int Cowstr::findLastNotOf(const Cowstr &s, int index) const {
  size_t r = value_->find_last_not_of(s.str(), index);
  return r == std::string::npos ? (-1) : (int)r;
}

bool Cowstr::operator==(const Cowstr &other) const {
  return value_.get() == other.value_.get() || *value_ == *other.value_;
}

bool Cowstr::operator!=(const Cowstr &other) const {
  return value_.get() != other.value_.get() && *value_ != *other.value_;
}

bool Cowstr::operator>(const Cowstr &other) const {
  return value_.get() != other.value_.get() && *value_ > *other.value_;
}

bool Cowstr::operator>=(const Cowstr &other) const {
  return *this == other || *this > other;
}

bool Cowstr::operator<(const Cowstr &other) const {
  return value_.get() != other.value_.get() && *value_ < *other.value_;
}

bool Cowstr::operator<=(const Cowstr &other) const {
  return *this == other || *this < other;
}

std::ostream &operator<<(std::ostream &os, const Cowstr &s) {
  os << s.str();
  return os;
}

Cowstr Cowstr::repeat(int n) const {
  if (empty() || n <= 0) {
    return "";
  }
  std::stringstream ss;
  for (int i = 0; i < n; i++) {
    ss << *value_;
  }
  return ss.str();
}
