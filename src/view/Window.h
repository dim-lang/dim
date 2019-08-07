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

  virtual const Position &r1() const;
  virtual void setR1(const Position &r);

  virtual const Position &r2() const;
  virtual void setR2(const Position &r);

  virtual const Position &r3() const;
  virtual void setR3(const Position &r);

  virtual const Position &r4() const;
  virtual void setR4(const Position &r);

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

  virtual const Position &a1() const;
  virtual void setA1(const Position &a);

  virtual const Position &a2() const;
  virtual void setA2(const Position &a);

  virtual const Position &a3() const;
  virtual void setA3(const Position &a);

  virtual const Position &a4() const;
  virtual void setA4(const Position &a);

  // render
  virtual void update();

private:
  std::vector<Line> lineList_;
};

} // namespace fastype
