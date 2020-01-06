// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Logging.h"
#include <fmt/format.h>

Line::Line() : value_(), lineNumber_(-1), dirty_(false) {}

Line::Line(const icu::UnicodeString &value, int lineNumber, int dirty)
    : value_(value), lineNumber_(lineNumber), dirty_(dirty) {
  FCHECK(lineNumber_ >= 0, "lineNumber_ {} >= 0", lineNumber_);
}

std::string Line::toString() const {
  std::string _1;
  return fmt::format("[ @Line value_:{} lineNumber_:{} dirty_:{} ]",
                     value_.toUTF8String(_1), lineNumber_, dirty_);
}

icu::UnicodeString &Line::value() { return value_; }

const icu::UnicodeString &Line::value() const { return value_; }

const int &Line::lineNumber() const { return lineNumber_; }

int &Line::lineNumber() { return lineNumber_; }

const bool &Line::dirty() const { return dirty_; }

bool &Line::dirty() { return dirty_; }
