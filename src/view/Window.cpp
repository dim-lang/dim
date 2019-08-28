// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Window.h"
#include "ConcurrentHashMap.h"
#include "Logging.h"
#include "Profile.h"
#include <atomic>
#include <fmt/format.h>
#include <ncurses.h>
#include <panel.h>

namespace fastype {

Window::Window()
    : name_("root"), parent_(std::shared_ptr<Window>(nullptr)), p1_(0, 0),
      cursor_(this), window_(nullptr), panel_(nullptr) {
  int x, y;
  window_ = initscr();
  start_color();
  keypad(window_, TRUE);
  noecho();
  cbreak();
  wmove(window_, p1_.x(), p1_.y());
  panel_ = new_panel(window_);
  getmaxyx(window_, x, y);
  area_.setHeight(x);
  area_.setWidth(y);
}

Window::Window(std::shared_ptr<Window> parent, const std::string &name,
               const Position &p1, int height, int width)
    : name_(name), parent_(parent),
      area_(std::min(height, parent->area().height()),
            std::min(width, parent->area().width())),
      p1_(p1), cursor_(this), window_(nullptr), panel_(nullptr) {
  F_CHECKF(name_ != "root", "name_ {} != 'root'", name_);
  F_CHECKF(area_.height() >= 0, "area_#height {} >= 0", area_.height());
  F_CHECKF(area_.height() <= parent_->area().height(),
           "area_#height {} <= parent_#area#height {}", area_.height(),
           parent_->area().height());
  F_CHECKF(area_.width() >= 0, "area_#width{} >= 0", area_.width());
  F_CHECKF(area_.width() <= parent_->area().width(),
           "area_#width {} <= parent_#area#width {}", area_.width(),
           parent_->area().width());

  window_ = newwin(area_.height(), area_.width(), p1_.x(), p1_.y());
  keypad(window_, TRUE);
  wmove(window_, p1_.x(), p1_.y());
  panel_ = new_panel(window_);
}

Window::~Window() {
  if (window_) {
    delwin(window_);
    window_ = nullptr;
  }
  if (panel_) {
    del_panel(panel_);
    panel_ = nullptr;
  }
}

std::shared_ptr<Window> Window::root() {
  static std::shared_ptr<Window> w(new Window());
  return w;
}

static ConcurrentHashMap<std::string, std::shared_ptr<Window>> WindowMap;
static std::atomic_bool WindowMapDirty(false);

std::shared_ptr<Window> Window::open(std::shared_ptr<Window> parent,
                                     const std::string &name,
                                     const Position &p1, int height,
                                     int width) {
  WindowMap.lock();
  if (WindowMap.find(name) == WindowMap.end()) {
    std::shared_ptr<Window> w =
        std::shared_ptr<Window>(new Window(parent, name, p1, height, width));
    WindowMap.insert(std::make_pair(name, w));
    WindowMapDirty = true;
  }
  WindowMap.unlock();
  return WindowMap[name];
}

void Window::close(const std::string &name) {
  WindowMap.lock();
  auto pos = WindowMap.find(name);
  if (pos != WindowMap.end()) {
    WindowMap.remove(pos);
    pos->second.reset();
    WindowMapDirty = true;
  }
  WindowMap.unlock();
}

void Window::close(std::shared_ptr<Window> window) { close(window->name()); }

std::string Window::name() const { return name_; }

void Window::setName(const std::string &name) { name_ = name; }

std::shared_ptr<Window> Window::parent() const { return parent_; }

void Window::setParent(std::shared_ptr<Window> parent) { parent_ = parent; }

const Area &Window::area() const { return area_; }

void Window::setArea(const Area &area) { area_ = area; }

const Position &Window::p1() const { return p1_; }

void Window::setP1(const Position &p) { p1_ = p; }

// p2.x == p1.x
// p2.y == p1.y + width
const Position &Window::p2() const {
  return Position(p1_.x(), p1_.y() + area_.width());
}

void Window::setP2(const Position &p) {
  if (p.x() != p1_.x()) {
    p1_.setX(p.x());
  }
  if (p.y() != p1_.y() + area_.width()) {
    area_.setWidth(p.y() - p1_.y());
  }
}

// p3.x == p1.x + height
// p3.y == p1.y + width
const Position &Window::p3() const {
  return Position(p1_.x() + area_.height(), p1_.y() + area_.width());
}

void Window::setP3(const Position &p) {
  if (p.x() != p1_.x() + area_.height()) {
    area_.setHeight(p.x() - p1_.x());
  }
  if (p.y() != p1_.y() + area_.width()) {
    area_.setWidth(p.y() - p1_.y());
  }
}

// p4.x  == p1.x + height
// p4.y == p1.y
const Position &Window::p4() const {
  return Position(p1_.x() + area_.height(), p1_.y());
}

void Window::setP4(const Position &p) {
  if (p.x() != p1_.x() + area_.height()) {
    area_.setHeight(p.x() - p1_.x());
  }
  if (p.y() != p1_.y()) {
    p1_.setY(p.y());
  }
}

// const Position &Window::absP1() const {
// return parent_ ? (p1() + parentAbsP1()) : p1();
//}

// void Window::setAbsP1(const Position &p) {
// setP1(parent_ ? (p - parentAbsP1()) : p);
//}

// const Position &Window::absP2() const {
// return parent_ ? (p2() + parentAbsP1()) : p2();
//}

// void Window::setAbsP2(const Position &p) {
// setP2(parent_ ? (p - parentAbsP1()) : p);
//}

// const Window::Position &absP3() const {
// return parent_ ? (p3() + parentAbsP1()) : p3();
//}

// void Window::setAbsP3(const Position &p) {
// setP3(parent_ ? (p - parentAbsP1()) : p);
//}

// const Position &Window::absP4() const {
// return parent_ ? (p4() + parentAbsP1()) : p4();
//}

// void Window::setAbsP4(const Position &p) {
// setP4(parent_ ? (p - parentAbsP1()) : p);
//}

void Window::update() {
  if (WindowMapDirty) {
    reorganize();
    WindowMapDirty = false;
  }
}

std::shared_ptr<Row> Window::get(int lineNumber) {
  return lineList_[lineNumber];
}

void Window::set(int lineNumber, std::shared_ptr<Row> l) {
  lineList_[lineNumber] = l;
}

Cursor &Window::cursor() { return cursor_; }

// Vec Window::parentAbsP1() const {
// if (parent_) {
// Position tmp = parent->absP1();
// return Vec(tmp.x(), tmp.y());
//}
// return Vec(0, 0);
//}

Position Window::cursorPosition() {
  int row, column;
  getyx(window_, row, column);
  return Position(row, column);
}

void Window::setCursorPosition(int x, int y) {
  int r = wmove(window_, x, y);
  (void)r;
  F_DEBUGF("x:{} y:{} r:{}", x, y, r);
}

// void Window::showCursor() {
// int r = curs_set(1);
//(void)r;
// F_DEBUGF("r:{}", r);
//}

// void Window::hideCursor() {
// int r = curs_set(0);
//(void)r;
// F_DEBUGF("r:{}", r);
//}

// bool Window::cursorVisible() {}

void Window::reorganize() {}

std::string Window::toString() const {
  return fmt::format("[ @Window p1_:{} area_:{} ]", p1_.toString(),
                     area_.toString());
}

} // namespace fastype
