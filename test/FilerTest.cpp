// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Filer.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Filer", "[Filer]") {
  SECTION("readAll CMakeLists.txt") {
    icu::UnicodeString text =
        fastype::Filer::readAll(UNICODE_STRING_SIMPLE("CMakeLists.txt"));
    REQUIRE(text.length() == 221);
    REQUIRE((int)text.tempSubString(0, 31).compare(
                UNICODE_STRING_SIMPLE("# Copyright 2019- <fastype.org>")) == 0);
    REQUIRE((int)text.tempSubString(195, 25).compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(example)")) == 0);
  }
  SECTION("readAll FilerTestBigFile.html") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/FilerTestBigFile.html"));
    REQUIRE(text.length() == 3969L);
    REQUIRE((int)text.tempSubString(0, 31).compare(
                UNICODE_STRING_SIMPLE("<!DOCTYPE html>")) == 0);
    REQUIRE((int)text.tempSubString(195, 25).compare(
                UNICODE_STRING_SIMPLE("</html>")) == 0);
  }
  SECTION("readLines CMakeLists.txt") {
    std::vector<icu::UnicodeString> texts =
        fastype::Filer::readLines(UNICODE_STRING_SIMPLE("CMakeLists.txt"));
    REQUIRE(texts.size() == 8);
    REQUIRE((int)texts[0].compare(UNICODE_STRING_SIMPLE(
                "# Copyright 2019- <fastype.org>\n")) == 0);
    REQUIRE((int)texts[1].compare(
                UNICODE_STRING_SIMPLE("# Apache License Version 2.0\n")) == 0);
    REQUIRE((int)texts[2].compare(UNICODE_STRING_SIMPLE("\n")) == 0);
    REQUIRE((int)texts[3].compare(UNICODE_STRING_SIMPLE(
                "cmake_minimum_required(VERSION 3.8)\n")) == 0);
    REQUIRE((int)texts[4].compare(UNICODE_STRING_SIMPLE(
                "project(fastype-parent VERSION 0.1.0 LANGUAGES CXX)\n")) == 0);
    REQUIRE((int)texts[5].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(src)\n")) == 0);
    REQUIRE((int)texts[6].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(test)\n")) == 0);
    REQUIRE((int)texts[7].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(example)\n")) == 0);
  }
  SECTION("readLines FilerTestBigFile.html") {
    std::vector<icu::UnicodeString> texts = fastype::Filer::readLines(
        UNICODE_STRING_SIMPLE("test/FilerTestBigFile.html"));
    REQUIRE(texts.size() == 8);
    REQUIRE((int)texts[0].compare(UNICODE_STRING_SIMPLE(
                "# Copyright 2019- <fastype.org>\n")) == 0);
    REQUIRE((int)texts[1].compare(
                UNICODE_STRING_SIMPLE("# Apache License Version 2.0\n")) == 0);
    REQUIRE((int)texts[2].compare(UNICODE_STRING_SIMPLE("\n")) == 0);
    REQUIRE((int)texts[3].compare(UNICODE_STRING_SIMPLE(
                "cmake_minimum_required(VERSION 3.8)\n")) == 0);
    REQUIRE((int)texts[4].compare(UNICODE_STRING_SIMPLE(
                "project(fastype-parent VERSION 0.1.0 LANGUAGES CXX)\n")) == 0);
    REQUIRE((int)texts[5].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(src)\n")) == 0);
    REQUIRE((int)texts[6].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(test)\n")) == 0);
    REQUIRE((int)texts[7].compare(
                UNICODE_STRING_SIMPLE("add_subdirectory(example)\n")) == 0);
  }
}
