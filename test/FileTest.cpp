// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "File.h"
#include "catch2/catch.hpp"
#include <numeric>

TEST_CASE("File", "[File]") {

  SECTION("read/write") {
    if (boost::filesystem::exists("FileTest1.log")) {
      boost::filesystem::remove("FileTest1.log");
    }
    if (boost::filesystem::exists("FileTest2.log")) {
      boost::filesystem::remove("FileTest2.log");
    }
    std::string text1 =
        "This is a long string for testing File::write and File::read "
        "static methods.\n"
        "Test cases will firstly write this unicode string into a temporary "
        "file named \"FileTest1.log\",\n"
        "then read text from this temporary file. Finally compare the read "
        "text with the original text.\n"
        "For convenience, our test cases do not compare substring, but only "
        "the whole text.\n";
    int n1 = File::write("FileTest1.log", text1);
    REQUIRE(n1 == text1.length());
    std::string result1 = File::read("FileTest1.log");
    REQUIRE(text1.length() == result1.length());
    REQUIRE(text1 == result1);

    std::string text2 =
        "This is a simple string for testing File::append and File::readline "
        "static methods.\n"
        "Test cases will firstly append to a temporary file named "
        "\"FileTest2.log\",\n"
        "then read text line by line from this temporary file. Finally compare "
        "all the lines text with the original text.\n";
    int n2 = File::write("FileTest2.log", text2);
    REQUIRE(n2 == text2.length());
    std::string result2 = File::read("FileTest2.log");
    REQUIRE(result2 == text2);
  }

  SECTION("append/readline") {
    if (boost::filesystem::exists("FileTest3.log")) {
      boost::filesystem::remove("FileTest3.log");
    }
    std::string l1 = "Hello World\n";
    std::string l2 = "What a greate morning! What a lovely weather!\n";
    std::string l3 = "Let's go to KFC and have a good meal!\n";
    std::string l4 = "I cannot wait anymore!\n";
    std::string l5, l6;
    l5 = l6 = "Test a very long string in one line!";
    for (int i = 0; i < 256; i++) {
      l5 += l6;
    }
    l5 += "\n";
    int n1 = File::append("FileTest3.log", l1);
    int n2 = File::append("FileTest3.log", l2);
    int n3 = File::append("FileTest3.log", l3);
    int n4 = File::append("FileTest3.log", l4);
    int n5 = File::append("FileTest3.log", l5);
    REQUIRE(n1 == l1.length());
    REQUIRE(n2 == l2.length());
    REQUIRE(n3 == l3.length());
    REQUIRE(n4 == l4.length());
    REQUIRE(n5 == l5.length());

    std::vector<std::string> lines = File::readline("FileTest3.log");
    REQUIRE(std::accumulate(lines.begin(), lines.end(), 0,
                            [](int a, const std::string &b) {
                              return a + (int)b.length();
                            }) == l1.length() + l2.length() + l3.length() +
                                      l4.length() + l5.length());
    REQUIRE(lines[0] == l1);
    REQUIRE(lines[1] == l2);
    REQUIRE(lines[2] == l3);
    REQUIRE(lines[3] == l4);
    REQUIRE(lines[4] == l5);
  }
}
