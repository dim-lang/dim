// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Area.h"
#include "Profile.h"
#include "fmt/format.h"

namespace fastype {

Area::Area() : g2_() {}

Area::Area(int height, int width) : g2_(height, width) {
  F_CHECKF(height > 0, "height {} > 0", height);
  F_CHECKF(width > 0, "width {} > 0", width);
}

const int &Area::height() const { return g2_.x(); }

Area &Area::setHeight(int height) {
  g2_.setX(height);
  return *this;
}

const int &Area::width() const { return g2_.y(); }

Area &Area::setWidth(int width) {
  g2_.setY(width);
  return *this;
}

bool Area::operator==(const Area &other) const { return g2_ == other.g2_; }

bool Area::operator!=(const Area &other) const { return g2_ != other.g2_; }

int Area::number() const {
  F_CHECKF(g2_.x() > 0, "g2_#x {} > 0", g2_.x());
  F_CHECKF(g2_.y() > 0, "g2_#y {} > 0", g2_.y());
  return g2_.x() * g2_.y();
}

std::string Area::toString() const {
  return fmt::format("[ @Area height_:{} width_{} number_:{} ]", g2_.x(),
                     g2_.y(), number());
}

} // namespace fastype
