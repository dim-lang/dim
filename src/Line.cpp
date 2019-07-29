// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Util.h"
#include "fmt/format.h"

namespace fastype {

Line::Line() : Logging("Line"), data_(), lineNumber_(-1), dirty_(false) {
  F_DEBUGF("No Args Constructor:{}", toString());
}

Line::Line(const char *src, int start, int length, int lineNumber, int dirty)
    : Line() {
  data_.expand(length);
  lineNumber_ = lineNumber;
  dirty_ = dirty;
  std::memcpy(data_.data(), src + start, length * sizeof(char));
  F_CHECKF(lineNumber_ >= 0, "lineNumber_:{}", lineNumber_);
  F_CHECKF(length > 0, "length:{}", length);
  F_CHECKF(data_.size() >= length, "data_#size:{} >= length:{}", data_.size(),
           length);
  F_DEBUGF("Args Constructor:{} data_#size:{} data_#capacity:{}", toString(),
           data_.size(), data_.capacity());
}

std::string Line::toString() const {
  return fmt::format("[ @Line data_#data:{} data_#pointer:{} data_#size:{} "
                     "lineNumber_:{} dirty_:{} ]",
                     data_.data(), (void *)data_.data(), data_.size(),
                     lineNumber_, dirty_);
}

char *Line::data() { return data_.data(); }

char &Line::operator[](int index) {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < data_.size(), "index {} < data_#size {}", index,
           data_.size());
  return data_[index];
}

int Line::size() const { return data_.size(); }

void Line::setSize(int size) { data_.setSize(size); }

int Line::capacity() const { return data_.capacity(); }

void Line::setCapacity(int capacity) { data_.expand(capacity); }

int &Line::lineNumber() { return lineNumber_; }

void Line::setLineNumber(int lineNumber) { lineNumber_ = lineNumber; }

bool &Line::dirty() { return dirty_; }

void Line::setDirty(int dirty) { dirty_ = dirty; }

void Line::expand(int n) { return data_.expand(n); }

} // namespace fastype
