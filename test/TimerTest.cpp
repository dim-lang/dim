// Copyright 2019- <rec-lang>
// Apache License Version 2.0

#include "Timer.h"
#include "catch2/catch.hpp"
#include <chrono>
#include <cstdio>
#include <thread>

#define TEST_UNIT 300

TEST_CASE("Timer", "[Timer]") {

  SECTION("Timer Start/Stop") {
    INFO("Timer Start/Stop");
    Timer t;
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.stop();
    REQUIRE(t.elapse() >= TEST_UNIT);
    REQUIRE(t.elapse() <= TEST_UNIT + 100);
  }

  SECTION("Timer Resume") {
    INFO("Timer Resume");
    Timer t;
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.stop();
    REQUIRE(t.elapse() >= 3 * TEST_UNIT);
    REQUIRE(t.elapse() <= 3 * TEST_UNIT + 100);
  }

  SECTION("Timer Always Stopped") {
    INFO("Timer Always Stopped");
    Timer t;
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.resume();
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.resume();
    t.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.resume();
    t.stop();
    REQUIRE(t.elapse() >= 0);
    REQUIRE(t.elapse() <= 100);
  }
}
