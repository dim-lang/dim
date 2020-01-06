// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <memory>
#include <string>

class Term {
public:
  virtual ~Term() = default;

  virtual void show(const icu::UnicodeString &fileName) = 0;

  static std::shared_ptr<Term> open(const icu::UnicodeString &termName);
};
