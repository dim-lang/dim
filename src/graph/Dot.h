// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace fastype {

// Dot can be present for multiple concept
// On 2D plane, Dot can be :
//
// position - (row, column)
// usually we use x instead of row, y instead of column
//
// area -
// (height, width), which height = x1 - x2, width = y1 - y2
//
// vector/vec -
// (vertical, horizontal), which vertical = x1 - x2, horizontal = y1 - y2

class Dot : public Stringify {
public:
  Dot();
  Dot(int x, int y);

  Dot(const Dot &) = default;
  Dot &operator=(const Dot &) = default;
  Dot(Dot &&) = default;
  Dot &operator=(Dot &&) = default;
  virtual ~Dot() = default;

  const int &x() const;
  Dot &setX(int x);
  const int &y() const;
  Dot &setY(int y);

  bool operator==(const Dot &other) const;
  bool operator!=(const Dot &other) const;

  virtual std::string toString() const;

private:
  int x_;
  int y_;
};

} // namespace fastype
