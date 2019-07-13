// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "Log.h"

namespace fastype {

// a logic term view
class View : public Logging {
public:
  View();
  View(const View &) = default;
  View &operator=(const View &) = default;
  virtual ~View() = default;

  int leftBottom() const;
  int rightBottom() const;
  int leftTop() const;
  int rightTop() const;

private:
  int leftBottom_;
  int rightBottom_;
  int leftTop_;
  int rightTop_;
};

} // namespace fastype
