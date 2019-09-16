// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "catch2/catch.hpp"
#include <cstring>

#define HELLO_WORLD "hello world"
#define GOODBYE_WORLD "goodbye world"
#define TEST_N 16
#define TEST_M 1025

TEST_CASE("Cowstr", "[Cowstr]") {

  SECTION("Empty Constructor") {
    fastype::Cowstr cs;
    REQUIRE(cs.capacity() == 0);
    REQUIRE(cs.empty());
    REQUIRE(cs.size() == 0);
    REQUIRE(cs.head() == nullptr);
    REQUIRE(cs.tail() == nullptr);
  }

  SECTION("Capacity Constructor") {
    for (int i = TEST_N; i < TEST_M; i++) {
      fastype::Cowstr cs(i);
      REQUIRE(cs.capacity() == i);
      REQUIRE(cs.empty());
      REQUIRE(cs.size() == 0);
      REQUIRE(cs.head() == nullptr);
      REQUIRE(cs.tail() == nullptr);

      std::string payload = std::string(HELLO_WORLD) + std::to_string(i);
      cs.concat(payload);
      REQUIRE(cs.capacity() == i);
      REQUIRE(!cs.empty());
      REQUIRE(cs.size() == payload.length());
      REQUIRE(cs.head() != nullptr);
      REQUIRE(cs.tail() == cs.head() + payload.length());
      REQUIRE(std::memcmp(cs.head(), payload.data(), cs.size()) == 0);
    }
  }

  SECTION("Copy") {
    for (int i = TEST_N; i < TEST_M; i++) {
      fastype::Cowstr cs(i);
      REQUIRE(cs.capacity() == i);
      REQUIRE(cs.empty());
      REQUIRE(cs.size() == 0);
      REQUIRE(cs.head() == nullptr);
      REQUIRE(cs.tail() == nullptr);

      std::string payload = std::string(HELLO_WORLD) + std::to_string(i);
      cs.concat(payload);
      REQUIRE(cs.capacity() == i);
      REQUIRE(!cs.empty());
      REQUIRE(cs.size() == payload.length());
      REQUIRE(cs.head() != nullptr);
      REQUIRE(cs.tail() == cs.head() + payload.length());
      REQUIRE(std::memcmp(cs.head(), payload.data(), cs.size()) == 0);
    }
  }
}
