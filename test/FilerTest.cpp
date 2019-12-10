// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Filer.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Filer", "[Filer]") {
  SECTION("readAll/writeAll") {
    icu::UnicodeString text = UNICODE_STRING_SIMPLE(
        "This is a long string for testing Filer::writeAll and Filer::readAll "
        "static methods.\n"
        "Test cases will firstly write this unicode string into a temporary "
        "file named \"readwrite.log\",\n"
        "then read text from this temporary file. Finally compare the read "
        "text with the original text.\n"
        "For convenience, our test cases do not compare substring, but only "
        "the whole text.\n");
    int n =
        fastype::Filer::writeAll(UNICODE_STRING_SIMPLE("FilerTest1.log"), text);
    REQUIRE(n == text.length());
    icu::UnicodeString text2 =
        fastype::Filer::readAll(UNICODE_STRING_SIMPLE("FilerTest1.log"));
    REQUIRE(text.length() == text2.length());
    REQUIRE((int)text2.tempSubString(0, text.length()).compare(text) == 0);
  }
}
