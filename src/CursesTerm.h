// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Term.h"

class CursesTerm : public Term {
public:
  explicit CursesTerm();

  virtual ~CursesTerm();

  virtual void show(const icu::UnicodeString &fileName);

private:
  int row_;
  int col_;
};
