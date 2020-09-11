// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include <algorithm>
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

  virtual const std::string &str() const;
  virtual const char *rawstr() const;

  virtual int length() const;
  virtual bool empty() const;

  virtual const char &operator[](int index) const;

  // return *this
  virtual Cowstr &insert(int index, const char &c, int count = 1);
  virtual Cowstr &insert(int index, const Cowstr &s);
  virtual Cowstr &insert(int index, std::vector<Cowstr>::const_iterator begin,
                         std::vector<Cowstr>::const_iterator end);

  virtual Cowstr &erase(int index, int count = 1);
  virtual Cowstr &popend(int count = 1); // equal to erase(length()-1)

  virtual Cowstr &append(const char &c, int count = 1);
  virtual Cowstr &append(const Cowstr &s);
  virtual Cowstr &append(std::vector<Cowstr>::const_iterator begin,
                         std::vector<Cowstr>::const_iterator end);

  virtual Cowstr operator+(const char &c) const;
  virtual Cowstr operator+(const Cowstr &s) const;

  virtual int compare(const Cowstr &s) const;

  virtual bool startWith(const char &c) const;
  virtual bool startWith(const Cowstr &s) const;
  virtual bool startWithAnyOf(std::vector<Cowstr>::const_iterator begin,
                              std::vector<Cowstr>::const_iterator end) const;

  virtual bool endWith(const char &c) const;
  virtual bool endWith(const Cowstr &s) const;
  virtual bool endWithAnyOf(std::vector<Cowstr>::const_iterator begin,
                            std::vector<Cowstr>::const_iterator end) const;

  virtual Cowstr &replace(const char &from, const char &to);
  virtual Cowstr &replace(const Cowstr &from, const Cowstr &to);
  virtual Cowstr &replace(const std::unordered_set<char> &from, const char &to);
  virtual Cowstr &replace(const std::unordered_set<char> &from,
                          const Cowstr &to);
  virtual Cowstr &replace(const std::set<char> &from, const char &to);
  virtual Cowstr &replace(const std::set<char> &from, const Cowstr &to);
  virtual Cowstr &replace(const std::unordered_map<char, Cowstr> &mapping);
  virtual Cowstr &replace(const std::map<char, Cowstr> &mapping);

  virtual Cowstr &replace(int fromindex, int toindex, const char &c);
  virtual Cowstr &replace(int fromindex, int toindex, const Cowstr &s);

  virtual Cowstr &subString(int index);
  virtual Cowstr &subString(int index, int count);

  virtual int find(const char &c, int index = 0) const;
  virtual int find(const Cowstr &s, int index = 0) const;
  virtual int rfind(const char &c, int index = 0) const;
  virtual int rfind(const Cowstr &s, int index = 0) const;

  virtual int findFirstOf(const char &c, int index = 0) const;
  virtual int findFirstOf(const Cowstr &s, int index = 0) const;
  virtual int findLastOf(const char &c, int index = 0) const;
  virtual int findLastOf(const Cowstr &s, int index = 0) const;

  virtual int findFirstNotOf(const char &c, int index = 0) const;
  virtual int findFirstNotOf(const Cowstr &s, int index = 0) const;
  virtual int findLastNotOf(const char &c, int index = 0) const;
  virtual int findLastNotOf(const Cowstr &s, int index = 0) const;

  virtual bool operator==(const Cowstr &other) const;
  virtual bool operator!=(const Cowstr &other) const;
  virtual bool operator>(const Cowstr &other) const;
  virtual bool operator>=(const Cowstr &other) const;
  virtual bool operator<(const Cowstr &other) const;
  virtual bool operator<=(const Cowstr &other) const;

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
