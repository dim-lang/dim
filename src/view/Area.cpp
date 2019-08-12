// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Area.h"
#include "Profile.h"
#include "fmt/format.h"

namespace fastype {

Area::Area() : dot2_() {}

Area::Area(int height, int width) : dot2_(height, width) {
  F_CHECKF(height > 0, "height {} > 0", height);
  F_CHECKF(width > 0, "width {} > 0", width);
}

const int &Area::height() const { return dot2_.x(); }

Area &Area::setHeight(int height) {
  dot2_.setX(height);
  return *this;
}

const int &Area::width() const { return dot2_.y(); }

Area &Area::setWidth(int width) {
  dot2_.setY(width);
  return *this;
}

bool Area::operator==(const Area &other) const { return dot2_ == other.dot2_; }

bool Area::operator!=(const Area &other) const { return dot2_ != other.dot2_; }

int Area::number() const {
  F_CHECKF(dot2_.x() > 0, "dot2_#x {} > 0", dot2_.x());
  F_CHECKF(dot2_.y() > 0, "dot2_#y {} > 0", dot2_.y());
  return dot2_.x() * dot2_.y();
}

std::string Area::toString() const {
  return fmt::format("[ @Area height_:{} width_{} number_:{} ]", dot2_.x(),
                     dot2_.y(), number());
}

} // namespace fastype
