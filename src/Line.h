// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "File.h"
#include "LineBound.h"
#include "Log.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <vector>

namespace fastype {

class File;

class Line : public Logging {
public:
  Line(const Line &) = default;
  Line &operator=(const Line &) = default;
  virtual ~Line() = default;

  bool operator==(const Line &other) const;
  bool operator!=(const Line &other) const;
  bool operator>(const Line &other) const;
  bool operator>=(const Line &other) const;
  bool operator<(const Line &other) const;
  bool operator<=(const Line &other) const;

  static const Line &undefined();
  int32_t lineNumber() const;
  int32_t setLineNumber(int32_t lineNumber);
  const LineBound &left() const;
  LineBound setLeft(const LineBound &left);
  const LineBound &right() const;
  LineBound setRight(const LineBound &right);

  std::string toString() const;

private:
  Line(int32_t lineNumber, const LineBound &left, const LineBound &right);
  Line(int32_t lineNumber, const LineBound &left);
  Line(int32_t lineNumber);

  int32_t lineNumber_;
  LineBound left_;
  LineBound right_;

  friend class File;
  friend class Buffer;
};

} // namespace fastype
