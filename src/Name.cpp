// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"
#include "Ast.h"
#include "fmt/format.h"
#include <functional>
#include <sstream>

Nameable::Nameable() : name_("") {}

Nameable::Nameable(const Cowstr &name) : name_(name) {}

Cowstr &Nameable::name() { return name_; }

const Cowstr &Nameable::name() const { return name_; }

Cowstr AstGraphNameGenerator::from(const Cowstr &s) {
  std::stringstream ss;
  ss << s << counter_.next();
  return ss.str();
}

Cowstr SymbolNameGenerator::from(const Ast *ast) {
  std::stringstream ss;
  ss << ast->name() << "." << ast->location().str();
  return ss.str();
}

// Cowstr IrNameGenerator::from(const Cowstr &hint) {
//   std::stringstream ss;
//   ss << "nerd.ir." << hint << "." << counter_.next();
//   return ss.str();
// }
