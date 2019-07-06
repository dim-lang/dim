// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "File.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <memory>

namespace fastype {

class Line : private boost::noncopyable {
public:
  // empty line
  Line();
  // empty line with capacity
  Line(int32_t capacity);
  // initialize line with value of capacity
  Line(int32_t capacity, char value);
  // copy n bytes of src, start from offset
  Line(char *src, int32_t offset, int32_t n);
  virtual ~Line();

  Line(Line &&) = default;
  Line &operator=(Line &&) = default;

  // clear data, reset seek and size
  void clear();

  // release line
  void release();

  // expand more capacity
  // @return expanded capacity
  int32_t expand(int32_t capacity);

  // check if capacity is full
  // @return true if size == capacity
  bool full() const;
  // check if capacity is empty
  // @return true if size == 0
  bool empty() const;

  // @return seek
  int32_t getSeek() const;
  // @return old seek
  int32_t setSeek(int32_t seek);
  // @return old seek
  int32_t increaseSeek(int32_t inc);
  // @return old seek
  int32_t decreaseSeek(int32_t dec);
  // reset seek potision at 0
  // @return old seek position
  int32_t reset();

  // check if seek position is at begin
  // @return true if seek == 0
  bool begin() const;
  // check if seek position is at end
  // @return true if seek == size
  bool end() const;

  // @return raw data at seek position
  const char *getData() const;
  // @return size
  int32_t getSize() const;
  // @return capacity
  int32_t getCapacity() const;

  // read at most n bytes from src, start from offset
  // @return read bytes
  int32_t read(const char *src, int32_t offset, int32_t n);
  // read at most n bytes from l, start from l.seek
  // @return read bytes
  int32_t read(Line &l, int32_t n);

  // write at most n bytes to dest, start from offset
  // @return write bytes
  int32_t write(char *dest, int32_t offset, int32_t n) const;
  // write at most n bytes to l, start from l.seek
  // @return write bytes
  int32_t write(Line &l, int32_t n) const;

private:
  char *buffer;
  int32_t capacity;
  int32_t size;
  int32_t seek;

  friend class File;
};

} // namespace fastype
