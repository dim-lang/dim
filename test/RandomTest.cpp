// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "catch2/catch.hpp"
#include <algorithm>

#define TEST_MAX 128

TEST_CASE("Random", "[Random]") {
  SECTION("random int") {
    INFO("random int");
    for (int i = 0; i < TEST_MAX; i++) {
      int a = Random::next<int>();
      int b = Random::next<int>(i);
      int c = Random::next<int>(std::min(a, b), std::max(a, b));
    }
  }

  SECTION("random unsigned int") {
    INFO("random unsigned int");
    for (int i = 0; i < TEST_MAX; i++) {
      unsigned int a = Random::next<unsigned int>();
      unsigned int b = Random::next<unsigned int>(i);
      unsigned int c =
          Random::next<unsigned int>(std::min(a, b), std::max(a, b));
    }
  }

  SECTION("random long") {
    INFO("random long");
    for (int i = 0; i < TEST_MAX; i++) {
      long a = Random::next<long>();
      long b = Random::next<long>(i);
      long c = Random::next<long>(std::min(a, b), std::max(a, b));
    }
  }
}
