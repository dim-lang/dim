// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "G2.h"
#include "Stringify.h"
#include "view/Vec.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class Position : public Stringify {
public:
  Position();
  // x is row, y is column
  Position(int x, int y);

  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  Position(Position &&) = default;
  Position &operator=(Position &&) = default;
  virtual ~Position() = default;

  const int &x() const;
  Position &setX(int x);
  const int &y() const;
  Position &setY(int y);

  // compare two position
  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;

  // vector operator
  Position operator-(const Vec &v) const;
  Position &operator-=(const Vec &v) const;
  Position operator+(const Vec &v) const;
  Position &operator+(const Vec &v) const;

  virtual std::string toString() const;

private:
  G2 g2_;
};

} // namespace fastype
