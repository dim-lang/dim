// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Cowstr.h"
#include <algorithm>
#include <cstring>
#include <sstream>
using sps = std::shared_ptr<std::string>;

static std::string mergeCowstrs(std::vector<Cowstr>::const_iterator begin,
                                std::vector<Cowstr>::const_iterator end) {
  std::stringstream ss;
  for (; begin != end; begin++) {
    ss << begin->str();
  }
  return ss.str();
}

Cowstr::Cowstr(const std::string &s)
    : value_(s.length() > 0 ? new std::string(s) : new std::string("")) {}

Cowstr::Cowstr(const char *s)
    : value_(s ? new std::string(s) : new std::string("")) {}

Cowstr::Cowstr(const char *s, int n)
    : value_((s && n > 0) ? new std::string(s, n) : new std::string("")) {}

Cowstr::Cowstr(std::vector<Cowstr>::const_iterator begin,
               std::vector<Cowstr>::const_iterator end)
    : value_(new std::string(mergeCowstrs(begin, end))) {}

const std::string &Cowstr::str() const { return *value_; }

const char *Cowstr::rawstr() const { return value_->c_str(); }

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
  sps nv(new std::string(*value_));
  nv->insert(index, count, c);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::insert(int index, const Cowstr &s) {
  sps nv(new std::string(*value_));
  nv->insert(index, s.str());
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::insert(int index, std::vector<Cowstr>::const_iterator begin,
                       std::vector<Cowstr>::const_iterator end) {
  sps nv(new std::string(*value_));
  nv->insert(index, mergeCowstrs(begin, end));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::erase(int index, int count) {
  sps nv(new std::string(*value_));
  nv->erase(index, count);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::popend(int count) {
  count = std::min<int>(count, length());
  sps nv(new std::string(*value_));
  nv->erase(length() - count - 1, count);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::append(const char &c, int count) {
  sps nv(new std::string(*value_));
  nv->append(count, c);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::append(const Cowstr &s) {
  sps nv(new std::string(*value_));
  nv->append(s.str());
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::append(std::vector<Cowstr>::const_iterator begin,
                       std::vector<Cowstr>::const_iterator end) {
  sps nv(new std::string(*value_));
  nv->append(mergeCowstrs(begin, end));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::operator+=(const Cowstr &s) {
  sps nv(new std::string(*value_));
  *nv += s.str();
  value_ = nv;
  return *this;
}

Cowstr Cowstr::operator+(const char &c) const {
  std::string r(*value_);
  r.append(1, c);
  return Cowstr(r);
}

Cowstr Cowstr::operator+(const Cowstr &s) const {
  std::string r(*value_);
  r.append(s.str());
  return Cowstr(r);
}

int Cowstr::compare(const Cowstr &s) const { return value_->compare(s.str()); }

bool Cowstr::startWith(const char &c) const { return value_->find(c) == 0; }

bool Cowstr::startWith(const Cowstr &s) const {
  return value_->find(s.str()) == 0;
}

bool Cowstr::startWithAnyOf(std::vector<Cowstr>::const_iterator begin,
                            std::vector<Cowstr>::const_iterator end) const {
  return std::any_of(begin, end, [&](const Cowstr &prefix) {
    return value_->find(prefix.str()) == 0;
  });
}

bool Cowstr::endWith(const char &c) const {
  return value_->rfind(c) == length() - 1;
}

bool Cowstr::endWith(const Cowstr &s) const {
  return value_->rfind(s.str()) == length() - 1 - s.length();
}

bool Cowstr::endWithAnyOf(std::vector<Cowstr>::const_iterator begin,
                          std::vector<Cowstr>::const_iterator end) const {
  return std::any_of(begin, end, [&](const Cowstr &prefix) {
    return value_->rfind(prefix.str()) == length() - 1 - prefix.length();
  });
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
  sps nv(new std::string(ss.str()));
  value_ = nv;
  return *this;
}

template <typename T>
std::string replaceSet(const T &fromset, const Cowstr &to,
                       const std::string &value) {
  std::stringstream ss;
  for (int i = 0; i < value.length(); i++) {
    if (fromset.find(value[i]) != fromset.end()) {
      ss << to.str();
    } else {
      ss << value[i];
    }
  }
  return ss.str();
}

Cowstr &Cowstr::replace(const std::unordered_set<char> &from, const char &to) {
  return replace(from, Cowstr(&to, 1));
}

Cowstr &Cowstr::replace(const std::unordered_set<char> &from,
                        const Cowstr &to) {
  if (from.empty()) {
    return *this;
  }
  sps nv(new std::string(replaceSet(from, to, *value_)));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(const std::set<char> &from, const char &to) {
  return replace(from, Cowstr(&to, 1));
}

Cowstr &Cowstr::replace(const std::set<char> &from, const Cowstr &to) {
  if (from.empty()) {
    return *this;
  }
  sps nv(new std::string(replaceSet(from, to, *value_)));
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
  sps nv(new std::string(replaceMap(mapping, *value_)));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(const std::map<char, Cowstr> &mapping) {
  if (mapping.empty()) {
    return *this;
  }
  sps nv(new std::string(replaceMap(mapping, *value_)));
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(int index, int count, const char &c) {
  sps nv(new std::string(*value_));
  nv->replace(index, count, 1, c);
  value_ = nv;
  return *this;
}

Cowstr &Cowstr::replace(int index, int count, const Cowstr &s) {
  sps nv(new std::string(*value_));
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
  return value_->find(c, index);
}

int Cowstr::find(const Cowstr &s, int index) const {
  return value_->find(s.str(), index);
}

int Cowstr::rfind(const char &c, int index) const {
  return value_->rfind(c, index);
}

int Cowstr::rfind(const Cowstr &s, int index) const {
  return value_->rfind(s.str(), index);
}

int Cowstr::findFirstOf(const char &c, int index) const {
  return value_->find_first_of(c, index);
}

int Cowstr::findFirstOf(const Cowstr &s, int index) const {
  return value_->find_first_of(s.str(), index);
}

int Cowstr::findLastOf(const char &c, int index) const {
  return value_->find_last_of(c, index);
}

int Cowstr::findLastOf(const Cowstr &s, int index) const {
  return value_->find_last_of(s.str(), index);
}

int Cowstr::findFirstNotOf(const char &c, int index) const {
  return value_->find_first_not_of(c, index);
}

int Cowstr::findFirstNotOf(const Cowstr &s, int index) const {
  return value_->find_first_not_of(s.str(), index);
}

int Cowstr::findLastNotOf(const char &c, int index) const {
  return value_->find_last_not_of(c, index);
}

int Cowstr::findLastNotOf(const Cowstr &s, int index) const {
  return value_->find_last_not_of(s.str(), index);
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
