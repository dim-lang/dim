// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <memory>

namespace fastype {

class File;

class Line {
public:
  Line();
  Line(int32_t index);
  Line(const Line &) = default;
  Line &operator=(const Line &) = default;
  virtual ~Line();

private:
  int32_t index;
  std::list<std::shared_ptr<Buffer>> bufferList;

  friend class File;
};

// equal
inline bool operator==(const Line &a, const Line &b) const {
  return a.index == b.index;
}

// not equal
inline bool operator!=(const Line &a, const Line &b) const { return !(a == b); }

// greater
inline bool operator>(const Line &a, const Line &b) const {
  return a.index > b.index;
}

// greater equal
inline bool operator>=(const Line &a, const Line &b) const {
  return a > b || a == b;
}

// less
inline bool operator<(const Line &a, const Line &b) const {
  return a.index < b.index;
}

// less equal
inline bool operator<=(const Line &a, const Line &b) const {
  return a < b || a == b;
}

} // namespace fastype
