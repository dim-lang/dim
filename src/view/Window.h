// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "boost/noncopyable.hpp"
#include "view/Area.h"
#include "view/Position.h"
#include "view/ViewType.h"
#include <memory>

namespace fastype {

class View : boost::noncopyable {
public:
  // root view's parent is null
  static std::shared_ptr<View> root();

  View(std::shared_ptr<View> parent, const std::string &name);
  virtual ~View();

  virtual std::string name() const;
  virtual void setName(const std::string &name);

  virtual std::shared_ptr<View> parent() const;
  virtual void setParent(std::shared_ptr<View> parent);

  virtual enum ViewType type() const;

  // area
  virtual const Area &area() const;
  virtual void setArea(const Area &a);

  // this View's relative coordinates are:
  //
  // p1-----p2
  //  |     |
  //  |     |
  // p4-----p3
  //
  // 4 relative coordinates must meet these conditions:
  // p1.row == p2.row, p1.column == p4.column
  // p2.column == p3.column, p3.row == p4.row
  // p2.column > p1.column, p3.row > p2.row
  // p4.column < p3.column, p1.row < p4.row
  //
  // in setting methods, these conditions can be temporary broke
  // but checked in getting methods, the same applies to absolute coordinates

  virtual const Position &p1() const;
  virtual void setP1(const Position &r);

  virtual const Position &p2() const;
  virtual void setP2(const Position &r);

  virtual const Position &p3() const;
  virtual void setP3(const Position &r);

  virtual const Position &p4() const;
  virtual void setP4(const Position &r);

  // assume View's parent coordinates are:
  //
  // p5=(x5, y5)------p6=(x6, y6)
  // |                 |
  // |                 |
  // p8=(x8, y8)------p7=(x7, y7)
  //
  // p1 coordinate of this View relative to its parent p5 coordinate is:
  // (r = p1.x - p5.x, c = p1.y - p5.y)
  //
  // then this View's absolute coordinates are:
  //
  // p1=(x5+r, y5+c)-------------p2=(x5+r, y5+c+width-1)
  // |                           |
  // |                           |
  // p4=(x5+r+height-1, y5+c)----p3=(x5+r+height-1, y5+c+width-1)
  //
  // as picture shows:
  //
  // (x5,y5)                     (x6,y6)
  //  o---------------------------o
  //  |                           |
  //  | (x5+r,y5+c)   (x5+r, y5+c+width-1)
  //  |      o--------------o     |
  //  |      |              |     |
  //  |      |              |     |
  //  |      |              |     |
  //  |      |              |     |
  //  |      o--------------o     |
  //  |     ...            ...    |
  //  |                           |
  //  o---------------------------o
  // (x8,y8)                     (x7,y7)

  virtual const Position &absP1() const;
  virtual void setAbsP1(const Position &a);

  virtual const Position &absP2() const;
  virtual void setAbsP2(const Position &a);

  virtual const Position &absP3() const;
  virtual void setAbsP3(const Position &a);

  virtual const Position &absP4() const;
  virtual void setAbsP4(const Position &a);

  // render
  virtual void update();

  // view line conditions:
  // line#size <= width
  // 0 <= lineNumber < height
  virtual const Line &get(int lineNumber);
  virtual void set(int lineNumber, const Line &l);

private:
  // root constructor
  Window();

  std::vector<Line> lineList_;
  Area area_;
  Position p1_; // relative p1
};

} // namespace fastype
