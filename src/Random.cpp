// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "Logging.h"
#include <sstream>

namespace fastype {

std::random_device Random::device_;
std::mt19937 Random::engine_(device_());
std::uniform_int_distribution<long> Random::long_;
std::uniform_int_distribution<unsigned long> Random::ulong_;
std::uniform_int_distribution<long long> Random::llong_;
std::uniform_int_distribution<unsigned long long> Random::ullong_;
std::uniform_real_distribution<float> Random::float_;
std::uniform_real_distribution<double> Random::double_;

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

float Random::nextFloat() { return float_(engine_); }

double Random::nextDouble() { return double_(engine_); }

std::string Random::nextAlpha(int limit) {
  const static char *alphas = {};
  return nextString(limit, alphas, sizeof(alphas) / sizeof(char));
}

std::string Random::nextAlphaNumeric(int limit) {
  const static char *alnums = {};
  return nextString(limit, alnums, sizeof(alnums) / sizeof(char));
}

std::string Random::nextDigit(int limit) {
  const static char *digits = {};
  return nextString(limit, digits, sizeof(digits) / sizeof(char));
}

std::string Random::nextString(int limit, const std::string &candidates) {
  return nextString(limit, candidates.data(), (int)candidates.length());
}

std::string Random::nextString(int limit, const char *candidates, int c) {
  std::stringstream ss;
  for (int i = 0; i < limit; i++) {
    int pos = nextInt(c);
    ss << candidates[pos];
  }
  return ss.str();
}

} // namespace fastype
