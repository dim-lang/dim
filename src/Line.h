// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "File.h"
#include "LineBound.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <vector>

namespace fastype {

class File;

class Line {
public:
  Line(const Line &) = default;
  Line &operator=(const Line &) = default;
  virtual ~Line() = default;

  bool hasNext() const;
  Line next();
  bool hasPrevious() const;
  Line previous();
  Line &operator++();
  Line &operator--();
  bool sameFile(const Line &other) const;

  bool operator==(const Line &other) const;
  bool operator!=(const Line &other) const;
  bool operator>(const Line &other) const;
  bool operator>=(const Line &other) const;
  bool operator<(const Line &other) const;
  bool operator<=(const Line &other) const;

  int32_t lineNumber() const;
  int32_t setLineNumber(int32_t lineNumber);
  const LineBound &left() const;
  LineBound setLeft(const LineBound &left);
  const LineBound &right() const;
  LineBound setRight(const LineBound &right);

private:
  Line(std::shared_ptr<File> fp, int32_t lineNumber, const LineBound &left,
       const LineBound &right);
  Line(std::shared_ptr<File> fp, int32_t lineNumber, const LineBound &left);
  Line(std::shared_ptr<File> fp, int32_t lineNumber);

  static const Line &undefinedLine();

  std::shared_ptr<File> fp_;
  int32_t lineNumber_;
  LineBound left_;
  LineBound right_;

  friend class File;
  friend class Buffer;
};

} // namespace fastype
