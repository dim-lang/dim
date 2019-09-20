// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "Logging.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <numeric>
#include <sstream>

namespace fastype {

std::random_device Random::device_;
std::mt19937 Random::engine_(device_());
std::uniform_int_distribution<long> Random::long_;
std::uniform_int_distribution<unsigned long> Random::ulong_;
std::uniform_int_distribution<long long> Random::llong_;
std::uniform_int_distribution<unsigned long long> Random::ullong_;

short Random::nextShort(short right) { return nextShort(0, right); }

short Random::nextShort(short left, short right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((long_(engine_) % (right - left)) + left);
}

unsigned short Random::nextUShort(unsigned short right) {
  return nextUShort(0, right);
}

unsigned short Random::nextUShort(unsigned short left, unsigned short right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((long_(engine_) % (right - left)) + left);
}

int Random::nextInt(int right) { return nextInt(0, right); }

int Random::nextInt(int left, int right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((long_(engine_) % (right - left)) + left);
}

unsigned int Random::nextUInt(unsigned int right) { return nextUInt(0, right); }

unsigned int Random::nextUInt(unsigned int left, unsigned int right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((ulong_(engine_) % (right - left)) + left);
}

long Random::nextLong(long right) { return nextLong(0, right); }

long Random::nextLong(long left, long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((long_(engine_) % (right - left)) + left);
}

unsigned long Random::nextULong(unsigned long right) {
  return nextULong(0, right);
}

unsigned long Random::nextULong(unsigned long left, unsigned long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((ulong_(engine_) % (right - left)) + left);
}

long long Random::nextLLong(long long right) { return nextULong(0, right); }

long long Random::nextLLong(long long left, long long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((llong_(engine_) % (right - left)) + left);
}

unsigned long long Random::nextULLong(unsigned long long right) {
  return nextULLong(0, right);
}

unsigned long long Random::nextULLong(unsigned long long left,
                                      unsigned long long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right ? left : ((ullong_(engine_) % (right - left)) + left);
}

static const std::vector<std::pair<int, int>> Alpha = {{65, 91}, {97, 123}};
static const int AlphaLength = 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> Digit = {{48, 58}};
static const int DigitLength = 58 - 48;

static const std::vector<std::pair<int, int>> AlphaNumeric = {
    {48, 58}, {65, 91}, {97, 123}};
static const int AlphaNumericLength = 58 - 48 + 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> Punctuation = {
    {33, 48}, {58, 65}, {91, 97}, {123, 127}};
static const int PunctuationLength = 48 - 33 + 65 - 58 + 97 - 91 + 127 - 123;

static const std::vector<std::pair<int, int>> Control = {{0, 32}, {127, 128}};
static const int ControlLength = 32 - 0 + 128 - 127;

static const std::vector<std::pair<int, int>> Printable = {{32, 127}};
static const int PrintableLength = 127 - 32;

static const std::vector<std::pair<int, int>> Ascii = {{0, 128}};
static const int AsciiLength = 128 - 0;

std::string
Random::nextAsciiString(const std::vector<std::pair<int, int>> &range,
                        int rangeLength, int len) {
  std::stringstream ss;
  for (int i = 0; i < len; i++) {
    ss << nextAsciiChar(range, rangeLength);
  }
  return ss.str();
}

char Random::nextAsciiChar(const std::vector<std::pair<int, int>> &range,
                           int rangeLength) {
  int n = 0;
  int pos = nextInt(rangeLength);
  for (int i = 0; i < range.size(); i++) {
    int p = range[i].second - range[i].first;
    if (n + p > pos) {
      return (char)(pos - n + range[i].first);
    }
  }
  F_CHECKF(false, "must not come here, pos:{} rangeLength:{}", pos,
           rangeLength);
  return 0;
}

char Random::nextAlphaChar() { return nextAsciiChar(Alpha, AlphaLength); }

char Random::nextAlphaNumericChar() {
  return nextAsciiChar(AlphaNumeric, AlphaNumericLength);
}

char Random::nextDigitChar() { return nextAsciiChar(Digit, DigitLength); }

char Random::nextPunctuationChar() {
  return nextAsciiChar(Punctuation, PunctuationLength);
}

char Random::nextControlChar() { return nextAsciiChar(Control, ControlLength); }

char Random::nextPrintableChar() {
  return nextAsciiChar(Printable, PrintableLength);
}

char Random::nextAsciiChar() { return nextAsciiChar(Ascii, AsciiLength); }

char Random::nextChar(const std::string &candidates) {
  return nextChar(candidates.data(), candidates.length());
}

char Random::nextChar(const char *candidates, int c) {
  int pos = nextInt(c);
  return candidates[pos];
}

std::string Random::nextAlpha(int len) {
  return nextAsciiString(Alpha, AlphaLength, len);
}

std::string Random::nextAlphaNumeric(int len) {
  return nextAsciiString(AlphaNumeric, AlphaNumericLength, len);
}

std::string Random::nextDigit(int len) {
  return nextAsciiString(Digit, DigitLength, len);
}

std::string Random::nextPunctuation(int len) {
  return nextAsciiString(Punctuation, PunctuationLength, len);
}

std::string Random::nextControl(int len) {
  return nextAsciiString(Control, ControlLength, len);
}

std::string Random::nextPrintable(int len) {
  return nextAsciiString(Printable, PrintableLength, len);
}

std::string Random::nextAscii(int len) {
  return nextAsciiString(Ascii, AsciiLength, len);
}

std::string Random::nextString(const std::string &candidates, int len) {
  return nextString(candidates.data(), candidates.length(), len);
}

std::string Random::nextString(const char *candidates, int c, int len) {
  std::stringstream ss;
  for (int i = 0; i < len; i++) {
    int pos = nextInt(c);
    ss << candidates[pos];
  }
  return ss.str();
}

} // namespace fastype
