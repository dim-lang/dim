// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Term.h"

namespace fastype {

class WinTerm : public Term {
public:
  WinTerm();
  virtual ~WinTerm();

  virtual void show(const icu::UnicodeString &fileName);
};

} // namespace fastype
