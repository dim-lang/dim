// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "location.hh"

class Locationly {
public:
  Locationly(const yy::location &a_location = yy::location());
  virtual ~Locationly() = default;

  yy::location location;
};
