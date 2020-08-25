// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Position.h"

class Positional {
public:
  Positional();
  Positional(const Position &position);
  virtual ~Positional() = default;
  virtual const Position &position() const;
  virtual void locate(const Position &position);

protected:
  Position position_;
};
