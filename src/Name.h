// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class Name {
public:
  virtual ~Name() = default;

  virtual int compare(const Name &other) const;
  virtual bool operator==(const Name &other) const;
  virtual bool operator!=(const Name &other) const;
  virtual bool operator>(const Name &other) const;
  virtual bool operator>=(const Name &other) const;
  virtual bool operator<(const Name &other) const;
  virtual bool operator<=(const Name &other) const;

  virtual const char *raw() const;
  virtual unsigned long long id() const;

  // example:
  // a -> $8f1b.2
  // "" -> $0000.1
  virtual std::string toLLVMName() const;

  // example:
  // a -> @a.2
  // "" -> @.1
  virtual std::string toSymbolName() const;
  virtual std::string toString() const;

  static Name get(const std::string &name);

protected:
  Name(const char *value);

  const char *value_;
  unsigned long long id_;
};

namespace std {

template <> struct hash<Name> {
  std::size_t operator()(const Name &o) const {
    return std::hash<std::string>()(o.raw());
  }
};

} // namespace std

class Nameable {
public:
  Nameable(const std::string &name = "");
  virtual ~Nameable() = default;
  virtual Name &name();
  virtual const Name &name() const;

private:
  Name name_;
};
