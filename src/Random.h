// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include <cstdint>
#include <cstring>
#include <limits>
#include <random>
#include <string>

template <typename T> class RandomInt {
public:
  // [0, b]
  RandomInt(T b = std::numeric_limits<T>::max()) : RandomInt((T)0, b) {}
  // [a, b]
  RandomInt(T a, T b) : device_(), engine_(device_()), dist_(a, b) {
    CASSERT(b >= a, "b {} >= a {}", b, a);
  }
  virtual ~RandomInt() = default;
  T next() { return dist_(engine_); }
  T min() const { return dist_.min(); }
  T max() const { return dist_.max(); }

private:
  std::random_device device_;
  std::mt19937 engine_;
  std::uniform_int_distribution<T> dist_;
};

template <typename T> class RandomReal {
public:
  RandomReal(T b = std::numeric_limits<T>::max()) : RandomReal((T)0.0, b) {}
  RandomReal(T a, T b) : device_(), engine_(device_()), dist_(a, b) {
    CASSERT(b >= a, "b {} >= a {}", b, a);
  }
  virtual ~RandomReal() = default;
  T next() { return dist_(engine_); }
  T min() const { return dist_.min(); }
  T max() const { return dist_.max(); }

private:
  std::random_device device_;
  std::mt19937 engine_;
  std::uniform_real_distribution<T> dist_;
};

class RandomChar {
public:
  RandomChar();
  virtual ~RandomChar() = default;

  char nextAlpha();
  char nextAlnum();
  char nextDigit();
  char nextHex();
  char nextPunct();
  char nextPrint();
  char nextSpace();
  char nextCtrl();
  char nextAscii();
  char next(const char *candidates, int n);
  char next(const std::string &candidates);

private:
  RandomInt<unsigned long> randint_;
};

class RandomString {
public:
  RandomString();
  virtual ~RandomString() = default;

  std::string nextAlpha(int n = 1);
  std::string nextAlnum(int n = 1);
  std::string nextDigit(int n = 1);
  std::string nextHex(int n = 1);
  std::string nextPunct(int n = 1);
  std::string nextPrint(int n = 1);
  std::string nextSpace(int n = 1);
  std::string nextCtrl(int n = 1);
  std::string nextAscii(int n = 1);
  std::string next(const char *candidates, int c, int n = 1);
  std::string next(const std::string &candidates, int n = 1);

private:
  RandomChar randchar_;
};
