// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "Location.h"

class Locationable {
public:
  Locationable(const Location &location = Location());
  virtual ~Locationable() = default;
  virtual Location &location();
  virtual const Location &location() const;

protected:
  Location locationable_;
};
