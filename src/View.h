// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "Line.h"
#include "Log.h"

namespace fastype {

// logic term view
class View : public Logging {
public:
  View();
  View(int width, int height);
  View(const View &) = default;
  View &operator=(const View &) = default;
  virtual ~View() = default;

  int width() const;
  int height() const;
  void resize(int width, int height);
  int setWidth(int width);
  int setHeight(int height);

private:
  int width_;
  int height_;
  std::vector<Line> lineList_;
};

} // namespace fastype
