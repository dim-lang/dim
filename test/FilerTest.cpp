// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Filer.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cstring>
#include <functional>

TEST_CASE("Filer", "[Filer]") {
  SECTION("read/write") {
    icu::UnicodeString text1 = UNICODE_STRING_SIMPLE(
        "This is a long string for testing Filer::write and Filer::read "
        "static methods.\n"
        "Test cases will firstly write this unicode string into a temporary "
        "file named \"FilerTest1.log\",\n"
        "then read text from this temporary file. Finally compare the read "
        "text with the original text.\n"
        "For convenience, our test cases do not compare substring, but only "
        "the whole text.\n");
    int n1 = Filer::write(UNICODE_STRING_SIMPLE("FilerTest1.log"), text1);
    REQUIRE(n1 == text1.length());
    icu::UnicodeString result1 =
        Filer::read(UNICODE_STRING_SIMPLE("FilerTest1.log"));
    REQUIRE(text1.length() == result1.length());
    REQUIRE((int)result1.tempSubString(0, text1.length()).compare(text1) == 0);

    icu::UnicodeString text2 = UNICODE_STRING_SIMPLE(
        "This is a simple string for testing Filer::append and Filer::readline "
        "static methods.\n"
        "Test cases will firstly append to a temporary file named "
        "\"FilerTest2.log\",\n"
        "then read text line by line from this temporary file. Finally compare "
        "all the lines text with the original text.\n");
    int n2 = Filer::write(UNICODE_STRING_SIMPLE("FilerTest2.log"), text2);
    REQUIRE(n2 == text2.length());
    icu::UnicodeString result2 =
        Filer::read(UNICODE_STRING_SIMPLE("FilerTest2.log"));
    REQUIRE(text2.length() == result2.length());
    REQUIRE((int)result2.tempSubString(0, text2.length()).compare(text2) == 0);
  }

  SECTION("append/readline") {
    icu::UnicodeString l1 = UNICODE_STRING_SIMPLE("Hello World\n");
    icu::UnicodeString l2 = UNICODE_STRING_SIMPLE(
        "What a greate morning! What a lovely weather!\n");
    icu::UnicodeString l3 =
        UNICODE_STRING_SIMPLE("Let's go to KFC and have a good meal!\n");
    icu::UnicodeString l4 = UNICODE_STRING_SIMPLE("I cannot wait anymore!\n");
    int n1 = Filer::append(UNICODE_STRING_SIMPLE("FilerTest3.log"), l1);
    int n2 = Filer::append(UNICODE_STRING_SIMPLE("FilerTest3.log"), l2);
    int n3 = Filer::append(UNICODE_STRING_SIMPLE("FilerTest3.log"), l3);
    int n4 = Filer::append(UNICODE_STRING_SIMPLE("FilerTest3.log"), l4);
    REQUIRE(n1 == l1.length());
    REQUIRE(n2 == l2.length());
    REQUIRE(n3 == l3.length());
    REQUIRE(n4 == l4.length());

    std::vector<icu::UnicodeString> lines =
        Filer::readline(UNICODE_STRING_SIMPLE("FilerTest3.log"));
    REQUIRE(std::accumulate(lines.begin(), lines.end(), 0,
                            [](int a, const icu::UnicodeString &b) {
                              return a + (int)b.length();
                            }) ==
            l1.length() + l2.length() + l3.length() + l4.length());
    REQUIRE(lines[0] == l1);
    REQUIRE(lines[1] == l2);
    REQUIRE(lines[2] == l3);
    REQUIRE(lines[3] == l4);
  }
}
