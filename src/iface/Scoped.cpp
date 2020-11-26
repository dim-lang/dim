// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "iface/Scoped.h"

Scoped::Scoped(Scope *scope) : scoped_(scope) {}

Scope *&Scoped::scope() { return scoped_; }

Scope *Scoped::scope() const { return scoped_; }

void Scoped::resetScope() { scoped_ = nullptr; }
