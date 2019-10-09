// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "graph/Dot.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace fastype {

class Vec : public Stringify {
public:
  Vec();
  Vec(int x, int y);

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
  Dot dot_;
};

} // namespace fastype
