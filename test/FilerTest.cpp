// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Filer.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Filer", "[Filer]") {
  SECTION("readAll") {
    icu::UnicodeString text =
        fastype::Filer::readAll(UNICODE_STRING_SIMPLE("CMakeLists.txt"));
    REQUIRE(text.length() == 1);
    REQUIRE(text.tempSubString(0, 30).compare(
                UNICODE_STRING_SIMPLE("# Copyright 2019- <fastype.org>")) == 0);
    REQUIRE(text.tempSubString(80, 104).compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(example)")) == 0);
  }
  SECTION("readLines") {
    std::vector<icu::UnicodeString> texts =
        fastype::Filer::readLines(UNICODE_STRING_SIMPLE("CMakeLists.txt"));
    REQUIRE(texts.size() == 8);
    REQUIRE(texts[0].compare(
                UNICODE_STRING_SIMPLE("# Copyright 2019- <fastype.org>")) == 0);
    REQUIRE(texts[1].compare(
                UNICODE_STRING_SIMPLE("# Apache License Version 2.0")) == 0);
    REQUIRE(texts[2].compare(UNICODE_STRING_SIMPLE(
                "cmake_minimum_required(VERSION 3.8)")) == 0);
    REQUIRE(texts[3].compare(UNICODE_STRING_SIMPLE(
                "project(fastype-parent VERSION 0.1.0 LANGUAGES CXX)")) == 0);
    REQUIRE(texts[4].compare(UNICODE_STRING_SIMPLE("add_subdirectory(src)")) ==
            0);
    REQUIRE(texts[5].compare(UNICODE_STRING_SIMPLE("add_subdirectory(test)")) ==
            0);
    REQUIRE(texts[6].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(example)")) == 0);
    REQUIRE(texts[7].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(example)")) == 0);
  }
}
