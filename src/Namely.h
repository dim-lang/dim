// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Name.h"

class Namely {
public:
  Namely(const std::string &name = "");
  virtual ~Namely() = default;
  Name name;
};
