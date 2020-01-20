// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Clock.h"
#include "catch2/catch.hpp"
#include <chrono>
#include <cstdio>
#include <thread>

#define TEST_UNIT 300

TEST_CASE("Clock", "[Clock]") {

  SECTION("Clock Start/Stop") {
    INFO("Clock Start/Stop");
    Clock t;
    std::this_thread::sleep_for(std::chrono::milliseconds(TEST_UNIT));
    t.stop();
    REQUIRE(t.elapse() >= TEST_UNIT);
    REQUIRE(t.elapse() <= TEST_UNIT + 100);
  }

  SECTION("Clock Resume") {
    INFO("Clock Resume");
    Clock t;
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

  SECTION("Clock Always Stopped") {
    INFO("Clock Always Stopped");
    Clock t;
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
