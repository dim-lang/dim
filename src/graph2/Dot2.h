// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

// Dot2 can be present for multiple concept
// On 2D plane, Dot2 can be :
//
// position - (row, column)
// usually we use x instead of row, y instead of column
//
// area -
// (height, width), which height = x1 - x2, width = y1 - y2
//
// vector/vec -
// (vertical, horizontal), which vertical = x1 - x2, horizontal = y1 - y2

class Dot2 : public Stringify, Logging {
public:
  Dot2();
  Dot2(int x, int y);

  Dot2(const Dot2 &) = default;
  Dot2 &operator=(const Dot2 &) = default;
  Dot2(Dot2 &&) = default;
  Dot2 &operator=(Dot2 &&) = default;
  virtual ~Dot2() = default;

  const int &x() const;
  Dot2 &setX(int x);
  const int &y() const;
  Dot2 &setY(int y);

  bool operator==(const Dot2 &other) const;
  bool operator!=(const Dot2 &other) const;

  virtual std::string toString() const;

private:
  int x_;
  int y_;
};

} // namespace fastype
