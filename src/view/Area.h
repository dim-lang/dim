// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "graph2/Dot2.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class Area : public Stringify {
public:
  Area();
  Area(int height, int width);

  Area(const Area &) = default;
  Area &operator=(const Area &) = default;
  Area(Area &&) = default;
  Area &operator=(Area &&) = default;
  virtual ~Area() = default;

  const int &height() const;
  Area &setHeight(int height);
  const int &width() const;
  Area &setWidth(int width);

  bool operator==(const Area &other) const;
  bool operator!=(const Area &other) const;

  // area number
  int number() const;

  virtual std::string toString() const;

private:
  Dot2 dot2_;
};

} // namespace fastype