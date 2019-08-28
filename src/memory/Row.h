// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "memory/Buf.h"
#include <boost/core/noncopyable.hpp>
#include <memory>
#include <vector>

namespace fastype {

class Row : public Stringify {
public:
  Row();
  Row(int capacity, int lineNumber, int dirty);
  Row(const Row &other) = default;
  Row &operator=(const Row &other) = default;
  Row(Row &&other) = default;
  Row &operator=(Row &&other) = default;
  virtual ~Row() = default;

  virtual std::string toString() const;

  char *data();
  const char *data() const;
  char &operator[](int index);
  const char &operator[](int index) const;
  int size() const;
  void setSize(int size);
  int capacity() const;
  void setCapacity(int capacity);
  int &lineNumber();
  void setRowNumber(int lineNumber);
  bool &dirty();
  void setDirty(int dirty);

  void expand(int n);

private:
  std::shared_ptr<Buf> data_;
  int lineNumber_;
  bool dirty_;
};

} // namespace fastype
