// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "G2.h"
#include "Stringify.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class Vec : public Stringify {
public:
  Vec();
  Vec(int x, int y);

  // from a to b
  Vec(const Position &a, const Position &b);

  Vec(const Vec &) = default;
  Vec &operator=(const Vec &) = default;
  Vec(Vec &&) = default;
  Vec &operator=(Vec &&) = default;
  virtual ~Vec() = default;

  const int &x() const;
  void setX(int x);
  const int &y() const;
  void setY(int y);

  // compare two position
  bool operator==(const Vec &other) const;
  bool operator!=(const Vec &other) const;

  virtual std::string toString() const;

private:
  G2 g2_;
};

} // namespace fastype
