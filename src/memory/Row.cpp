// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "memory/Row.h"
#include "Logging.h"
#include "Profile.h"
#include <fmt/format.h>

namespace fastype {

Row::Row() : data_(new Buffer()), lineNumber_(-1), dirty_(false) {
  F_DEBUGF("No Args Constructor:{}", toString());
}

Row::Row(int capacity, int lineNumber, int dirty)
    : data_(new Buffer(capacity)), lineNumber_(lineNumber), dirty_(dirty) {
  F_CHECKF(lineNumber_ >= 0, "lineNumber_:{} >= 0", lineNumber_);
  F_DEBUGF("lineNumber/dirty Args Constructor:{}", toString());
}

std::string Row::toString() const {
  return fmt::format(
      "[ @Row data_#size:{} lineNumber_:{} dirty_:{} data_#data:{} ]",
      data_->size(), lineNumber_, dirty_, data_->data());
}

char *Row::data() { return data_->data(); }

const char *Row::data() const { return data_->data(); }

char &Row::operator[](int index) {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < data_->capacity(), "index {} < data_#capacity_ {}", index,
           data_->capacity());
  return (*data_)[index];
}
const char &Row::operator[](int index) const {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < data_->capacity(), "index {} < data_#capacity_ {}", index,
           data_->capacity());
  return (*data_)[index];
}

int Row::size() const { return data_->size(); }

void Row::setSize(int size) { data_->setSize(size); }

int Row::capacity() const { return data_->capacity(); }

void Row::setCapacity(int capacity) { data_->expand(capacity); }

int &Row::lineNumber() { return lineNumber_; }

void Row::setRowNumber(int lineNumber) { lineNumber_ = lineNumber; }

bool &Row::dirty() { return dirty_; }

void Row::setDirty(int dirty) { dirty_ = dirty; }

void Row::expand(int n) { data_->expand(n); }

} // namespace fastype
