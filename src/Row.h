// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Stringify.h"
#include <boost/core/noncopyable.hpp>
#include <memory>
#include <vector>

namespace fastype {

class Row : public Stringify {
public:
  Row();
  Row(const Cowstr &str, int lineNumber, int dirty);
  Row(const Row &other) = default;
  Row &operator=(const Row &other) = default;
  Row(Row &&other) = default;
  Row &operator=(Row &&other) = default;
  virtual ~Row() = default;

  virtual std::string toString() const;

  Cowstr &str();
  const Cowstr &str() const;
  const int &lineNumber() const;
  int &lineNumber();
  const bool &dirty() const;
  bool &dirty();

private:
  Cowstr str_;
  int lineNumber_;
  bool dirty_;
};

} // namespace fastype
