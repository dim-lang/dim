// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include <memory>

namespace fastype {

class Line : private boost::noncopyable {
public:
  // empty buffer
  Line();
  // empty buffer with capacity
  Line(int32_t capacity);
  // initialize buffer with value of capacity
  Line(int32_t capacity, char value);
  // copy src of len
  Line(char *src, int32_t off, int32_t len);
  virtual ~Line();

  Line(Line &&) = default;
  Line &operator=(Line &&) = default;

  // clear buffer, reset seek and size
  void clear();

  // release buffer
  void release();

  // capacity
  // @return expanded capacity
  int32_t expandCapacity(int32_t capacity);

  // capacity is full, size == capacity
  bool full() const;
  // capacity is empty, size == 0
  bool empty() const;

  // seek
  int32_t getSeek() const;
  int32_t setSeek(int32_t seek);
  int32_t reseek();

  // seek position at begin, seek == 0
  bool begin() const;
  // seek position at begin, seek == size
  bool end() const;

  // get raw data
  const char *getData() const;
  // get size
  int32_t getSize() const;
  // get capacity
  int32_t getCapacity() const;

  // read len bytes start from off
  int32_t read(const char *src, int32_t off, int32_t len);
  int32_t read(Line &l, int32_t len);

  // write len
  int32_t write(char *dest, int32_t off, int32_t len) const;
  int32_t write(Line &l, int32_t len) const;

private:
  char *buffer;
  int32_t capacity;
  int32_t size;
  int32_t seek;
};

} // namespace fastype
