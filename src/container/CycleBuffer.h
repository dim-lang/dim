// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include <cstdio>

namespace detail {

/**
 * CycleBuffer memory status
 *
 * positive: tail >= head
 * 1)
 * buf = head = tail = nullptr
 *
 * 2)
 * head                    tail
 * |                       |
 * |-----------------------||
 * |
 * buf
 *
 * 3)
 *           head
 *           |
 * |---------||-------------|
 * |          |
 * buf        tail
 *
 * 4)
 *      head            tail
 *      |               |
 * |----|---------------|---|
 * |
 * buf
 *
 * negative: tail < head
 * 1)
 *      tail            head
 *      |               |
 * |----|---------------|---|
 * |
 * buf
 *
 * 2)
 * tail             head
 * |                |
 * |----------------|-------|
 * |
 * buf
 *
 * 3)
 *      tail               head
 *      |                  |
 * |----|------------------||
 * |
 * buf
 *
 * 4)
 * tail                    head
 * |                       |
 * |-----------------------||
 * |
 * buf
 *
 * impossible)
 * tail                     head
 * |                        |
 * |------------------------|
 * |
 * buf
 */

template <unsigned int D> class CycleBuffer {
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

  virtual char *search(char c);
  virtual const char *search(char c) const;

  virtual char *search(char *s, int n);
  virtual const char *search(const char *s, int n) const;

  virtual char *rsearch(char c);
  virtual const char *rsearch(char c) const;

  virtual char *rsearch(char *s, int n);
  virtual const char *rsearch(const char *s, int n) const;

  virtual Cowstr str() const;

  // write at most <n> bytes to <buf>
  // @return    <buf>
  virtual Cowstr write(int n);

  // read at most <n> bytes from <buf>
  // @return    bytes really read
  /* virtual int read(const char *buf, int n); */
  virtual int read(const Cowstr &buf);

  // write all bytes to <fp>
  // @return    bytes really write
  virtual int writefile(FILE *fp);

  // write at most <n> bytes to <fp>
  virtual int writefile(FILE *fp, int n);

  // read all bytes from <fp>
  // @return    bytes really read
  virtual int readfile(FILE *fp);

  // read at most <n> bytes from <fp>
  // @return    bytes really read
  virtual int readfile(FILE *fp, int n);

protected:
  virtual char *nextImpl(char *position, int distance = 1) const;
  virtual char *prevImpl(char *position, int distance = 1) const;
  virtual void release();
  virtual int expand(int n);

  virtual char *bufEnd();
  virtual const char *bufEnd() const;
  // if positive direction
  virtual bool positive() const;
  // positive direction
  virtual long pSize() const;
  virtual bool pContain(const char *position) const;
  // negative direction
  virtual long nLeftSize() const;
  virtual long nRightSize() const;
  virtual bool nLeftContain(const char *position) const;
  virtual bool nRightContain(const char *position) const;

  virtual int writeImpl(void *src, int n,
                        int (*writeHandler)(void *source, void *buffer, int n,
                                            int alreadyWrite));
  virtual int readImpl(void *src, int n,
                       int (*readHandler)(void *source, void *buffer, int n,
                                          int alreadyRead));

  char *buf_;
  char *head_;
  char *tail_;
  int capacity_;
};

} // namespace detail

class DynamicBuffer : public detail::CycleBuffer<1> {
public:
  DynamicBuffer(int capacity = 0);
  virtual ~DynamicBuffer();
  virtual Cowstr str() const;
};

class FixedBuffer : public detail::CycleBuffer<0> {
public:
  FixedBuffer(int capacity);
  virtual ~FixedBuffer();
  virtual Cowstr str() const;
};
