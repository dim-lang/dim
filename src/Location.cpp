// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Location.h"
#include "fmt/format.h"

// Position {

Position::Position(int a_line, int a_column) : line(a_line), column(a_column) {}

Cowstr Position::str() const {
  return Cowstr(fmt::format("{}.{}", line, column));
}

bool Position::operator==(const Position &pos) const {
  return (&pos == this) || (line == pos.line && column == pos.column);
}

bool Position::operator!=(const Position &pos) const { return !(pos == *this); }

bool Position::operator>(const Position &pos) const {
  return line == pos.line ? column > pos.column : line > pos.line;
}

bool Position::operator>=(const Position &pos) const {
  return *this > pos || *this == pos;
}

bool Position::operator<(const Position &pos) const {
  return line == pos.line ? column < pos.column : line < pos.line;
}

bool Position::operator<=(const Position &pos) const {
  return *this < pos || *this == pos;
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

bool Location::operator==(const Location &loc) const {
  return (&loc == this) || (begin == loc.begin && end == loc.end);
}

bool Location::operator!=(const Location &loc) const { return !(loc == *this); }

bool Location::operator>(const Location &loc) const {
  return end == loc.end ? begin > loc.begin : end > loc.end;
}

bool Location::operator>=(const Location &loc) const {
  return *this > loc || *this == loc;
}

bool Location::operator<(const Location &loc) const {
  return end == loc.end ? begin < loc.begin : end < loc.end;
}

bool Location::operator<=(const Location &loc) const {
  return *this < loc || *this == loc;
}

// Location }

LocationableImpl::LocationableImpl(const Location &location)
    : locationableImpl_(location) {}

Location &LocationableImpl::location() { return locationableImpl_; }

const Location &LocationableImpl::location() const { return locationableImpl_; }
