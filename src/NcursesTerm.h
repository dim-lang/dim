// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Term.h"
#include <string>

namespace fastype {

class NCursesTerm : public Term {
public:
  explicit NCursesTerm();

  virtual ~NCursesTerm();

  virtual void show(const std::string &fileName);

private:
  int row_;
  int col_;
};

} // namespace fastype
