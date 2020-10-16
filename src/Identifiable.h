// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class Identifiable {
public:
  Identifiable();
  virtual ~Identifiable() = default;
  virtual unsigned long long identifier() const;

protected:
  unsigned long long identifiable_;
};
