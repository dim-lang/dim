// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"
#include <sstream>

Nameable::Nameable() : name_("") {}

Nameable::Nameable(const Cowstr &name) : name_(name) {}

Cowstr &Nameable::name() { return name_; }

const Cowstr &Nameable::name() const { return name_; }

namespace detail {

Cowstr NameGenerator::from(const Cowstr &hint) {
  std::stringstream ss;
  ss << hint << "." << counter_.next();
  return ss.str();
}

} // namespace detail

Cowstr AstGraphNameGenerator::from(const Cowstr &hint) {
  return detail::NameGenerator::from(hint);
}

Cowstr SymbolNameGenerator::from(const Cowstr &hint) {
  std::stringstream ss;
  ss << "nerd.sym." << hint << "." << counter_.next();
  return ss.str();
}

Cowstr IrNameGenerator::from(const Cowstr &hint) {
  std::stringstream ss;
  ss << "nerd.ir." << hint << "." << counter_.next();
  return ss.str();
}
