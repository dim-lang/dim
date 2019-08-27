// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>

namespace fastype {

class Buf : public Stringify {
public:
  Buf();
  Buf(int capacity);
  virtual ~Buf();

  Buf(const Buf &other);
  Buf &operator=(const Buf &other);
  Buf(Buf &&other);
  Buf &operator=(Buf &&other);

  void expand(int capacity);
  bool empty() const;
  bool full() const;
  void clear();
  void release();
  void truncate(int start, int length);
  void trim(int length);
  void leftTrim(int length);
  void rightTrim(int length);

  char *data();
  const char *data() const;
  char &operator[](int index);
  const char &operator[](int index) const;
  int size() const;
  void setSize(int size);
  void incSize(int update);
  void decSize(int update);
  int capacity() const;

  virtual std::string toString() const;

private:
  char *data_;
  int size_;
  int capacity_;
};

} // namespace fastype
