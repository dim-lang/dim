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

namespace detail {

// G2 can be present for multiple concept
// On 2D plane, G2 can be :
//
// position - (row, column)
// usually we use x instead of row, y instead of column
//
// area -
// (height, width), which height = x1 - x2, width = y1 - y2
//
// vector/vec -
// (vertical, horizontal), which vertical = x1 - x2, horizontal = y1 - y2

class G2 : public Stringify, Logging {
public:
  G2();
  G2(int x, int y);

  G2(const G2 &) = default;
  G2 &operator=(const G2 &) = default;
  G2(G2 &&) = default;
  G2 &operator=(G2 &&) = default;
  virtual ~G2() = default;

  const int &x() const;
  int setX(int x);
  const int &y() const;
  int setY(int y);

  bool operator==(const G2 &other) const;
  bool operator!=(const G2 &other) const;

  virtual std::string toString() const;

private:
  int x_;
  int y_;
};

} // namespace detail

} // namespace fastype
