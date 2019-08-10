// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "graph2/G2.h"
#include "view/Window.h"
#include <memory>

namespace fastype {

class Window;

class Cursor {
public:
  Cursor(std::shared_ptr<Window> window, int row = 0, int col = 0);

  Cursor(const Cursor &) = default;
  Cursor &operator=(const Cursor &) = default;
  Cursor(Cursor &&) = default;
  Cursor &operator=(Cursor &&) = default;
  virtual ~Cursor() = default;

  const int &row() const;
  Cursor &setRow(int row);
  const int &column() const;
  Cursor &setColumn(int column);

  Cursor &moveUp(int length);
  Cursor &moveDown(int length);
  Cursor &moveLeft(int length);
  Cursor &moveRight(int length);

private:
  G2 g2_;
  std::shared_ptr<Window> window_;
};

} // namespace fastype
