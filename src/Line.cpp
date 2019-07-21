// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Util.h"
#include "exceptions/ParameterException.h"
#include "fmt/format.h"

namespace fastype {

Line::Line() : Logging("Line"), lineNumber_(-1), dirty_(false) {
  F_DEBUGF("constructor:{}", toString());
}

Line::Line(const UChar *src, int start, int length, int lineNumber, int dirty)
    : Logging("Line"), lineNumber_(lineNumber), dirty_(dirty) {
  data_.resize((int64_t)length);
  std::memcpy(data_.data(), src + start, length * sizeof(UChar));
  F_CHECKF(lineNumber_ >= 0, "lineNumber_:{}", lineNumber_);
  F_CHECKF(length > 0, "length:{}", length);
  F_CHECKF(data.size() >= length, "data#size:{} >= length:{}", data.size(),
           length);
  F_DEBUGF("constructor:{} data_#size:{} data_#capacity:{}", toString(),
           data_.size(), data_.capacity());
}

std::string Line::toString() const {
  return fmt::format(
      "[ @Line data_#size:{} data_#data:{} lineNumber_:{} dirty_:{} ]",
      data_.size(), (void *)data_.data(), lineNumber_, dirty_);
}

UChar *Line::data() { return data_.data(); }

int Line::size() const { return data_.size(); }

void Line::setSize(int size) { data_.resize(size); }

int Line::capacity() const { return data_.capacity(); }

void Line::setCapacity(int capacity) { data_.reserve(capacity); }

int &Line::lineNumber() { return lineNumber_; }

void Line::setLineNumber(int lineNumber) { lineNumber_ = lineNumber; }

bool &Line::dirty() { return dirty_; }

void Line::setDirty(int dirty) { dirty_ = dirty; }

int Line::expand(int n) {
  data_.reserve(n);
  return data_.capacity();
}

} // namespace fastype
