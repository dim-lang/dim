// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include "infra/Counter.h"
#include "infra/Cowstr.h"
#include <algorithm>
#include <sstream>

class Nameable {
public:
  Nameable(const Cowstr &name = "");
  virtual ~Nameable() = default;
  virtual Cowstr &name();
  virtual const Cowstr &name() const;

protected:
  Cowstr nameable_;
};
