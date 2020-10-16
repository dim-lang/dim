// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"

class Identifiable {
public:
  Identifiable();
  virtual ~Identifiable() = default;
  virtual unsigned long long identifier() const;
  virtual Cowstr hexIdentifier() const;
  virtual Cowstr decIdentifier() const;
  virtual Cowstr octIdentifier() const;
  virtual Cowstr binIdentifier() const;

protected:
  unsigned long long identifiable_;
};
