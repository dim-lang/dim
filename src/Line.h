// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include "Stringify.h"
#include "boost/noncopyable.hpp"
#include <memory>
#include <vector>

namespace fastype {

class Line : public Stringify {
public:
  Line();
  Line(int capacity, int lineNumber, int dirty);
  Line(const Line &other) = default;
  Line &operator=(const Line &other) = default;
  Line(Line &&other) = default;
  Line &operator=(Line &&other) = default;
  virtual ~Line() = default;

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
  void setLineNumber(int lineNumber);
  bool &dirty();
  void setDirty(int dirty);

  void expand(int n);

private:
  std::shared_ptr<Buffer> data_;
  int lineNumber_;
  bool dirty_;
};

} // namespace fastype
