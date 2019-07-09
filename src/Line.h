// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include "File.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <vector>

namespace fastype {

class File;
namespace detail {
class LineImpl;
}

class Line {
public:
  Line(const Line &) = default;
  Line &operator=(const Line &) = default;
  virtual ~Line() = default;

  Line &operator++();
  Line &operator--();
  int32_t index() const;
  bool sameFile(const Line &other);

  bool operator==(const Line &other) const;
  bool operator!=(const Line &other) const;
  bool operator>(const Line &other) const;
  bool operator>=(const Line &other) const;
  bool operator<(const Line &other) const;
  bool operator<=(const Line &other) const;

private:
  Line(std::vector<std::shared_ptr<detail::LineImpl>>::iterator iter,
       std::shared_ptr<File> fp);

  std::vector<std::shared_ptr<detail::LineImpl>>::iterator iter;
  std::shared_ptr<File> fp;
};

namespace detail {

class LineImpl {
public:
  LineImpl(const LineImpl &) = default;
  LineImpl &operator=(const LineImpl &) = default;
  virtual ~LineImpl();

  bool sameFile(const LineImpl &other) const;

  static const LineImpl &nil();

private:
  LineImpl(int32_t index);
  LineImpl(std::shared_ptr<File> fp,
           std::vector<std::shared_ptr<Buffer>> &&bufferList, int32_t index);

  int32_t index;
  std::vector<std::shared_ptr<Buffer>> bufferList;
  std::shared_ptr<File> fp;

  friend class File;
};

} // namespace detail

} // namespace fastype
