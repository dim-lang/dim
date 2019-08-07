// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Window.h"

namespace fastype {

Window::Window() {}

std::shared_ptr<View> Window::root() {
  static std::shared_ptr<Window> w(new Window());
  return w;
}

View(std::shared_ptr<View> parent, const std::string &name);
virtual ~View();

virtual std::string name() const;
virtual void setName(const std::string &name);

virtual std::shared_ptr<View> parent() const;
virtual void setParent(std::shared_ptr<View> parent);

virtual enum ViewType type() const;

virtual const Area &area() const;
virtual void setArea(const Area &a);

virtual const Position &p1() const;
virtual void setP1(const Position &r);

virtual const Position &p2() const;
virtual void setP2(const Position &r);

virtual const Position &p3() const;
virtual void setP3(const Position &r);

virtual const Position &p4() const;
virtual void setP4(const Position &r);

virtual const Position &absP1() const;
virtual void setAbsP1(const Position &a);

virtual const Position &absP2() const;
virtual void setAbsP2(const Position &a);

virtual const Position &absP3() const;
virtual void setAbsP3(const Position &a);

virtual const Position &absP4() const;
virtual void setAbsP4(const Position &a);

virtual void update();

virtual const Line &get(int lineNumber);
virtual void set(int lineNumber, const Line &l);

} // namespace fastype
