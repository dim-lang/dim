// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "container/LinkedHashMap.h"
#include "Log.h"
#include "catch2/catch.hpp"
#include <string>

TEST_CASE("container/LinkedHashMap", "[container/LinkedHashMap]") {
  SECTION("constructor") {
    LinkedHashMap<char, unsigned char> mchar1;
    LinkedHashMap<unsigned char, char> mchar2;
    LinkedHashMap<short, unsigned short> mshort1;
    LinkedHashMap<unsigned short, short> mshort2;
    LinkedHashMap<int, unsigned int> mint1;
    LinkedHashMap<unsigned int, int> mint2;
    LinkedHashMap<long, unsigned long> mlong1;
    LinkedHashMap<unsigned long, long> mlong2;
    LinkedHashMap<long long, unsigned long long> mllong1;
    LinkedHashMap<unsigned long long, long long> mllong2;
    LinkedHashMap<std::string, int> mstring1;

    REQUIRE(mchar1.size() == 0);
    REQUIRE(mchar2.size() == 0);
    REQUIRE(mshort1.size() == 0);
    REQUIRE(mshort2.size() == 0);
    REQUIRE(mint1.size() == 0);
    REQUIRE(mint2.size() == 0);
    REQUIRE(mlong1.size() == 0);
    REQUIRE(mlong2.size() == 0);
    REQUIRE(mllong1.size() == 0);
    REQUIRE(mllong2.size() == 0);
    REQUIRE(mstring1.size() == 0);
  }
  SECTION("destructor") {}
}
