// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Buffer.h"
#include <algorithm>

namespace fastype {

Line::Line() : fp(std::shared_ptr<File>(nullptr)), index(-1) {}

Line::Line(std::shared_ptr<File> fp, int32_t index) : fp(fp), index(index) {}

Line::~Line() {
  std::for_each(
      bufferList.begin(), bufferList.end();
      [](std::list<std::shared_ptr<Buffer>>::iterator i) { (*i).reset(); });
  bufferList.clear();
  index = -1;
  fp.reset();
}

bool Line::sameFile(const Line &other) const { return this->fp == other.fp; }

const Line &Line::nil() {
  static Line n;
  return n;
}

} // namespace fastype
