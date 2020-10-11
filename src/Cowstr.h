// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Cowstr {
public:
  Cowstr();
  Cowstr(const std::string &s);
  Cowstr(const char *s);
  Cowstr(const char *s, int n);
  Cowstr(std::vector<Cowstr>::const_iterator begin,
         std::vector<Cowstr>::const_iterator end);
  virtual ~Cowstr() = default;
  Cowstr(const Cowstr &) = default;
  Cowstr &operator=(const Cowstr &) = default;

  const std::string &str() const;
  const char *rawstr() const;

  int length() const;
  bool empty() const;

  const char &operator[](int index) const;
  const char &front() const;
  const char &back() const;
  const char *begin() const;
  const char *end() const;
  const char *rbegin() const;
  const char *rend() const;

  // return *this
  Cowstr &insert(int index, const char &c, int count = 1);
  Cowstr &insert(int index, const Cowstr &s);
  Cowstr &insert(int index, std::vector<Cowstr>::const_iterator begin,
                 std::vector<Cowstr>::const_iterator end);

  Cowstr &erase(int index, int count = 1);
  Cowstr &popend(int count = 1); // equal to erase(length()-1)

  Cowstr &append(const char &c, int count = 1);
  Cowstr &append(const Cowstr &s);
  Cowstr &append(std::vector<Cowstr>::const_iterator begin,
                 std::vector<Cowstr>::const_iterator end);
  Cowstr &operator+=(const Cowstr &s);

  Cowstr operator+(const char &c) const;
  Cowstr operator+(const Cowstr &s) const;

  int compare(const Cowstr &s) const;

  bool startWith(const char &c) const;
  bool startWith(const Cowstr &s) const;
  bool startWithAnyOf(std::vector<Cowstr>::const_iterator begin,
                      std::vector<Cowstr>::const_iterator end) const;

  bool endWith(const char &c) const;
  bool endWith(const Cowstr &s) const;
  bool endWithAnyOf(std::vector<Cowstr>::const_iterator begin,
                    std::vector<Cowstr>::const_iterator end) const;

  Cowstr &replace(const char &from, const char &to);
  Cowstr &replace(const Cowstr &from, const Cowstr &to);
  Cowstr &replace(const std::unordered_set<char> &from, const char &to);
  Cowstr &replace(const std::unordered_set<char> &from, const Cowstr &to);
  Cowstr &replace(const std::set<char> &from, const char &to);
  Cowstr &replace(const std::set<char> &from, const Cowstr &to);
  Cowstr &replace(const std::unordered_map<char, Cowstr> &mapping);
  Cowstr &replace(const std::map<char, Cowstr> &mapping);

  Cowstr &replace(int index, int count, const char &c);
  Cowstr &replace(int index, int count, const Cowstr &s);

  Cowstr subString(int index) const;
  Cowstr subString(int index, int count) const;

  int find(const char &c, int index = 0) const;
  int find(const Cowstr &s, int index = 0) const;
  int rfind(const char &c, int index = 0) const;
  int rfind(const Cowstr &s, int index = 0) const;

  int findFirstOf(const char &c, int index = 0) const;
  int findFirstOf(const Cowstr &s, int index = 0) const;
  int findLastOf(const char &c, int index = 0) const;
  int findLastOf(const Cowstr &s, int index = 0) const;

  int findFirstNotOf(const char &c, int index = 0) const;
  int findFirstNotOf(const Cowstr &s, int index = 0) const;
  int findLastNotOf(const char &c, int index = 0) const;
  int findLastNotOf(const Cowstr &s, int index = 0) const;

  bool operator==(const Cowstr &other) const;
  bool operator!=(const Cowstr &other) const;
  bool operator>(const Cowstr &other) const;
  bool operator>=(const Cowstr &other) const;
  bool operator<(const Cowstr &other) const;
  bool operator<=(const Cowstr &other) const;

private:
  std::shared_ptr<std::string> value_;
};

namespace std {

template <> struct hash<Cowstr> {
  std::size_t operator()(const Cowstr &s) const {
    return std::hash<std::string>()(s.str());
  }
};

} // namespace std

std::ostream &operator<<(std::ostream &os, const Cowstr &s);

namespace fmt {

template <> struct formatter<Cowstr> : formatter<std::string> {
  template <typename FormatContext> auto format(Cowstr s, FormatContext &ctx) {
    return formatter<std::string>::format(s.str(), ctx);
  }
};

} // namespace fmt
