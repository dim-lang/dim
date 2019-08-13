// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Profile.h"
#include "catch2/catch.hpp"
#include <chrono>
#include <cstdio>
#include <thread>

TEST_CASE("Profile", "[Profile]") {
  int x = 300;

  SECTION("Timer Start/Stop") {
    INFO("Timer Start/Stop");
    fastype::Timer t;
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.stop();
    REQUIRE(t.elapse() >= x);
    REQUIRE(t.elapse() <= x + 100);
  }

  SECTION("Timer Resume") {
    INFO("Timer Resume");
    fastype::Timer t;
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.stop();
    REQUIRE(t.elapse() >= 3 * x);
    REQUIRE(t.elapse() <= 3 * x + 100);
  }

  SECTION("Timer Always Stopped") {
    INFO("Timer Always Stopped");
    fastype::Timer t;
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.resume();
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.resume();
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
    t.resume();
    t.stop();
    REQUIRE(t.elapse() >= 0);
    REQUIRE(t.elapse() <= 100);
  }
}
