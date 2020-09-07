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

static std::unordered_map<std::string, Name> NameMap = {
    {"", Name::noName()},
};

Name::Name(const std::string &value)
    : value_(new std::string(value)), id_(Counter::get()) {}

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

std::string Name::toString() const {
  return fmt::format("[@Name id:{} value:{}]", id(), raw());
}

Name Name::get(const std::string &name) {
  if (NameMap.find(name) == NameMap.end()) {
    NameMap.insert(std::make_pair(name, Name(name)));
  }
  return NameMap.find(name)->second;
}

const Name &Name::noName() {
  static Name noName("");
  return noName;
}

Nameable::Nameable(const std::string &name) : name_(Name::get(name)) {}

Name &Nameable::name() { return name_; }

const Name &Nameable::name() const { return name_; }
