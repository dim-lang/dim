// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/unistr.h"

namespace fastype {

class Buffer : private boost::noncopyable, public Logging, Stringify {
public:
  Buffer();
  Buffer(Buffer &&other);
  Buffer &operator=(Buffer &&other);
  virtual ~Buffer();

  char *&data();
  void setData(char *data);
  int &size();
  void setSize(int size);
  int &capacity();
  void setCapacity(int capacity);
  int expand(int n);

  virtual std::string toString() const;

private:
  char *data_;
  int size_;
  int capacity_;
};

} // namespace fastype
