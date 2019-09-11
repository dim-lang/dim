// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Row.h"
#include "Logging.h"
#include <fmt/format.h>

namespace fastype {

Row::Row() : str_(), lineNumber_(-1), dirty_(false) {}

Row::Row(const Cowstr &str, int lineNumber, int dirty)
    : str_(str), lineNumber_(lineNumber), dirty_(dirty) {
  F_CHECKF(lineNumber_ >= 0, "lineNumber_ {} >= 0", lineNumber_);
}

std::string Row::toString() const {
  return fmt::format("[ @Row str_:{} lineNumber_:{} dirty_:{} ]",
                     str_.toString(), lineNumber_, dirty_);
}

Cowstr &Row::str() { return str_; }

const Cowstr &Row::str() const { return str_; }

const int &Row::lineNumber() const { return lineNumber_; }

int &Row::lineNumber() { return lineNumber_; }

const bool &Row::dirty() const { return dirty_; }

bool &Row::dirty() { return dirty_; }

} // namespace fastype
