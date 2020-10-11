// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Location.h"
#include <string>

class Locationable {
public:
  virtual ~Locationable() = default;
  virtual Location &location() = 0;
  virtual const Location &location() const = 0;
};

class LocationableImpl : public Locationable {
public:
  LocationableImpl(const Location &location = Location());
  virtual ~LocationableImpl() = default;
  virtual Location &location();
  virtual const Location &location() const;

protected:
  Location locationableImpl_;
};
