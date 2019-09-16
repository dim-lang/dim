// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cstring>
#include <utility>

#define HELLO_WORLD "hello world"
#define GOODBYE_WORLD "goodbye world"
#define TEST_MIN 16
#define TEST_MAX 1025
#define F_ALLOC_UNIT 8

TEST_CASE("Cowstr", "[Cowstr]") {

  SECTION("Empty Constructor") {
    fastype::Cowstr cs;
    REQUIRE(cs.capacity() == 0);
    REQUIRE(cs.empty());
    REQUIRE(cs.size() == 0);
    REQUIRE((void *)cs.head() == nullptr);
    REQUIRE((void *)cs.tail() == nullptr);
  }

  SECTION("Capacity Constructor") {
    for (int i = TEST_MIN; i < TEST_MAX; i++) {
      fastype::Cowstr cs(i);
      REQUIRE(cs.capacity() >= i);
      REQUIRE(cs.empty());
      REQUIRE(cs.size() == 0);
      REQUIRE((void *)cs.head() == nullptr);
      REQUIRE((void *)cs.tail() == nullptr);

      std::string payload = std::string(HELLO_WORLD) + std::to_string(i);
      cs = cs.concat(payload);
      REQUIRE(cs.capacity() >= payload.length());
      REQUIRE(!cs.empty());
      REQUIRE(cs.size() == payload.length());
      REQUIRE((void *)cs.head() != nullptr);
      REQUIRE((void *)cs.tail() == cs.head() + payload.length());
      REQUIRE(std::memcmp(cs.head(), payload.data(), cs.size()) == 0);
    }
  }

  SECTION("Concat") {
    for (int i = TEST_MIN; i < TEST_MAX; i++) {
      {
        fastype::Cowstr cs(i);
        REQUIRE(cs.capacity() >= i);
        REQUIRE(cs.empty());
        REQUIRE(cs.size() == 0);
        REQUIRE((void *)cs.head() == nullptr);
        REQUIRE((void *)cs.tail() == nullptr);

        std::string payload = std::string(HELLO_WORLD) + std::to_string(i);
        cs = cs.concat(payload);
        fastype::Cowstr cs1 = cs + payload;
        REQUIRE(cs.capacity() >= payload.length());
        REQUIRE(!cs.empty());
        REQUIRE(cs.size() == payload.length());
        REQUIRE((void *)cs.head() != nullptr);
        REQUIRE((void *)cs.tail() == cs.head() + payload.length());
        REQUIRE(std::memcmp(cs.head(), payload.data(), cs.size()) == 0);
        REQUIRE(cs1.capacity() >= payload.length());
        REQUIRE(!cs1.empty());
        REQUIRE(cs1.size() == payload.length());
        REQUIRE((void *)cs1.head() != nullptr);
        REQUIRE((void *)cs1.tail() == cs1.head() + payload.length());
        REQUIRE(std::memcmp(cs1.head(), payload.data(), cs1.size()) == 0);

        fastype::Cowstr ss1(i);
        ss1 += payload;
        REQUIRE(ss1.capacity() >= payload.length());
        REQUIRE(!ss1.empty());
        REQUIRE(ss1.size() == payload.length());
        REQUIRE((void *)ss1.head() != nullptr);
        REQUIRE((void *)ss1.tail() == ss1.head() + payload.length());
        REQUIRE(std::memcmp(ss1.head(), payload.data(), ss1.size()) == 0);

        cs = cs.concat(GOODBYE_WORLD, std::strlen(GOODBYE_WORLD));
        fastype::Cowstr cs2 = cs + std::string(GOODBYE_WORLD);
        REQUIRE(cs.capacity() >= std::strlen(GOODBYE_WORLD) + payload.length());
        REQUIRE(!cs.empty());
        REQUIRE(cs.size() == payload.length() + std::strlen(GOODBYE_WORLD));
        REQUIRE((void *)cs.head() != nullptr);
        REQUIRE((void *)cs.tail() ==
                cs.head() + payload.length() + std::strlen(GOODBYE_WORLD));
        std::string t0 = payload + GOODBYE_WORLD;
        REQUIRE(std::memcmp(cs.head(), t0.data(), t0.length()) == 0);

        REQUIRE(cs2.capacity() >=
                std::strlen(GOODBYE_WORLD) + payload.length());
        REQUIRE(!cs2.empty());
        REQUIRE(cs2.size() == payload.length() + std::strlen(GOODBYE_WORLD));
        REQUIRE((void *)cs2.head() != nullptr);
        REQUIRE((void *)cs2.tail() ==
                cs2.head() + payload.length() + std::strlen(GOODBYE_WORLD));
        std::string t1 = payload + GOODBYE_WORLD;
        REQUIRE(std::memcmp(cs2.head(), t1.data(), t1.length()) == 0);

        fastype::Cowstr ss2(i);
        ss2 += cs;
        ss2 += std::string(GOODBYE_WORLD);
        REQUIRE(ss2.capacity() >=
                std::strlen(GOODBYE_WORLD) + payload.length());
        REQUIRE(!ss2.empty());
        REQUIRE(ss2.size() == payload.length() + std::strlen(GOODBYE_WORLD));
        REQUIRE((void *)ss2.head() != nullptr);
        REQUIRE((void *)ss2.tail() ==
                ss2.head() + payload.length() + std::strlen(GOODBYE_WORLD));
        std::string t2 = payload + GOODBYE_WORLD;
        REQUIRE(std::memcmp(ss2.head(), t2.data(), t2.length()) == 0);
      }

      {
        fastype::Cowstr c1(HELLO_WORLD, std::strlen(HELLO_WORLD));
        fastype::Cowstr c2(GOODBYE_WORLD, std::strlen(GOODBYE_WORLD));
        fastype::Cowstr c3 = c1.concat(c2);
        REQUIRE(c3.capacity() >=
                std::strlen(HELLO_WORLD) + std::strlen(GOODBYE_WORLD));
        REQUIRE(c3.size() ==
                std::strlen(HELLO_WORLD) + std::strlen(GOODBYE_WORLD));
        REQUIRE(!c3.empty());
        REQUIRE((void *)c3.head() != nullptr);
        REQUIRE((void *)c3.tail() == c3.head() + std::strlen(HELLO_WORLD) +
                                         std::strlen(GOODBYE_WORLD));
        REQUIRE(std::memcmp(c3.head(),
                            (std::string(HELLO_WORLD) + GOODBYE_WORLD).data(),
                            std::strlen(HELLO_WORLD) +
                                std::strlen(GOODBYE_WORLD)) == 0);

        fastype::Cowstr c4 = c1 + c2;
        REQUIRE(c4.capacity() >=
                std::strlen(HELLO_WORLD) + std::strlen(GOODBYE_WORLD));
        REQUIRE(c4.size() ==
                std::strlen(HELLO_WORLD) + std::strlen(GOODBYE_WORLD));
        REQUIRE(!c4.empty());
        REQUIRE((void *)c4.head() != nullptr);
        REQUIRE((void *)c4.tail() == c4.head() + std::strlen(HELLO_WORLD) +
                                         std::strlen(GOODBYE_WORLD));
        REQUIRE(std::memcmp(c4.head(),
                            (std::string(HELLO_WORLD) + GOODBYE_WORLD).data(),
                            std::strlen(HELLO_WORLD) +
                                std::strlen(GOODBYE_WORLD)) == 0);

        fastype::Cowstr c5;
        c5 += c1;
        c5 += c2;
        REQUIRE(c5.capacity() >=
                std::strlen(HELLO_WORLD) + std::strlen(GOODBYE_WORLD));
        REQUIRE(c5.size() ==
                std::strlen(HELLO_WORLD) + std::strlen(GOODBYE_WORLD));
        REQUIRE(!c5.empty());
        REQUIRE((void *)c5.head() != nullptr);
        REQUIRE((void *)c5.tail() == c4.head() + std::strlen(HELLO_WORLD) +
                                         std::strlen(GOODBYE_WORLD));
        REQUIRE(std::memcmp(c5.head(),
                            (std::string(HELLO_WORLD) + GOODBYE_WORLD).data(),
                            std::strlen(HELLO_WORLD) +
                                std::strlen(GOODBYE_WORLD)) == 0);
      }
    }
  }
}
