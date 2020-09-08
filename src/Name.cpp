// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"
#include "Strings.h"
#include "fmt/format.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>

#define SYMBOL_LENGTH 16

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
  return r.length() > SYMBOL_LENGTH ? r.substr(0, SYMBOL_LENGTH) : r;
}

Name::Name(std::shared_ptr<std::string> name)
    : name_(name), symName_(nullptr), llvmName_(nullptr), id_(Counter::get()) {
  std::stringstream ss;
  ss << "@" << parseSymbolName(*name_) << "." << id();
  symName_ = std::make_shared<std::string>(Strings::dup(ss.str().c_str()));

  ss.clear();
  ss << "$" << fmt::format("{0:x}", std::hash<std::string>()(*name_)) << "."
     << id();
  llvmName_ = std::make_shared<std::string>(Strings::dup(ss.str().c_str()));
}

int Name::compare(const Name &other) const {
  return (char *)name_.get() - (char *)other.name_.get();
}

bool Name::operator==(const Name &other) const {
  return name_.get() == other.name_.get();
}

bool Name::operator!=(const Name &other) const {
  return name_.get() != other.name_.get();
}

bool Name::operator>(const Name &other) const {
  return name_.get() > other.name_.get();
}

bool Name::operator>=(const Name &other) const {
  return name_.get() >= other.name_.get();
}

bool Name::operator<(const Name &other) const {
  return name_.get() < other.name_.get();
}

bool Name::operator<=(const Name &other) const {
  return name_.get() <= other.name_.get();
}

const std::string &Name::raw() const { return *name_; }

unsigned long long Name::id() const { return id_; }

const std::string &Name::toLLVMName() const { return *llvmName_; }

const std::string &Name::toSymbolName() const { return *symName_; }

Name Name::get(const std::string &name) {
  std::shared_ptr<std::string> value =
      std::make_shared<std::string>(Strings::dup(name.c_str()));
  return Name(value);
}

Nameable::Nameable(const std::string &name) : name_(Name::get(name)) {}

Name &Nameable::name() { return name_; }

const Name &Nameable::name() const { return name_; }
