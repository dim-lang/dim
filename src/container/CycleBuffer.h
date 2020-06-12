// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "interface/Stringify.h"
#include <cstdio>

namespace detail {

template <unsigned int D> class CycleBuffer : public Stringify {
public:
  CycleBuffer();
  virtual ~CycleBuffer();

  virtual int capacity() const;
  virtual int size() const;
  virtual bool empty() const;
  virtual bool full() const;
  virtual void reset();

  // next position start from <position>
  virtual char *next(char *position, int distance = 1) const;
  virtual const char *next(const char *position, int distance = 1) const;
  // previous position start from <position>
  virtual char *prev(char *position, int distance = 1) const;
  virtual const char *prev(const char *position, int distance = 1) const;

  virtual char *begin();
  virtual const char *begin() const;
  virtual char *rbegin();
  virtual const char *rbegin() const;

  virtual char *end();
  virtual const char *end() const;
  virtual char *rend();
  virtual const char *rend() const;

  virtual bool contain(const char *position) const;

  virtual std::string toString() const;

  // write at most <n> bytes to <buf>
  // @return    bytes really write
  virtual int write(char *buf, int n);

  // read at most <n> bytes from <buf>
  // @return    bytes really read
  virtual int read(const char *buf, int n);

  // write all bytes to <fp>
  // @return    bytes really write
  virtual int fpwrite(FILE *fp);

  // write at most <n> bytes to <fp>
  virtual int fpwrite(FILE *fp, int n);

  // read all bytes from <fp>
  // @return    bytes really read
  virtual int fpread(FILE *fp);

  // read at most <n> bytes from <fp>
  // @return    bytes really read
  virtual int fpread(FILE *fp, int n);

protected:
  virtual char *nextImpl(char *position, int distance = 1) const;
  virtual char *prevImpl(char *position, int distance = 1) const;
  virtual void release();
  virtual int expand(int n) = 0;

  char *buf_;
  char *head_;
  char *tail_;
  int capacity_;
};

} // namespace detail

class DynamicBuffer : public detail::CycleBuffer<1> {
public:
  DynamicBuffer(int capacity = 0);
  virtual ~DynamicBuffer() = default;
  virtual std::string toString() const;

protected:
  virtual int expand(int n);
};

class FixedBuffer : public detail::CycleBuffer<0> {
public:
  FixedBuffer(int capacity);
  virtual ~FixedBuffer() = default;
  virtual std::string toString() const;

protected:
  virtual int expand(int n);
};
