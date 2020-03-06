// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include <cstdint>
#include <cstring>
#include <limits>
#include <random>
#include <string>

template <typename T, typename D = std::uniform_int_distribution<T>>
class RandomNumber {
public:
  // [0, b]
  RandomNumber(T b = std::numeric_limits<T>::max()) : RandomNumber((T)0, b) {}
  // [a, b]
  RandomNumber(T a, T b) : device_(), engine_(device_()), dist_(a, b) {
    CASSERT(b >= a, "b {} >= a {}", b, a);
  }
  virtual ~RandomNumber() = default;
  T next() { return dist_(engine_); }
  T min() const { return dist_.min(); }
  T max() const { return dist_.max(); }

private:
  std::random_device device_;
  std::mt19937 engine_;
  D dist_;
};

template <> class RandomNumber<float, std::uniform_real_distribution<float>> {
public:
  RandomNumber(float b = std::numeric_limits<float>::max())
      : RandomNumber((float)0.0, b) {}
  RandomNumber(float a, float b) : device_(), engine_(device_()), dist_(a, b) {
    CASSERT(b >= a, "b {} >= a {}", b, a);
  }
  virtual ~RandomNumber() = default;
  float next() { return dist_(engine_); }
  float min() const { return dist_.min(); }
  float max() const { return dist_.max(); }

private:
  std::random_device device_;
  std::mt19937 engine_;
  std::uniform_real_distribution<float> dist_;
};

template <> class RandomNumber<double, std::uniform_real_distribution<double>> {
public:
  RandomNumber(double b = std::numeric_limits<double>::max())
      : RandomNumber((double)0.0, b) {}
  RandomNumber(double a, double b)
      : device_(), engine_(device_()), dist_(a, b) {
    CASSERT(b >= a, "b {} >= a {}", b, a);
  }
  virtual ~RandomNumber() = default;
  double next() { return dist_(engine_); }
  double min() const { return dist_.min(); }
  double max() const { return dist_.max(); }

private:
  std::random_device device_;
  std::mt19937 engine_;
  std::uniform_real_distribution<double> dist_;
};

template <typename T> class RandomReal {
public:
  // [0, b)
  RandomReal(T b = (T)1.0) : RandomReal((T)0.0, b) {}
  // [a, b)
  RandomReal(T a, T b) : device_(), engine_(device_()), dist_(a, b) {
    CASSERT(b > a, "b {} > a {}", b, a);
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
  RandomNumber<unsigned long> randint_;
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
