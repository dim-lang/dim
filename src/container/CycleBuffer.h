// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "interface/Stringify.h"
#include <cstdio>

namespace detail {

class CycleBuffer : public Stringify {
public:
  CycleBuffer();
  virtual ~CycleBuffer();

  virtual int capacity() const;
  virtual int size() const;
  virtual bool empty() const;
  virtual bool full() const;

  // next position start from <position>
  virtual char *next(char *position) const;
  virtual const char *next(const char *position) const;
  // previous position start from <position>
  virtual char *prev(char *position) const;
  virtual const char *prev(const char *position) const;

  virtual char *begin();
  virtual const char *begin() const;
  virtual char *rbegin();
  virtual const char *rbegin() const;

  virtual char *end();
  virtual const char *end() const;
  virtual char *rend();
  virtual const char *rend() const;

  virtual bool contain(const char *position) const;

  // write at most <n> bytes from <buf> to CycleBuffer
  // @return    bytes really write
  virtual int write(const char *buf, int n) = 0;
  // read at most <n> bytes to <buf> from CycleBuffer
  // @return    bytes really read
  virtual int read(char *buf, int n) = 0;

  // write at most <n> bytes from <fp> to CycleBuffer
  // @return    bytes really write
  virtual int writeFile(FILE *fp, int n) = 0;

  // read at most <n> bytes to <fp> from CycleBuffer
  // @return    bytes really read
  virtual int readFile(FILE *fp, int n) = 0;

  virtual std::string toString() const;

protected:
  virtual char *nextImpl(char *position) const;
  virtual char *prevImpl(char *position) const;
  virtual void release();

  char *buf_;
  char *head_;
  char *tail_;
  int capacity_;
};

} // namespace detail

class DynamicBuffer : public detail::CycleBuffer {
public:
  DynamicBuffer(int capacity = 0);
  virtual ~DynamicBuffer() = default;

  // write at most <n> bytes from <buf> to CycleBuffer
  // @return    bytes really write
  virtual int write(const char *buf, int n);
  // read at most <n> bytes to <buf> from CycleBuffer
  // @return    bytes really read
  virtual int read(char *buf, int n);

  // write at most <n> bytes from <fp> to CycleBuffer
  // @return    bytes really write
  virtual int writeFile(FILE *fp, int n);

  // read at most <n> bytes to <fp> from CycleBuffer
  // @return    bytes really read
  virtual int readFile(FILE *fp, int n);

  virtual std::string toString() const;

private:
  virtual int expand(int n);
};

class FixedBuffer : public detail::CycleBuffer {
public:
  FixedBuffer(int capacity);
  virtual ~FixedBuffer() = default;

  // write at most <n> bytes from <buf> to CycleBuffer
  // @return    bytes really write
  virtual int write(const char *buf, int n);
  // read at most <n> bytes to <buf> from CycleBuffer
  // @return    bytes really read
  virtual int read(char *buf, int n);

  // write at most <n> bytes from <fp> to CycleBuffer
  // @return    bytes really write
  virtual int writeFile(FILE *fp, int n);

  // read at most <n> bytes to <fp> from CycleBuffer
  // @return    bytes really read
  virtual int readFile(FILE *fp, int n);

  virtual std::string toString() const;
};
