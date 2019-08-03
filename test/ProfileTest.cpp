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

  int x = 300;
  SECTION("Timer Start/Stop") {
    INFO("Timer Start/Stop");
    fastype::Timer t;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.stop();
    REQUIRE(t.elapse() >= x);
    REQUIRE(t.elapse() <= x + 100);
  }

  SECTION("Timer Resume") {
    INFO("Timer Resume");
    fastype::Timer t;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.stop();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.resume();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.stop();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.resume();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.stop();
    fprintf(stderr, "3-elapse: %d\n", t.elapse());
    REQUIRE(t.elapse() >= 3 * x);
    REQUIRE(t.elapse() <= 3 * x + 10);
  }

  SECTION("Timer Always Stopped") {
    INFO("Timer Always Stopped");
    fastype::Timer t;
    t.stop();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.resume();
    t.stop();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.resume();
    t.stop();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(x));
    t.resume();
    t.stop();
    fprintf(stderr, "4-elapse: %d\n", t.elapse());
    REQUIRE(t.elapse() >= 0);
    REQUIRE(t.elapse() <= 10);
  }
}
