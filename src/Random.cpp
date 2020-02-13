// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Random.h"
#include "Log.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <numeric>
#include <sstream>

std::random_device *Random::device_ = nullptr;
std::mt19937 *Random::engine_ = nullptr;
std::uniform_int_distribution<long> *Random::longDist_ = nullptr;
std::uniform_int_distribution<unsigned long> *Random::ulongDist_ = nullptr;
std::uniform_int_distribution<long long> *Random::llongDist_ = nullptr;
std::uniform_int_distribution<unsigned long long> *Random::ullongDist_ =
    nullptr;

void Random::initialize() {
  if (device_)
    delete device_;
  if (engine_)
    delete engine_;
  if (longDist_)
    delete longDist_;
  if (ulongDist_)
    delete ulongDist_;
  if (llongDist_)
    delete llongDist_;
  if (ullongDist_)
    delete ullongDist_;
  device_ = new std::random_device();
  engine_ = new std::mt19937((*device_)());
  longDist_ = new std::uniform_int_distribution<long>();
  ulongDist_ = new std::uniform_int_distribution<unsigned long>();
  llongDist_ = new std::uniform_int_distribution<long long>();
  ullongDist_ = new std::uniform_int_distribution<unsigned long long>();
}

std::random_device &Random::device() { return *device_; }

std::mt19937 &Random::engine() { return *engine_; }

std::uniform_int_distribution<long> &Random::longDistribution() {
  return *longDist_;
}

std::uniform_int_distribution<unsigned long> &Random::ulongDistribution() {
  return *ulongDist_;
}

std::uniform_int_distribution<long long> &Random::llongDistribution() {
  return *llongDist_;
}

std::uniform_int_distribution<unsigned long long> &
Random::ullongDistribution() {
  return *ullongDist_;
}

short Random::nextShort(short right) { return nextShort(0, right); }

