// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "infra/Cowstr.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"
#include <algorithm>
#include <deque>

#define C_MIN 0
#define C_MAX 100

TEST_CASE("Cowstr", "[Cowstr]") {
  SECTION("basic") {
    Cowstr s1;
    Cowstr s2("hello");
    Cowstr s3("hello", 4);
    Cowstr s4(std::string("hello"));
    REQUIRE(s1.length() == 0);
    REQUIRE(s1.str() == "");
    REQUIRE(s1.empty());
    REQUIRE(s2.length() == 5);
    REQUIRE(s2.str() == "hello");
    REQUIRE(!s2.empty());
    REQUIRE(s3.length() == 4);
    REQUIRE(s3.str() == "hell");
    REQUIRE(!s3.empty());
    REQUIRE(s4.length() == 5);
    REQUIRE(s4.str() == "hello");
    REQUIRE(!s4.empty());
    REQUIRE(s4.front() == 'h');
    REQUIRE(*s4.begin() == 'h');
    REQUIRE(*s4.rbegin() == 'o');
    REQUIRE(s4.back() == 'o');
    REQUIRE(*(s4.end() - 1) == 'o');
    REQUIRE(*(s4.rend() + 1) == 'h');
    REQUIRE(s1.compare(s2) != 0);
  }
  SECTION("modify") {
    Cowstr s1("hello");
    Cowstr s2("hello");
    Cowstr s3(s1);
    s1.insert(0, "world ");
    std::vector<Cowstr> inserter = {"w", "o", "r", "l", "d", " "};
    s2.insert(0, inserter.begin(), inserter.end());
    Cowstr s4(s1);
    REQUIRE(s1.str() == "world hello");
    REQUIRE(s1 == s2);
    REQUIRE((void *)s4.rawstr() == (void *)s1.rawstr());
    REQUIRE((void *)s3.rawstr() != (void *)s1.rawstr());

    s1.erase(5);
    REQUIRE(s1.str() == "worldhello");
    s1.erase(5);
    REQUIRE(s1.str() == "worldello");
    s1.erase(5);
    REQUIRE(s1.str() == "worldllo");
    s1.erase(5);
    REQUIRE(s1.str() == "worldlo");
    s1.erase(5);
    REQUIRE(s1.str() == "worldo");
    s1.erase(5);
    REQUIRE(s1.str() == "world");
    s1.popend();
    REQUIRE(s1.str() == "worl");
    s1.popend();
    REQUIRE(s1.str() == "wor");
    s1.popend();
    REQUIRE(s1.str() == "wo");
    s1.popend();
    REQUIRE(s1.str() == "w");
    s1.popend();
    REQUIRE(s1.str() == "");
    REQUIRE(s1.empty());

    s1.append("h");
    s1.append('e');
    s1.append('l', 2);
    s1.append('o');
    REQUIRE(s1.str() == "hello");
  }
}
