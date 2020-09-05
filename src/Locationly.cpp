// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Locationly.h"
#include <sstream>

Locationly::Locationly(const yy::location &location) : location_(location) {}

yy::location &Locationly::location() { return location_; }

const yy::location &Locationly::location() const { return location_; }

std::string Locationly::locationString(const yy::location &location) {
  std::stringstream ss;
  ss << location;
  return ss.str();
}

std::string Locationly::positionString(const yy::position &position) {
  std::stringstream ss;
  ss << position;
  return ss.str();
}
