// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Window.h"
#include "ConcurrentHashMap.h"
#include "Profile.h"
#include <atomic>
#include <ncurses.h>
#include <panel.h>

namespace fastype {

Window::Window()
    : Logging("Window"), name_("root"),
      parent_(std::shared_ptr<Window>(nullptr)), p1_(0, 0),
      cursor_(std::shared_ptr<Window>(this)), window_(nullptr),
      panel_(nullptr) {
  int row, col;
  window_ = initscr();
  panel_ = new_panel(window_);
  getyxmax(window_, row, col);
  area_.setHeight(row);
  area_.setWidth(col);
}

Window(std::shared_ptr<Window> parent, const std::string &name,
       const Position &p1, int height, int width)
    : Logging("Window"), name_(name), parent_(parent),
      area_(std::min(height, parent->area().height()),
            std::min(width, parent->area().width())),
      p1_(p1), cursor_(std::shared_ptr<Window>(this)), window_(nullptr),
      panel_(nullptr) {
  F_CHECKF(p1_.row() != "root", "name_ {} != 'root'", name_);
  F_CHECKF(name_ != "root", "name_ {} != 'root'", name_);
  F_CHECKF(area_.height() >= 0, "area_#height {} >= 0", area_.height());
  F_CHECKF(area_.height() <= parent_->area().height(),
           "area_#height {} <= parent_#area#height {}", area_.height(),
           parent_->area().height());
  F_CHECKF(area_.width() >= 0, "area_#width{} >= 0", area_.width());
  F_CHECKF(area_.width() <= parent_->area().width(),
           "area_#width {} <= parent_#area#width {}", area_.width(),
           parent_->area().width());

  window_ = newwin(area_.height(), area_.width(), p1_.row(), p1_.column());
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
static std::atomic_bool WindowMapDirty = false;

std::shared_ptr<Window> Window::open(std::shared_ptr<Window> parent,
                                     const std::string &name) {
  WindowMap.lock();
  if (WindowMap.find(name) == WindowMap.end()) {
    std::shared_ptr<Window> w = std::shared_ptr<Window>(new Window());
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
    WindowMap.erase(pos);
    delete pos->second;
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

void Window::setArea(const Area &area) { area_ = a; }

const Position &Window::p1() const { return p1_; }

void Window::setP1(const Position &p) { p1_ = p; }

const Position &Window::p2() const {
  // p2 = p1 + width
  return Position(p1_.row(), p1_.column() + area_.width());
}

void Window::setP2(const Position &p) {}

const Position &p3() const;
void setP3(const Position &p);

const Position &p4() const;
void setP4(const Position &p);

const Position &absP1() const;
void setAbsP1(const Position &p);

const Position &absP2() const;
void setAbsP2(const Position &p);

const Position &absP3() const;
void setAbsP3(const Position &p);

const Position &absP4() const;
void setAbsP4(const Position &p);

void Window::update() {
  if (WindowMapDirty) {
    reorganize();
    WindowMapDirty = false;
  }
}

const Line &get(int lineNumber);
void set(int lineNumber, const Line &l);

Cursor &Window::cursor() { return cursor_; }

} // namespace fastype
