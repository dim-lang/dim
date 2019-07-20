// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Util.h"
#include "exceptions/ParameterException.h"
#include "fmt/format.h"

namespace fastype {

Line::Line() : Logging("Line"), data(), lineNumber(-1), dirty(false) {}

Line::Line(const icu::UnicodeString &data, int lineNumber, int dirty)
    : Logging("Line"), data_(data), lineNumber_(lineNumber), dirty_(dirty) {
  if (lineNumber_ < 0) {
    F_THROW_EXF(ParameterException, "lineNumber_:{}", lineNumber_);
  }
}

Line::~Line() {
  lineNumber_ = -1;
  dirty_ = false;
}

std::string Line::toString() const {
  return fmt::format("[ @Line data_#length:{} lineNumber_:{} dirty_:{}",
                     data_.length(), lineNumber_, dirty_);
}

icu::UnicodeString &Line::data() { return data_; }

void Line::setData(const icu::UnicodeString &data) { data_ = data; }

int &Line::lineNumber() { return lineNumber_; }

void Line::setLineNumber(int lineNumber) { lineNumber_ = lineNumber; }

bool &Line::dirty() { return dirty_; }

void Line::setDirty(int dirty) { dirty_ = dirty; }

} // namespace fastype
