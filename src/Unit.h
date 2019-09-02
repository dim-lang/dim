// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <string>

namespace fastype {

class Unit : public Stringify {
public:
  Unit();
  Unit(int capacity);
  virtual ~Unit();

  Unit(const Unit &other);
  Unit &operator=(const Unit &other);
  Unit(Unit &&other);
  Unit &operator=(Unit &&other);

  void expand(int capacity);
  // no data
  bool empty() const;
  // data used all allocated memory
  bool full() const;
  // clear data, but remain allocated memory
  void clear();
  // clear data and allocated memory
  void release();
  // remain [start, start+length) data
  void truncate(int start, int length);
  // delete left side and right side whitespaces, remain valid data
  void trim(int length);
  // delete left side whitespaces
  void leftTrim(int length);
  // delete right side whitespaces
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
