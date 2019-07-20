// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "unicode/unistr.h"

namespace fastype {

class Line : public Logging, Stringify {
public:
  Line();
  Line(const icu::UnicodeString &data, int lineNumber, int dirty);
  Line(const Line &) = default;
  Line &operator=(const Line &) = default;
  Line(Line &&) = default;
  Line &operator=(Line &&) = default;
  virtual ~Line();

  virtual std::string toString() const;

  icu::UnicodeString &data();
  void setData(const icu::UnicodeString &data);
  int &lineNumber();
  void setLineNumber(int lineNumber);
  bool &dirty();
  void setDirty(int dirty);

private:
  icu::UnicodeString data_;
  int lineNumber_;
  bool dirty_;
};

} // namespace fastype
