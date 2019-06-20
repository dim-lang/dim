// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <memory>

namespace fastype {

class Term {
public:
  virtual ~Term() = default;

  static std::shared_ptr<Term> open();

private:
  Term();
};

} // namespace fastype
