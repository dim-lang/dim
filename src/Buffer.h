// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include <memory>

namespace fastype {

class Buffer : private boost::noncopyable {
public:
  Buffer();
  Buffer(int32_t capacity);
  Buffer(int32_t capacity, char value);
  Buffer(char *src, int32_t n);
  virtual ~Buffer();

  // clear buffer
  void clear();
  // release buffer
  void release();
  // expand capacity
  void expand(int32_t capacity);

  // seek operation
  int32_t getSeek() const;
  int32_t setSeek(int32_t seek);
  int32_t reseek();

  // get raw buffer data
  const char *getData() const;
  // get size
  int32_t getSize() const;
  // get capacity
  int32_t getCapacity() const;

  // read n
  int32_t read(char *dest, int32_t n);
  int32_t read(std::shared_ptr<Buffer> buf, int32_t n);
  // read n or until pattern
  int32_t read(char *dest, int32_t n, char *pattern);
  int32_t read(std::shared_ptr<Buffer> buf, int32_t n, char *pattern);

  // write n
  int32_t write(char *src, int32_t n);
  int32_t write(std::shared_ptr<Buffer> buf, int32_t n);
  // write n or until pattern
  int32_t write(char *src, int32_t n, char *pattern);
  int32_t write(std::shared_ptr<Buffer> buf, int32_t n, char *pattern);

  // copy buffer
  std::shared_ptr<Buffer> copy();
  std::shared_ptr<Buffer> copy(int32_t n);

private:
  char *buffer;
  int32_t capacity;
  int32_t size;
  int32_t seek;
};

} // namespace fastype
