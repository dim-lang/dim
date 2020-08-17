// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"
#include "fmt/format.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <utility>
#include <vector>

#define USD std::string("$")
#define AT std::string("@")
#define DOT std::string(".")
#define HEX(s) fmt::format("{0:x}", std::hash<std::string>()(s))

Counter Name::counter_(1ULL);

Name Name::noName_("");

std::unordered_map<std::string, Name> Name::nameMap_ = {
    {"", Name::noName_},
};

Name::Name(const std::string &value)
    : value_(new std::string(value)), id_(counter_.next()) {}

int Name::compare(const Name &name) const {
  return value_->compare(*name.value_);
}

bool Name::operator==(const Name &name) const {
  return value_.get() == name.value_.get();
}

bool Name::operator!=(const Name &name) const {
  return value_.get() != name.value_.get();
}

bool Name::operator>(const Name &name) const { return *value_ > *name.value_; }

bool Name::operator>=(const Name &name) const {
  return *value_ >= *name.value_;
}

bool Name::operator<(const Name &name) const { return *value_ < *name.value_; }

bool Name::operator<=(const Name &name) const {
  return *value_ <= *name.value_;
}

std::string Name::raw() const { return *value_; }

unsigned long long Name::id() const { return id_; }

std::string Name::toLLVMName() const {
  return USD + HEX(*value_) + DOT + std::to_string(id());
}

static bool containsChar(char c, const std::vector<char> &vec) {
  return std::find(vec.begin(), vec.end(), c) != vec.end();
}

static std::string parseSymbolName(const std::string &s) {
  std::string r(s);
  std::for_each(r.begin(), r.end(), [](char &c) {
    if (!std::isalnum(c) &&
        !containsChar(c, std::vector<char>{'.', '$', '_', '-'})) {
      c = '-';
    }
  });
  return r.length() > 10 ? r.substr(0, 10) : r;
}

std::string Name::toSymbolName() const {
  return AT + parseSymbolName(*value_) + DOT + std::to_string(id());
}

Name Name::get(const std::string &name) {
  if (nameMap_.find(name) == nameMap_.end()) {
    nameMap_.insert(std::make_pair(name, Name(name)));
  }
  return nameMap_[name];
}

const Name &Name::noName() { return noName_; }
