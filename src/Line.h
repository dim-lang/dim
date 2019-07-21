// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "unicode/unistr.h"
#include <vector>

namespace fastype {

class Line : public Logging, Stringify {
public:
  Line();
  Line(const UChar *src, int start, int length, int lineNumber, int dirty);
  Line(const Line &other) = default;
  Line &operator=(const Line &other) = default;
  Line(Line &&other) = default;
  Line &operator=(Line &&other) = default;
  virtual ~Line() = default;

  virtual std::string toString() const;

  UChar *data();
  int size() const;
  void setSize(int size);
  int capacity() const;
  void setCapacity(int size);
  int &lineNumber();
  void setLineNumber(int lineNumber);
  bool &dirty();
  void setDirty(int dirty);

  int expand(int n);

private:
  std::vector<UChar> data_;
  int lineNumber_;
  bool dirty_;
};

} // namespace fastype
