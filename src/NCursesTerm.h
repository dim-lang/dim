// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "Term.h"
#include <memory>
#include <string>

namespace fastype {

class NCursesTerm : public Term {
public:
  explicit NCursesTerm();

  virtual ~NCursesTerm();

  virtual void show(const std::string &fileName);

private:
  std::shared_ptr<Logger> log_;
};

} // namespace fastype
