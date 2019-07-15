// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <memory>

namespace fastype {

class File;
class Line;

class Line : private boost::noncopyable, public Logging, Stringify {
public:
  // empty
  Line();
  // line number
  Line(int32_t lineNumber);
  // line number, allocate memory
  Line(int32_t lineNumber, int32_t capacity);
  // line number, allocate memory, fill value
  Line(int32_t lineNumber, int32_t capacity, char value);
  // line number, copyed from substring
  Line(int32_t lineNumber, char *src, int32_t offset);
  Line(int32_t lineNumber, char *src, int32_t offset, int32_t n);
  // line number, copyed from substring
  Line(int32_t lineNumber, char *first, char *last);
  virtual ~Line();

  // clear data, reset seek and size
  void clear();
  // release buffer, reset seek, size and capacity
  void release();

  // check if capacity is full, e.g `size_ == capacity_`
  // @return true if full
  bool full() const;
  // check if capacity is empty, e.g `size_ == 0`
  // @return true if empty
  bool empty() const;
  // @return margin, e.g `capacity_ - size_`
  int32_t margin() const;
  // @return size
  int32_t size() const;
  // @return line number
  int32_t lineNumber() const;
  // set line number
  // @return old line number
  int32_t setLineNumber(int32_t lineNumber);
  // @return char at begin position, e.g `&buffer_[0]`
  char *begin() const;
  // @return char at end position, e.g `&buffer_[size_]`
  char *end() const;
  // expand more capacity, keep readed data
  // @return expanded capacity
  int32_t expand(int32_t capacity);
  // @return capacity
  int32_t capacity() const;

  // insert bytes into line
  // insert src[offset...offset+n] at pos
  // @return inserted bytes
  int32_t insert(int32_t pos, const char *src, int32_t offset, int32_t n);
  int32_t insert(int32_t pos, const Line &l, int32_t offset, int32_t n);

  // append bytes at end of line
  // append src[offset...offset+n] at end of line, e.g `&buffer_[size_]`
  // @return appended bytes
  int32_t append(const char *src, int32_t offset, int32_t n);
  int32_t append(const Line &l, int32_t offset, int32_t n);

  // get substring of line
  // copy buffer_[offset...offset+n] to dest[offset2...offset2+n2]
  // @return substring bytes
  int32_t substring(int32_t offset, int32_t n, char *dest, int32_t offset2,
                    int32_t n2);
  int32_t substring(int32_t offset, int32_t n, Line &dest, int32_t offset2,
                    int32_t n2);

  virtual std::string toString() const;

  static const Line &undefined();

private:
  int32_t lineNumber_;
  std::string buffer_;

  friend class File;
};

} // namespace fastype
