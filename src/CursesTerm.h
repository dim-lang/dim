// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Term.h"
#include <string>

namespace fastype {

class CursesTerm : public Term {
public:
  explicit CursesTerm();

  virtual ~CursesTerm();

  virtual void show(const icu::UnicodeString &fileName);

private:
  int row_;
  int col_;
};

} // namespace fastype
