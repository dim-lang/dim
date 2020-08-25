// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Positional.h"

Positional::Positional() : position_() {}

Positional::Positional(const Position &position) : position_(position) {}

const Position &Positional::position() const { return position_; }

void Positional::locate(const Position &position) {
  position_.update(position);
}
