// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "parser.tab.hh"
#include <string>

class Position {
public:
  Position(int a_line = 1, int a_column = 1);
  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  virtual ~Position() = default;

  int line;
  int column;
  virtual Cowstr str() const;
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
  virtual Cowstr str() const;
};

class Locationable {
public:
  Locationable(const Location &location = Location());
  virtual ~Locationable() = default;
  virtual Location &location();
  virtual const Location &location() const;

private:
  Location location_;
};
