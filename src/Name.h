// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <memory>
#include <string>
#include <unordered_map>

class Name {
public:
  virtual ~Name() = default;

  virtual int compare(const Name &name) const;
  virtual bool operator==(const Name &name) const;
  virtual bool operator!=(const Name &name) const;
  virtual bool operator>(const Name &name) const;
  virtual bool operator>=(const Name &name) const;
  virtual bool operator<(const Name &name) const;
  virtual bool operator<=(const Name &name) const;

  virtual std::string raw() const;
  virtual unsigned long long id() const;

  // example:
  // a -> $8f1b.2
  // "" -> $0000.1
  virtual std::string toAsmName() const;

  // example:
  // a -> @a.2
  // "" -> @.1
  virtual std::string toSymbolName() const;
  virtual std::string toString() const;

  static Name get(const std::string &name);
  static const Name &noName();

protected:
  Name(const std::string &value);

  std::shared_ptr<std::string> value_;
  unsigned long long id_;

  static Counter counter_;
  static Name noName_;
  static std::unordered_map<std::string, Name> nameMap_;
};
