// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class Identifiable {
public:
  virtual ~Identifiable() = default;
  virtual unsigned long long identifier() const = 0;
};

class IdentifiableImpl : public Identifiable {
public:
  IdentifiableImpl();
  IdentifiableImpl(unsigned long long a_identifier);
  virtual ~IdentifiableImpl() = default;
  virtual unsigned long long identifier() const;

protected:
  unsigned long long identifiableImpl_;
};