short Random::nextShort(short left, short right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

unsigned short Random::nextUShort(unsigned short right) {
  return nextUShort(0, right);
}

unsigned short Random::nextUShort(unsigned short left, unsigned short right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

int Random::nextInt(int right) { return nextInt(0, right); }

int Random::nextInt(int left, int right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

unsigned int Random::nextUInt(unsigned int right) { return nextUInt(0, right); }

unsigned int Random::nextUInt(unsigned int left, unsigned int right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((ulongDistribution()(engine()) % (right - left)) + left);
}

long Random::nextLong(long right) { return nextLong(0, right); }

long Random::nextLong(long left, long right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

unsigned long Random::nextULong(unsigned long right) {
  return nextULong(0, right);
}

unsigned long Random::nextULong(unsigned long left, unsigned long right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((ulongDistribution()(engine()) % (right - left)) + left);
}

long long Random::nextLLong(long long right) { return nextULong(0, right); }

long long Random::nextLLong(long long left, long long right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((llongDistribution()(engine()) % (right - left)) + left);
}

unsigned long long Random::nextULLong(unsigned long long right) {
  return nextULLong(0, right);
}

unsigned long long Random::nextULLong(unsigned long long left,
                                      unsigned long long right) {
  CASSERT(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((ullongDistribution()(engine()) % (right - left)) + left);
}

static const std::vector<std::pair<int, int>> AlphaRange = {{65, 91},
                                                            {97, 123}};
static const int AlphaLength = 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> DigitRange = {{48, 58}};
static const int DigitLength = 58 - 48;

static const std::vector<std::pair<int, int>> HexRange = {
    {48, 58}, {65, 71}, {97, 103}};
static const int HexLength = 58 - 48 + 71 - 65 + 103 - 97;

static const std::vector<std::pair<int, int>> AlphaNumericRange = {
    {48, 58}, {65, 91}, {97, 123}};
static const int AlphaNumericLength = 58 - 48 + 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> PunctuationRange = {
    {33, 48}, {58, 65}, {91, 97}, {123, 127}};
static const int PunctuationLength = 48 - 33 + 65 - 58 + 97 - 91 + 127 - 123;

static const std::vector<std::pair<int, int>> ControlRange = {{0, 32},
                                                              {127, 128}};
static const int ControlLength = 32 - 0 + 128 - 127;

static const std::vector<std::pair<int, int>> PrintableRange = {{32, 127}};
static const int PrintableLength = 127 - 32;

static const std::vector<std::pair<int, int>> WhitespaceRange = {{9, 14},
                                                                 {32, 33}};
static const int WhitespaceLength = 14 - 9 + 33 - 32;

static const std::vector<std::pair<int, int>> AsciiRange = {{0, 128}};
static const int AsciiLength = 128 - 0;

icu::UnicodeString
Random::nextStringImpl(const std::vector<std::pair<int, int>> &range, int n,
                       int len) {
  icu::UnicodeString buffer;
  for (int i = 0; i < len; i++) {
    buffer.append(nextCharImpl(range, n));
  }
  return buffer;
}

UChar Random::nextCharImpl(const std::vector<std::pair<int, int>> &range,
                           int n) {
  int c = 0;
  int pos = nextInt(n);
  for (int i = 0; i < (int)range.size(); i++) {
    int p = range[i].second - range[i].first;
    if (c + p > pos) {
      int r = pos - c + range[i].first;
      CASSERT(r >= range[0].first, "r {} >= range[0].first {}", r,
              range[0].first);
      CASSERT(r < range[range.size() - 1].second,
              "r {} < range[range.size()-1].second {}", r,
              range[range.size() - 1].second);
      return (UChar)r;
    }
    c += p;
  }
  CASSERT(false, "must not come here, pos:{} c:{}", pos, c);
  return (UChar)0;
}

UChar Random::nextAlphaChar() { return nextCharImpl(AlphaRange, AlphaLength); }

UChar Random::nextAlphaNumericChar() {
  UChar c = nextCharImpl(AlphaNumericRange, AlphaNumericLength);
  CASSERT(std::isalnum(c), "c {} isalnum", (int)c);
  return c;
}

UChar Random::nextDigitChar() {
  UChar c = nextCharImpl(DigitRange, DigitLength);
  CASSERT(std::isdigit(c), "c {} isdigit", (int)c);
  return c;
}

UChar Random::nextHexChar() {
  UChar c = nextCharImpl(HexRange, HexLength);
  CASSERT(std::isxdigit(c), "c {} isxdigit", (int)c);
  return c;
}

UChar Random::nextPunctuationChar() {
  UChar c = nextCharImpl(PunctuationRange, PunctuationLength);
  CASSERT(std::ispunct(c), "c {} ispunct", (int)c);
  return c;
}

UChar Random::nextControlChar() {
  UChar c = nextCharImpl(ControlRange, ControlLength);
  CASSERT(std::iscntrl(c), "c {} is cntrl", (int)c);
  return c;
}

UChar Random::nextPrintableChar() {
  UChar c = nextCharImpl(PrintableRange, PrintableLength);
  CASSERT(std::isprint(c), "c {} isprint", (int)c);
  return c;
}

UChar Random::nextWhitespaceChar() {
  UChar c = nextCharImpl(WhitespaceRange, WhitespaceLength);
  CASSERT(std::isspace(c), "c {} isspace", (int)c);
  return c;
}

UChar Random::nextAsciiChar() { return nextCharImpl(AsciiRange, AsciiLength); }

UChar Random::nextChar(const icu::UnicodeString &candidates) {
  return nextChar(candidates.getBuffer(), candidates.length());
}

UChar Random::nextChar(const UChar *candidates, int c) {
  int pos = nextInt(c);
  return candidates[pos];
}

icu::UnicodeString Random::nextAlpha(int len) {
  return nextStringImpl(AlphaRange, AlphaLength, len);
}

icu::UnicodeString Random::nextAlphaNumeric(int len) {
  return nextStringImpl(AlphaNumericRange, AlphaNumericLength, len);
}

icu::UnicodeString Random::nextDigit(int len) {
  return nextStringImpl(DigitRange, DigitLength, len);
}

icu::UnicodeString Random::nextHex(int len) {
  return nextStringImpl(HexRange, HexLength, len);
}

icu::UnicodeString Random::nextPunctuation(int len) {
  return nextStringImpl(PunctuationRange, PunctuationLength, len);
}

icu::UnicodeString Random::nextControl(int len) {
  return nextStringImpl(ControlRange, ControlLength, len);
}

icu::UnicodeString Random::nextPrintable(int len) {
  return nextStringImpl(PrintableRange, PrintableLength, len);
}

icu::UnicodeString Random::nextWhitespace(int len) {
  return nextStringImpl(WhitespaceRange, WhitespaceLength, len);
}

icu::UnicodeString Random::nextAscii(int len) {
  return nextStringImpl(AsciiRange, AsciiLength, len);
}

icu::UnicodeString Random::nextString(const icu::UnicodeString &candidates,
                                      int len) {
  return nextString(candidates.getBuffer(), candidates.length(), len);
}

icu::UnicodeString Random::nextString(const UChar *candidates, int c, int len) {
  icu::UnicodeString buffer;
  for (int i = 0; i < len; i++) {
    int pos = nextInt(c);
    buffer.append(candidates[pos]);
  }
  return buffer;
}
