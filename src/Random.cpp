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
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

unsigned short Random::nextUShort(unsigned short right) {
  return nextUShort(0, right);
}

unsigned short Random::nextUShort(unsigned short left, unsigned short right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

int Random::nextInt(int right) { return nextInt(0, right); }

int Random::nextInt(int left, int right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

unsigned int Random::nextUInt(unsigned int right) { return nextUInt(0, right); }

unsigned int Random::nextUInt(unsigned int left, unsigned int right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((ulongDistribution()(engine()) % (right - left)) + left);
}

long Random::nextLong(long right) { return nextLong(0, right); }

long Random::nextLong(long left, long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((longDistribution()(engine()) % (right - left)) + left);
}

unsigned long Random::nextULong(unsigned long right) {
  return nextULong(0, right);
}

unsigned long Random::nextULong(unsigned long left, unsigned long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((ulongDistribution()(engine()) % (right - left)) + left);
}

long long Random::nextLLong(long long right) { return nextULong(0, right); }

long long Random::nextLLong(long long left, long long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((llongDistribution()(engine()) % (right - left)) + left);
}

unsigned long long Random::nextULLong(unsigned long long right) {
  return nextULLong(0, right);
}

unsigned long long Random::nextULLong(unsigned long long left,
                                      unsigned long long right) {
  F_CHECKF(right >= left, "right {} >= left {}", right, left);
  return left == right
             ? left
             : ((ullongDistribution()(engine()) % (right - left)) + left);
}

static const std::vector<std::pair<int, int>> Alpha = {{65, 91}, {97, 123}};
static const int AlphaLength = 91 - 65 + 123 - 97;

static const std::vector<std::pair<int, int>> Digit = {{48, 58}};
static const int DigitLength = 58 - 48;

static const std::vector<std::pair<int, int>> Hex = {
    {48, 58}, {65, 71}, {97, 103}};
static const int HexLength = 58 - 48 + 71 - 65 + 103 - 97;

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
  for (int i = 0; i < (int)range.size(); i++) {
    int p = range[i].second - range[i].first;
    if (n + p > pos) {
      int r = pos - n + range[i].first;
      F_CHECKF(r >= range[0].first, "r {} >= range[0].first {}", r,
               range[0].first);
      F_CHECKF(r < range[range.size() - 1].second,
               "r {} < range[range.size()-1].second {}", r,
               range[range.size() - 1].second);
      return (char)r;
    }
    n += p;
  }
  F_CHECKF(false, "must not come here, pos:{} rangeLength:{}", pos,
           rangeLength);
  return 0;
}

char Random::nextAlphaChar() { return nextAsciiChar(Alpha, AlphaLength); }

char Random::nextAlphaNumericChar() {
  char c = nextAsciiChar(AlphaNumeric, AlphaNumericLength);
  F_CHECKF(std::isalnum(c), "c {} isalnum", (int)c);
  return c;
}

char Random::nextDigitChar() {
  char c = nextAsciiChar(Digit, DigitLength);
  F_CHECKF(std::isdigit(c), "c {} isdigit", (int)c);
  return c;
}

char Random::nextHexChar() {
  char c = nextAsciiChar(Hex, HexLength);
  F_CHECKF(std::isxdigit(c), "c {} isxdigit", (int)c);
  return c;
}

char Random::nextPunctuationChar() {
  char c = nextAsciiChar(Punctuation, PunctuationLength);
  F_CHECKF(std::ispunct(c), "c {} ispunct", (int)c);
  return c;
}

char Random::nextControlChar() {
  char c = nextAsciiChar(Control, ControlLength);
  F_CHECKF(std::iscntrl(c), "c {} is cntrl", (int)c);
  return c;
}

char Random::nextPrintableChar() {
  char c = nextAsciiChar(Printable, PrintableLength);
  F_CHECKF(std::isprint(c), "c {} isprint", (int)c);
  return c;
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

std::string Random::nextHex(int len) {
  return nextAsciiString(Hex, HexLength, len);
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
