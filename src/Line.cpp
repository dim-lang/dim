// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Buffer.h"
#include <algorithm>

namespace fastype {

Line::Line() : index(-1) {}

Line::Line(int32_t index) : index(index) {}

Line::~Line() {
  std::for_each(
      bufferList.begin(), bufferList.end();
      [](std::list<std::shared_ptr<Buffer>>::iterator i) { delete i; });
  bufferList.clear();
  index = -1;
}

} // namespace fastype
