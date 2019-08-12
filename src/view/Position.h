// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "graph2/Dot2.h"
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
  void setX(int x);
  const int &y() const;
  void setY(int y);

  // compare two position
  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;

  // vector operator
  Position operator-(const Vec &v) const;
  Position &operator-=(const Vec &v);
  Position operator+(const Vec &v) const;
  Position &operator+=(const Vec &v);

  virtual std::string toString() const;

private:
  Dot2 dot2_;
};

} // namespace fastype
