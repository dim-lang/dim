// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "ViewType.h"
#include <memory>

namespace fastype {

class View {
public:
  virtual std::string name() const = 0;
  virtual void setName(const std::string &name) = 0;
  virtual std::shared_ptr<View> parent() const = 0;
  virtual void setParent(std::shared_ptr<View> parent) = 0;
  virtual enum ViewType type() const = 0;

  //
  //  row
  //  |
  //  |
  //  x------------(x,y)
  //  |             |
  //  |             |
  //  |             |
  //  o-------------y------col
  //

  // relatively
  // this *View* coordinates are:
  //
  // (0, 0)------------------(0, width-1)
  //  |                                |
  //  |                                |
  // (height-1, 0)----(height-1, width-1)

  virtual int height() const = 0;
  virtual int width() const = 0;
  virtual void setHeight(int height) = 0;
  virtual void setWidth(int width) = 0;

  // absolutely
  // assume parent coordinates are:
  //
  // (x1, y1)------(x2, y2)
  //  |                  |
  //  |                  |
  // (x3, y3)------(x4, y4)
  //
  // then this *View* coordinates are:
  //
  // (x1+row, y1+column)------------------------(x1+row, y1+column+width-1)
  //  |                                                                  |
  //  |                                                                  |
  // (x1+row+height-1, y1+column-1)----(x1+row+height-1, y1+column+width-1)
  //
  // as picture shows:
  //
  // (x1,y1)                     (x2,y2)
  //  o---------------------------o
  //  |                           |
  //  | (x1+row,y1+column)        |
  //  |      o--------------o     |
  //  |      |              |     |
  //  |      |              |     |
  //  |      |              |     |
  //  |      |              |     |
  //  |      o--------------o     |
  //  |                           |
  //  |                           |
  //  o---------------------------o
  // (x3,y3)                     (x4,y4)

  virtual int row() const = 0;
  virtual int column() const = 0;
  virtual void setRow(int row) = 0;
  virtual void setColumn(int column) = 0;

  virtual int leftUp() const = 0;
  virtual int rightUp() const = 0;
  virtual int leftDown() const = 0;
  virtual int rightDown() const = 0;

  virtual void update() = 0;
};

} // namespace fastype
