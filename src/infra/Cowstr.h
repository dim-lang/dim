// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cstdlib>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

class Cowstr {
public:
  Cowstr();
  Cowstr(const std::string &s);
  Cowstr(const char *s);
  Cowstr(const char *s, int n);

  virtual ~Cowstr() = default;
  Cowstr(const Cowstr &) = default;
  Cowstr &operator=(const Cowstr &) = default;

  template <typename T> static Cowstr from(T value) {
    return fmt::format("{}", value);
  }
  static Cowstr from(llvm::Value *llvmValue) {
    std::string s;
    llvm::raw_string_ostream sos(s);
    sos << *llvmValue;
    return s;
  }
  static Cowstr from(llvm::Type *llvmType) {
    std::string s;
    llvm::raw_string_ostream sos(s);
    sos << *llvmType;
    return s;
  }
  static Cowstr from(llvm::Constant *llvmConstant) {
    std::string s;
    llvm::raw_string_ostream sos(s);
    sos << *llvmConstant;
    return s;
  }
  static Cowstr from(llvm::Function *llvmFunction) {
    std::string s;
    llvm::raw_string_ostream sos(s);
    sos << *llvmFunction;
    return s;
  }
  static Cowstr from(llvm::Module *llvmModule) {
    std::string s;
    llvm::raw_string_ostream sos(s);
    sos << *llvmModule;
    return s;
  }

  // Iterator is iterator/const_iterator of STL containers with typename Cowstr,
  // such as:
  // - std::vector<Cowstr>
  // - std::list<Cowstr>
  // - std::set<Cowstr>
  // - etc
  // But except mapping STL data structures like std::map, std::unordered_map,
  // etc.
  template <typename Iterator>
  static Cowstr join(Iterator begin, Iterator end,
                     const Cowstr &delimiter = "") {
    if (begin == end) {
      return "";
    }
    std::stringstream ss;
    for (; begin != end; ++begin) {
      ss << begin->str();
      if (begin != end - 1 && !delimiter.empty()) {
        ss << delimiter.str();
      }
    }
    return ss.str();
  }

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

  // modify and return self
  Cowstr &insert(int index, const char &c, int count = 1);
  Cowstr &insert(int index, const Cowstr &s);

  template <typename Iterator>
  Cowstr &insert(int index, Iterator begin, Iterator end) {
    if (begin == end) {
      return *this;
    }
    std::shared_ptr<std::string> nv = std::shared_ptr<std::string>(
        (value_ && !value_->empty()) ? new std::string(*value_)
                                     : new std::string());
    nv->insert(index, Cowstr::join(begin, end).str());
    value_ = nv;
    return *this;
  }

  Cowstr &erase(int index, int count = 1);
  // pop out the last char, equal to erase(length() - 1)
  Cowstr &popend(int count = 1);

  Cowstr &append(const char &c, int count = 1);
  Cowstr &append(const Cowstr &s);

  template <typename Iterator> Cowstr &append(Iterator begin, Iterator end) {
    if (begin == end) {
      return *this;
    }
    std::shared_ptr<std::string> nv = std::shared_ptr<std::string>(
        (value_ && !value_->empty()) ? new std::string(*value_)
                                     : new std::string());
    nv->append(Cowstr::join(begin, end));
    value_ = nv;
    return *this;
  }

  Cowstr &operator+=(const Cowstr &s);

  Cowstr operator+(const char &c) const;
  Cowstr operator+(const Cowstr &s) const;

  int compare(const Cowstr &s) const;

  bool startWith(const char &c) const;
  bool startWith(const Cowstr &s) const;

  template <typename Iterator>
  bool startWithAnyOf(Iterator begin, Iterator end) const {
    if (begin == end) {
      return false;
    }
    return std::any_of(begin, end, [&](const Cowstr &prefix) {
      return value_->find(prefix.str()) == 0;
    });
  }

  bool endWith(const char &c) const;
  bool endWith(const Cowstr &s) const;

  template <typename Iterator>
  bool endWithAnyOf(Iterator begin, Iterator end) const {
    if (begin == end) {
      return false;
    }
    return std::any_of(begin, end, [&](const Cowstr &suffix) {
      size_t pos = value_->rfind(suffix.str());
      int target = length() - suffix.length();
      return pos != std::string::npos && target >= 0 && pos == (size_t)target;
    });
  }

  Cowstr &replace(const char &from, const char &to);
  Cowstr &replace(const Cowstr &from, const Cowstr &to);

  template <typename Iterator>
  Cowstr &replace(Iterator frombegin, Iterator fromend, const Cowstr &to) {
    if (frombegin == fromend) {
      return *this;
    }
    std::stringstream ss;
    for (int i = 0; i < (int)value_->length(); ++i) {
      bool found = false;
      for (auto j = frombegin; j != fromend; ++j) {
        if ((*j) == value_->at(i)) {
          found = true;
          break;
        }
      }
      if (found) {
        ss << to.str();
      } else {
        ss << value_->at(i);
      }
    }
    std::shared_ptr<std::string> nv = std::shared_ptr<std::string>(
        ss.str().empty() ? new std::string() : new std::string(ss.str()));
    value_ = nv;
    return *this;
  }

  // Iterator is iterator/const_iterator of STL containers with typename char,
  // such as:
  // - std::vector<char>
  // - std::list<char>
  // - std::set<char>
  // - etc
  // But except mapping STL data structures like std::map, std::unordered_map,
  // etc.
  template <typename Iterator>
  Cowstr &replace(Iterator frombegin, Iterator fromend, const char &to) {
    return replace(frombegin, fromend, Cowstr(&to, 1));
  }

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

  Cowstr repeat(int n) const;

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
