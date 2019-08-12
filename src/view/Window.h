// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "Logging.h"
#include "boost/noncopyable.hpp"
#include "view/Area.h"
#include "view/Cursor.h"
#include "view/Position.h"
#include "view/Vec.h"
#include <atomic>
#include <memory>
#include <ncurses.h>
#include <panel.h>

namespace fastype {

class Window : boost::noncopyable, public Logging, Stringify {
public:
  virtual ~Window();

  // root window's parent is null
  static std::shared_ptr<Window> root();

  static std::shared_ptr<Window> open(std::shared_ptr<Window> parent,
                                      const std::string &name,
                                      const Position &p1, int height,
                                      int width);

  static void close(std::shared_ptr<Window> window);

  static void close(const std::string &name);

  virtual std::string name() const;
  virtual void setName(const std::string &name);

  virtual std::shared_ptr<Window> parent() const;
  virtual void setParent(std::shared_ptr<Window> parent);

  // area
  virtual const Area &area() const;
  virtual void setArea(const Area &area);

  // p1 vector relative to parent p1

  // this Window's relative coordinates are:
  //
  // p1-----p2
  //  |     |
  //  |     |
  // p4-----p3
  //
  // 4 relative coordinates must meet these conditions:
  // p1.x == p2.x, p1.y == p4.y
  // p2.y == p3.y, p3.x == p4.x
  // p2.y > p1.y, p3.x > p2.x
  // p4.y < p3.y, p1.x < p4.x
  //
  // in setting methods, these conditions can be temporary broke
  // but checked in getting methods, the same applies to absolute coordinates

  virtual const Position &p1() const;
  virtual void setP1(const Position &p);

  virtual const Position &p2() const;
  virtual void setP2(const Position &p);

  virtual const Position &p3() const;
  virtual void setP3(const Position &p);

  virtual const Position &p4() const;
  virtual void setP4(const Position &p);

  // assume Window's parent coordinates are:
  //
  // p5=(x5, y5)------p6=(x6, y6)
  // |                 |
  // |                 |
  // p8=(x8, y8)------p7=(x7, y7)
  //
  // p1 coordinate of this Window relative to its parent p5 coordinate is:
  // (r = p1.x - p5.x, c = p1.y - p5.y)
  //
  // then this Window's absolute coordinates are:
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

  // virtual const Position &absP1() const;
  // virtual void setAbsP1(const Position &p);

  // virtual const Position &absP2() const;
  // virtual void setAbsP2(const Position &p);

  // virtual const Position &absP3() const;
  // virtual void setAbsP3(const Position &p);

  // virtual const Position &absP4() const;
  // virtual void setAbsP4(const Position &p);

  // render
  virtual void update();

  // window line conditions:
  // line#size <= width
  // 0 <= lineNumber < height
  virtual const Line &get(int lineNumber);

  virtual void set(int lineNumber, const Line &l);

  virtual Cursor &cursor();

  virtual std::string toString() const;

private:
  // root constructor
  Window();

  // non-root constructor
  Window(std::shared_ptr<Window> parent, const std::string &name,
         const Position &p1, int height, int width);

  // Vec parentAbsP1() const;

  // sort all windows in parent order
  static void reorganize();

  // cursor position
  Position cursorPosition();
  void setCursorPosition(int x, int y);
  // void showCursor();
  // void hideCursor();
  // bool cursorVisible();
  // void cursorEnter();
  // void cursorLeave();

  // graph
  std::string name_;
  std::shared_ptr<Window> parent_;
  Area area_;
  Position p1_; // relative p1

  // render
  std::vector<Line> lineList_;
  Cursor cursor_;
  WINDOW *window_;
  PANEL *panel_;

  friend class Cursor;
};

} // namespace fastype
