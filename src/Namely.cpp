// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Namely.h"

Namely::Namely(const std::string &name) : name_(Name::get(name)) {}

Name &Namely::name() { return name_; }

const Name &Namely::name() const { return name_; }
