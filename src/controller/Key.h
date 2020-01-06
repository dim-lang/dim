// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <string>

#define FKEY(k)

enum KeyEnum {};

class Key {
public:
  Key(const std::string &s);
  Key(const KeyEnum &e);
  Key(int v);
  virtual ~Key() = default;

  const std::string &getName() const;
  int getValue();
  KeyEnum getEnum() const;

  bool isCtrl() const;
  bool isAlt() const;
  bool isShift() const;
  bool isEsc() const;
  bool isF() const;
  bool isInsert() const;
  bool isDelete() const;
  bool isHome() const;
  bool isEnd() const;
  bool isPgUp() const;
  bool isPgDn() const;
  bool isBackSpace() const;

private:
  int value;
};
