// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Profile.h"
#include "boost/chrono.hpp"
#include "boost/thread/thread.hpp"
#include "catch2/catch.hpp"
#include <cstdio>

TEST_CASE("Profile", "[Profile]") {
  SECTION("Profile Check") {
    INFO("Profile Check");
    F_CHECK(1 == 1, "1 == 1");
    int x = 2;
    F_CHECKF(x == 2, "{} == {}", x, 2);
  }

  SECTION("Timer") {
    INFO("Timer");
    fastype::Timer t;
    int x = 1000;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.stop();
    fprintf(stderr, "elapse: %d", t.elapse());
    REQUIRE(t.elapse() >= x);
    REQUIRE(t.elapse() <= x + 100);
  }
}
