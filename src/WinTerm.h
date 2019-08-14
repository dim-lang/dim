// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Term.h"
#include <memory>
#include <string>

namespace fastype {

class WinTerm : public Term {
public:
  WinTerm();
  virtual ~WinTerm();

  virtual void show(const std::string &fileName);
};

} // namespace fastype
