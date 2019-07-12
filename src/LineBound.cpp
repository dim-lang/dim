// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "LineBound.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

LineBound::LineBound() : buffer_(-1), byte_(-1) {}

LineBound::LineBound(int32_t buffer, int32_t byte)
    : buffer_(buffer), byte_(byte) {}

int32_t LineBound::buffer() const { return buffer_; }

int32_t LineBound::setBuffer(int32_t buffer) {
  return std::exchange(buffer_, buffer);
}

int32_t LineBound::byte() const { return byte_; }

int32_t LineBound::setByte(int32_t byte) { return std::exchange(byte_, byte); }

bool LineBound::undefined() const { return buffer_ < 0 && byte_ < 0; }

LineBound LineBound::reset() {
  LineBound old(*this);
  buffer_ = -1;
  byte_ = -1;
  return old;
}

std::string LineBound::toString() const {
  return fmt::format("[ @LineBound buffer: {}, byte: {}]", buffer_, byte_);
}

} // namespace fastype
