// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Location.h"
#include "fmt/format.h"

// Position {

Position::Position(int a_line, int a_column) : line(a_line), column(a_column) {}

Cowstr Position::str() const {
  return Cowstr(fmt::format("{}.{}", line, column));
}

// Position }

// Location {

Location::Location(const Position &a_begin, const Position &a_end)
    : begin(a_begin), end(a_end) {}

Location::Location(YYLTYPE yylloc)
    : begin(yylloc.first_line, yylloc.first_column),
      end(yylloc.last_line, yylloc.last_column) {}

Cowstr Location::str() const {
  return Cowstr(fmt::format("{}-{}", begin.str().str(), end.str().str()));
}

// Location }

Locationable::Locationable(const Location &location) : location_(location) {}

Location &Locationable::location() { return location_; }

const Location &Locationable::location() const { return location_; }
