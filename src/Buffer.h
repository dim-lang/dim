// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <memory>

namespace fastype {

class File;
class Line;

class Buffer : private boost::noncopyable {
public:
  // empty buffer
  Buffer();
  // empty buffer with capacity
  Buffer(int32_t capacity);
  // initialize buffer with value of capacity
  Buffer(int32_t capacity, char value);
  // copy n bytes of src, start from offset
  Buffer(char *src, int32_t offset, int32_t n);
  virtual ~Buffer();

  Buffer(Buffer &&) = default;
  Buffer &operator=(Buffer &&) = default;

  // clear data, reset seek and size
  void clear();

  // release buffer
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
  // @return margin, capacity - size
  int32_t margin() const;

  // @return seek
  int32_t seek() const;
  // @return old seek
  int32_t increase(int32_t inc);
  // @return old seek
  int32_t decrease(int32_t dec);
  // reset seek potision at 0
  // @return old seek position
  int32_t reseek();

  // @return index
  int32_t index() const;

  // check if seek position is at begin
  // @return true if seek == 0
  bool begin() const;
  // check if seek position is at end
  // @return true if seek == size
  bool end() const;

  // @return raw data at seek position
  char *data() const;

  // @return size
  int32_t size() const;
  // @return old size
  int32_t setSize(int32_t size);

  // @return capacity
  int32_t capacity() const;

  // read at most one buffer or n bytes from src, start from offset
  // @return read bytes
  int32_t read(const char *src, int32_t offset, int32_t n);
  // read at most one buffer or n bytes from l, start from l.seek
  // @return read bytes
  int32_t read(Buffer &l, int32_t n);

  // write at most one buffer or n bytes to dest, start from offset
  // @return write bytes
  int32_t write(char *dest, int32_t offset, int32_t n) const;
  // write at most one buffer or n bytes to l, start from l.seek
  // @return write bytes
  int32_t write(Buffer &l, int32_t n) const;

  std::string toString() const;

private:
  char *buffer_;
  int32_t capacity_;
  int32_t size_;
  int32_t seek_;
  std::shared_ptr<Logger> log_;

  friend class File;
  friend class Line;
};

} // namespace fastype
