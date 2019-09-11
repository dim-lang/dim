// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <fmt/format.h>
#include <limits>
#include <string>
#include <vector>

namespace fastype {

/**
 * Block is a data buffer implemented via double-end queue
 */
class Block : public Stringify {
public:
  Block();
  Block(int capacity);
  virtual ~Block();

  /* copy */
  Block(const Block &s);
  Block(char c);
  Block(const char *s, int n);
  Block(const std::string &s);
  Block &operator=(const Block &s);

  /* move */
  Block(Block &&s);
  Block &operator=(Block &&s);

  Block &swap(Block &s);

  // expand memory size
  Block &expand(int capacity);
  Block &clear();
  Block &release();

  // concatenate s at tail
  Block &concat(const Block &s);
  Block &concat(const char *s, int n);
  Block &concat(const std::string &s);
  // same with concat, but modify and return itself
  Block &operator+=(const Block &s);
  Block &operator+=(const std::string &s);

  // concatenate s at head
  Block &concatHead(const Block &s);
  Block &concatHead(const char *s, int n);
  Block &concatHead(const std::string &s);

  // remain [start, ...) or [start, start+startn), remove other parts
  Block &truncate(int start);
  Block &truncate(int start, int n);

  // remove n from head
  Block &removeHead(int n);
  // remove n from tail
  Block &removeTail(int n);

  /* read api */

  // get stringify snapshot
  virtual std::string toString() const;

  // raw data pointer at head
  char *head();
  const char *head() const;

  // raw data pointer at tail
  char *tail();
  const char *tail() const;

  // indexing
  char &operator[](int pos);
  const char &operator[](int pos) const;
  char &at(int pos);
  const char &at(int pos) const;

  // test if cowstr has no data
  bool empty() const;

  // cowstr string length
  int size() const;

  // allocated memory size
  int capacity() const;

  // unused capacity
  int leftCapacity() const;

  /* comparation */
  bool operator==(const Block &s) const;
  bool operator!=(const Block &s) const;
  bool operator>(const Block &s) const;
  bool operator>=(const Block &s) const;
  bool operator<(const Block &s) const;
  bool operator<=(const Block &s) const;
  int compare(const Block &s) const;

private:
  bool memoryInOrder() const;
  int tailCapacity() const;
  int headCapacity() const;
  int tailDistance() const;
  int headDistance() const;

  char *buf_;
  int start_;
  int end_;
  int capacity_;
};

} // namespace fastype
