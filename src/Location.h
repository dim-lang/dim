// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "container/Cowstr.h"
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

  // to string
  virtual Cowstr str() const;

  // comparison
  virtual bool operator==(const Position &pos) const;
  virtual bool operator!=(const Position &pos) const;
  virtual bool operator>(const Position &pos) const;
  virtual bool operator>=(const Position &pos) const;
  virtual bool operator<(const Position &pos) const;
  virtual bool operator<=(const Position &pos) const;
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
  virtual Cowstr str() const;

  // comparison
  virtual bool operator==(const Location &loc) const;
  virtual bool operator!=(const Location &loc) const;
  virtual bool operator>(const Location &loc) const;
  virtual bool operator>=(const Location &loc) const;
  virtual bool operator<(const Location &loc) const;
  virtual bool operator<=(const Location &loc) const;
};

class Locationable {
public:
  virtual ~Locationable() = default;
  virtual Location &location() = 0;
  virtual const Location &location() const = 0;
};

class LocationableImpl : public Locationable {
public:
  LocationableImpl(const Location &location = Location());
  virtual ~LocationableImpl() = default;
  virtual Location &location();
  virtual const Location &location() const;

protected:
  Location locationableImpl_;
};
