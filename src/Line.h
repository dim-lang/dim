// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>
#include <vector>

namespace fastype {

class Line : public Stringify {
public:
  Line();
  Line(const icu::UnicodeString &value, int lineNumber, int dirty);
  Line(const Line &other) = default;
  Line &operator=(const Line &other) = default;
  Line(Line &&other) = default;
  Line &operator=(Line &&other) = default;
  virtual ~Line() = default;

  virtual std::string toString() const;

  icu::UnicodeString &value();
  const icu::UnicodeString &value() const;
  const int &lineNumber() const;
  int &lineNumber();
  const bool &dirty() const;
  bool &dirty();

private:
  icu::UnicodeString value_;
  int lineNumber_;
  bool dirty_;
};

} // namespace fastype
