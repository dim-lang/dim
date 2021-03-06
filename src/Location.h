// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include "infra/Cowstr.h"
#include "parser.tab.hh"
#include <iostream>

class Position {
public:
  Position(int a_line = 0, int a_column = 0);
  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  virtual ~Position() = default;

  int line;
  int column;

  // to string
  Cowstr str() const;

  // comparison
  bool operator==(const Position &pos) const;
  bool operator!=(const Position &pos) const;
  bool operator>(const Position &pos) const;
  bool operator>=(const Position &pos) const;
  bool operator<(const Position &pos) const;
  bool operator<=(const Position &pos) const;
};

class Location {
public:
  Location(const Position &a_begin = Position(),
           const Position &a_end = Position());
  Location(YYLTYPE yylloc);
  Location(const Location &) = default;
  Location &operator=(const Location &) = default;
  virtual ~Location() = default;

  Position begin;
  Position end;

  // to string
  Cowstr str() const;

  // comparison
  bool operator==(const Location &loc) const;
  bool operator!=(const Location &loc) const;
  bool operator>(const Location &loc) const;
  bool operator>=(const Location &loc) const;
  bool operator<(const Location &loc) const;
  bool operator<=(const Location &loc) const;
};

std::ostream &operator<<(std::ostream &os, const Location &loc);

namespace fmt {

template <> struct formatter<Location> : formatter<std::string> {
  template <typename FormatContext>
  auto format(Location s, FormatContext &ctx) {
    return formatter<std::string>::format(s.str().str(), ctx);
  }
};

} // namespace fmt
