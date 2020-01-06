// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Area.h"
#include "Logging.h"

Area::Area() : dot_() {}

Area::Area(int height, int width) : dot_(height, width) {
  FCHECK(height > 0, "height {} > 0", height);
  FCHECK(width > 0, "width {} > 0", width);
}

const int &Area::height() const { return dot_.x(); }

Area &Area::setHeight(int height) {
  dot_.setX(height);
  return *this;
}

const int &Area::width() const { return dot_.y(); }

Area &Area::setWidth(int width) {
  dot_.setY(width);
  return *this;
}

bool Area::operator==(const Area &other) const { return dot_ == other.dot_; }

bool Area::operator!=(const Area &other) const { return dot_ != other.dot_; }

int Area::number() const {
  FCHECK(dot_.x() > 0, "dot_#x {} > 0", dot_.x());
  FCHECK(dot_.y() > 0, "dot_#y {} > 0", dot_.y());
  return dot_.x() * dot_.y();
}

std::string Area::toString() const {
  return fmt::format("[ @Area height_:{} width_{} number_:{} ]", dot_.x(),
                     dot_.y(), number());
}
