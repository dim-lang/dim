// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Random.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <numeric>
#include <sstream>

static const std::vector<std::pair<int, int>> AlphaRange = {{65, 91},
                                                            {97, 123}};
static const int AlphaLength = 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> DigitRange = {{48, 58}};
static const int DigitLength = 58 - 48;

static const std::vector<std::pair<int, int>> HexRange = {
    {48, 58}, {65, 71}, {97, 103}};
static const int HexLength = 58 - 48 + 71 - 65 + 103 - 97;

static const std::vector<std::pair<int, int>> AlnumRange = {
    {48, 58}, {65, 91}, {97, 123}};
static const int AlnumLength = 58 - 48 + 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> PunctRange = {
    {33, 48}, {58, 65}, {91, 97}, {123, 127}};
static const int PunctLength = 48 - 33 + 65 - 58 + 97 - 91 + 127 - 123;

static const std::vector<std::pair<int, int>> CtrlRange = {{0, 32}, {127, 128}};
static const int CtrlLength = 32 - 0 + 128 - 127;

static const std::vector<std::pair<int, int>> PrintRange = {{32, 127}};
static const int PrintLength = 127 - 32;

static const std::vector<std::pair<int, int>> SpaceRange = {{9, 14}, {32, 33}};
static const int SpaceLength = 14 - 9 + 33 - 32;

static const std::vector<std::pair<int, int>> AsciiRange = {{0, 128}};
static const int AsciiLength = 128 - 0;

RandomChar::RandomChar() : randint_() {}

static char nextCharImpl(const std::vector<std::pair<int, int>> &range, int n,
                         RandomInt<unsigned long> &randint) {
  int c = 0;
  int position = (int)(randint.next() % (unsigned long)n);
  for (int i = 0; i < (int)range.size(); i++) {
    int p = range[i].second - range[i].first;
    if (c + p > position) {
      int r = position - c + range[i].first;
      X_ASSERT(r >= range[0].first, "r {} >= range[0].first {}", r,
               range[0].first);
      X_ASSERT(r < range[range.size() - 1].second,
               "r {} < range[range.size()-1].second {}", r,
               range[range.size() - 1].second);
      return (char)r;
    }
    c += p;
  }
  X_ASSERT(false, "must not come here, position:{} c:{}", position, c);
  return (char)0;
}

char RandomChar::nextAlpha() {
  return nextCharImpl(AlphaRange, AlphaLength, randint_);
}

char RandomChar::nextAlnum() {
  return nextCharImpl(AlnumRange, AlnumLength, randint_);
}

char RandomChar::nextDigit() {
  return nextCharImpl(DigitRange, DigitLength, randint_);
}

char RandomChar::nextHex() {
  return nextCharImpl(HexRange, HexLength, randint_);
}

char RandomChar::nextPunct() {
  return nextCharImpl(PunctRange, PunctLength, randint_);
}

char RandomChar::nextPrint() {
  return nextCharImpl(PrintRange, PrintLength, randint_);
}

char RandomChar::nextSpace() {
  return nextCharImpl(SpaceRange, SpaceLength, randint_);
}

char RandomChar::nextCtrl() {
  return nextCharImpl(CtrlRange, CtrlLength, randint_);
}

char RandomChar::nextAscii() {
  return nextCharImpl(AsciiRange, AsciiLength, randint_);
}

char RandomChar::next(const char *candidates, int n) {
  int p = randint_.next() % n;
  return candidates[p];
}

char RandomChar::next(const std::string &candidates) {
  int p = randint_.next() % (int)candidates.length();
  return candidates[p];
}

RandomString::RandomString() : randchar_() {}

#define NEXT_STRING_IMPL(f, n)                                                 \
  do {                                                                         \
    std::stringstream ss;                                                      \
    for (int i = 0; i < n; i++) {                                              \
      ss << randchar_.f();                                                     \
    }                                                                          \
    return ss.str();                                                           \
  } while (0)

std::string RandomString::nextAlpha(int n) { NEXT_STRING_IMPL(nextAlpha, n); }

std::string RandomString::nextAlnum(int n) { NEXT_STRING_IMPL(nextAlnum, n); }

std::string RandomString::nextDigit(int n) { NEXT_STRING_IMPL(nextDigit, n); }

std::string RandomString::nextHex(int n) { NEXT_STRING_IMPL(nextHex, n); }

std::string RandomString::nextPunct(int n) { NEXT_STRING_IMPL(nextPunct, n); }

std::string RandomString::nextPrint(int n) { NEXT_STRING_IMPL(nextPrint, n); }

std::string RandomString::nextSpace(int n) { NEXT_STRING_IMPL(nextSpace, n); }

std::string RandomString::nextCtrl(int n) { NEXT_STRING_IMPL(nextCtrl, n); }

std::string RandomString::nextAscii(int n) { NEXT_STRING_IMPL(nextAscii, n); }

#undef NEXT_STRING_IMPL

std::string RandomString::next(const char *candidates, int c, int n) {
  std::stringstream ss;
  for (int i = 0; i < n; i++) {
    ss << randchar_.next(candidates, c);
  }
  return ss.str();
}

std::string RandomString::next(const std::string &candidates, int n) {
  return next(candidates.data(), candidates.length(), n);
}
