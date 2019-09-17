// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include <sstream>

namespace fastype {

std::random_device Random::device_;
std::mt19937 Random::engine_(device_());
std::uniform_int_distribution<long> Random::long_;
std::uniform_int_distribution<unsigned long> Random::ulong_;
std::uniform_int_distribution<long long> llong_;
std::uniform_int_distribution<unsigned long long> ullong_;
std::uniform_real_distribution<float> float_;
std::uniform_real_distribution<double> double_;

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
    int pos = Random::next<int>(c);
    ss << candidates[pos];
  }
  return ss.str();
}

} // namespace fastype
