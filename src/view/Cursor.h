// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "graph/Dot.h"

class Window;

class Cursor {
public:
  virtual ~Cursor() = default;

  Cursor(const Cursor &) = default;
  Cursor &operator=(const Cursor &) = default;
  Cursor(Cursor &&) = default;
  Cursor &operator=(Cursor &&) = default;

  int row();
  void setRow(int row);
  int column();
  void setColumn(int column);

  void up(int length);
  void down(int length);
  void left(int length);
  void right(int length);

  // void show();
  // void hide();
  // bool visible();

private:
  Cursor(Window *window);

  Window *window_;

  friend class Window;
};
