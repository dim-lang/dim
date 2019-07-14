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
  // empty buffer
  Line();
  // empty buffer with line number
  Line(int32_t lineNumber);
  // buffer with line number and allocated capacity
  Line(int32_t lineNumber, int32_t capacity);
  // buffer with line number, value and allocated capacity
  Line(int32_t lineNumber, int32_t capacity, char value);
  // buffer with line number and allocated capacity
  // copy n bytes of src, start from offset
  Line(int32_t lineNumber, char *src, int32_t offset, int32_t n);
  virtual ~Line();

  // clear data, reset seek and size
  void clear();
  // release buffer, reset seek, size and capacity
  void release();

  // check if capacity is full, e.g `readpos_ == capacity_`
  // @return true if full
  bool full() const;
  // check if capacity is empty, e.g `readpos_ == 0`
  // @return true if empty
  bool empty() const;
  // @return margin, e.g `capacity_ - readpos_`
  int32_t margin() const;
  // @return read position
  int32_t readPosition() const;
  // @return write position
  int32_t writePosition() const;
  // update read position relatively
  // @return old read position
  int32_t updateReadPosition(int32_t update);
  // update write position relatively
  // @return old write position
  int32_t updateWritePosition(int32_t update);
  // set read position absolutely
  // @return old read position
  int32_t setReadPosition(int32_t pos);
  // set write position absolutely
  // @return old write position
  int32_t setWritePosition(int32_t pos);

  // @return this line number
  int32_t lineNumber() const;
  // set this line number
  // @return old this line number
  int32_t setLineNumber(int32_t lineNumber);
  // @return char at begin position, e.g `&buffer_[0]`
  char *begin() const;
  // @return char at read position, e.g `&buffer_[readpos_]`
  char *readAt() const;
  // @return data at write position, e.g `&buffer_[writepos_]`
  char *writeAt() const;
  // expand more capacity, keep readed data
  // @return expanded capacity
  int32_t expand(int32_t capacity);
  // @return capacity
  int32_t capacity() const;

  // insert src[offset ... offset+n] into read position
  // @return bytes
  int32_t insert(const char *src, int32_t offset, int32_t n);
  // read at most one buffer or n bytes from l, start from l.seek
  // @return read bytes
  int32_t insert(Line &l, int32_t n);

  // write at most one buffer or n bytes to dest, start from offset
  // @return write bytes
  int32_t section(char *dest, int32_t offset, int32_t n);
  // write at most one buffer or n bytes to l, start from l.seek
  // @return write bytes
  int32_t section(Line &l, int32_t n);

  virtual std::string toString() const;

  static const Line &undefined();

private:
  int32_t lineNumber_;
  char *buffer_;
  int32_t capacity_;
  int32_t size_;
  int32_t seek_;

  friend class File;
};

} // namespace fastype
