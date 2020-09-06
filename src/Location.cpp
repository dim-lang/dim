// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Location.h"
#include "fmt/format.h"

// Position {

Position::Position(int a_line, int a_column) : line(a_line), column(a_column) {}

std::string Position::toString() const {
  return fmt::format("{}.{}", line, column);
}

// Position }

// Location {

Location::Location(const Position &a_begin, const Position &a_end)
    : begin(a_begin), end(a_end) {}

Location::Location(YYLTYPE yylloc)
    : begin(yylloc.first_line, yylloc.first_column),
      end(yylloc.last_line, yylloc.last_column) {}

std::string Location::toString() const {
  return fmt::format("{}-{}", begin.toString(), end.toString());
}

// Location }

Locationable::Locationable(const Location &location) : location_(location) {}

Location &Locationable::location() { return location_; }

const Location &Locationable::location() const { return location_; }
