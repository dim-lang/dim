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

Name::Name(char *name) : name_(name), id_(Counter::get()) {}

int Name::compare(const Name &other) const { return name_ - other.name_; }

bool Name::operator==(const Name &other) const { return name_ == other.name_; }

bool Name::operator!=(const Name &other) const { return name_ != other.name_; }

bool Name::operator>(const Name &other) const { return name_ > other.name_; }

bool Name::operator>=(const Name &other) const { return name_ >= other.name_; }

bool Name::operator<(const Name &other) const { return name_ < other.name_; }

bool Name::operator<=(const Name &other) const { return name_ <= other.name_; }

const char *Name::raw() const { return name_; }

unsigned long long Name::id() const { return id_; }

std::string Name::toString() const { return fmt::format("{}.{}", raw(), id()); }

const char *Name::llvmName() const {
  std::stringstream ss;
  ss << "$"
     << fmt::format("{0:x}", std::hash<std::string>()(std::string(name_)))
     << "." << id();
  return Strings::dup(ss.str().c_str());
}

const char *Name::symbolName() const {
  std::stringstream ss;
  ss << "@" << toString();
  return Strings::dup(ss.str().c_str());
}

Name Name::get(const std::string &name) { return Name(Strings::dup(name)); }

Name Name::get(const char *name) { return Name(Strings::dup(name)); }

Nameable::Nameable() : name_(Name::get("")) {}

Nameable::Nameable(const std::string &name) : name_(Name::get(name)) {}

Nameable::Nameable(const char *name) : name_(Name::get(name)) {}

Name &Nameable::name() { return name_; }

const Name &Nameable::name() const { return name_; }
