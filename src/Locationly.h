// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "location.hh"

class Locationly {
public:
  Locationly(const yy::location &location = yy::location());
  virtual ~Locationly() = default;
  virtual yy::location &location();
  virtual const yy::location &location() const;

  static std::string locationString(const yy::location &location);
  static std::string positionString(const yy::position &position);

private:
  yy::location location_;
};
