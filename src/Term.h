// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

namespace fastype {

class Term {
public:
  virtual ~Term() = default;

  static std::shared_ptr<Term> open();
};

} // namespace fastype
