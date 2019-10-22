// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Term {
public:
  virtual ~Term() = default;

  virtual void show(const icu::UnicodeString &fileName) = 0;

  static Sptr<Term> open(const icu::UnicodeString &termName);
};

} // namespace fastype
