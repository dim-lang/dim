// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "Buffer.h"
#include <algorithm>

namespace fastype {

Line &Line::operator++() {
  Line tmp(this->iter);
  this->iter++;
  return tmp;
}

Line &Line::operator--() {
  Line tmp(this->iter);
  this->iter--;
  return tmp;
}

int32_t Line::index() const {
  return std::distance(fp->lineList.begin(), iter);
}

bool Line::sameFile(const Line &other) const {
  return (*iter)->sameFile(*(*other));
}

bool Line::operator==(const Line &other) const {
  assert(fp == other.fp);
  return iter == other.iter;
}
bool Line::operator!=(const Line &other) const {
  assert(fp == other.fp);
  return iter != other.iter;
}
bool Line::operator>(const Line &other) const {
  assert(fp == other.fp);
  return std::distance(other.index, iter) > 0;
}
bool Line::operator>=(const Line &other) const {
  assert(fp == other.fp);
  return *this > other || *this == other;
}
bool Line::operator<(const Line &other) const {
  assert(fp == other.fp);
  return std::distance(other.index, iter) < 0;
}
bool Line::operator<=(const Line &other) const {
  assert(fp == other.fp);
  return *this < other || *this == other;
}

Line::Line(std::vector<std::shared_ptr<detail::LineImpl>>::iterator iter,
           std::shared_ptr<File> fp)
    : iter(iter), fp(fp) {}

namespace detail {

LineImpl::LineImpl() : fp(std::shared_ptr<File>(nullptr)), index(-1) {}

LineImpl::LineImpl(std::shared_ptr<File> fp,
                   std::vector<std::shared_ptr<Buffer>> &&bufferList,
                   int32_t index)
    : fp(fp), index(index) {
  assert(this->bufferList.empty());
  std::swap(this->bufferList, bufferList);
}

LineImpl::~LineImpl() {
  std::for_each(
      bufferList.begin(), bufferList.end();
      [](std::list<std::shared_ptr<Buffer>>::iterator i) { (*i).reset(); });
  bufferList.clear();
  index = -1;
  fp.reset();
}

bool LineImpl::sameFile(const LineImpl &other) const {
  return this->fp == other.fp;
}

const LineImpl &LineImpl::nil() {
  static LineImpl n;
  return n;
}

} // namespace detail

} // namespace fastype
