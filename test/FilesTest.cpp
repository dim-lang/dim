// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Files.h"
#include "catch2/catch.hpp"
#include <numeric>

#define BLOCK_SIZE 16

TEST_CASE("Files", "[Files]") {
  const Cowstr text1 =
      "This is a long string for testing File::write and File::read "
      "static methods.\n"
      "Test cases will firstly write this unicode string into a temporary "
      "file named \"FileTest1.log\",\n"
      "then read text from this temporary file. Finally compare the read "
      "text with the original text.\n"
      "For convenience, our test cases do not compare substring, but only "
      "the whole text.\n";
  const Cowstr text2 =
      "This is a simple string for testing File::append and File::readline "
      "static methods.\n"
      "Test cases will firstly append to a temporary file named "
      "\"FileTest2.log\",\n"
      "then read text line by line from this temporary file. Finally compare "
      "all the lines text with the original text.\n";

  const Cowstr l1 = "Hello World\n";
  const Cowstr l2 = "What a greate morning! What a lovely weather!\n";
  const Cowstr l3 = "Let's go to KFC and have a good meal!\n";
  const Cowstr l4 = "I cannot wait anymore!\n";
  const Cowstr tmp = "Test a very long string in one line!";
  Cowstr l5;
  for (int i = 0; i < 256; i++) {
    l5.append(tmp);
  }
  l5.append("\n");
  const Cowstr l6 = l1 + l2 + l3 + l4 + l5;

  SECTION("read/write") {
    const Cowstr f1 = "FileTest-rw1.log";
    const Cowstr f2 = "FileTest-rw2.log";

    if (boost::filesystem::exists(f1.str())) {
      boost::filesystem::remove(f1.str());
    }
    if (boost::filesystem::exists(f2.str())) {
      boost::filesystem::remove(f2.str());
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    Cowstr t1 = r1.readall();
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    Cowstr t2 = r2.readall();
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }
  SECTION("readline/write") {
    const Cowstr f1 = "FileTest-line1.log";
    const Cowstr f2 = "FileTest-line2.log";

    if (boost::filesystem::exists(f1.str())) {
      boost::filesystem::remove(f1.str());
    }
    if (boost::filesystem::exists(f2.str())) {
      boost::filesystem::remove(f2.str());
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    Cowstr t1, t11;
    while (!(t11 = r1.readln()).empty()) {
      t1.append(t11);
    }
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    Cowstr t2, t22;
    while (!(t22 = r2.readln()).empty()) {
      t2.append(t22);
    }
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }
  SECTION("readchar/write") {
    const Cowstr f1 = "FileTest-char1.log";
    const Cowstr f2 = "FileTest-char2.log";

    if (boost::filesystem::exists(f1.str())) {
      boost::filesystem::remove(f1.str());
    }
    if (boost::filesystem::exists(f2.str())) {
      boost::filesystem::remove(f2.str());
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    Cowstr t1, t11;
    while (!(t11 = r1.readc()).empty()) {
      t1 += t11;
    }
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    Cowstr t2, t22;
    while (!(t22 = r2.readc()).empty()) {
      t2 += t22;
    }
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }
  SECTION("readblock/write") {
    const Cowstr f1 = "FileTest-block1.log";
    const Cowstr f2 = "FileTest-block2.log";

    if (boost::filesystem::exists(f1.str())) {
      boost::filesystem::remove(f1.str());
    }
    if (boost::filesystem::exists(f2.str())) {
      boost::filesystem::remove(f2.str());
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    Cowstr t1, t11;
    while (!(t11 = r1.read(BLOCK_SIZE)).empty()) {
      t1 += t11;
    }
    t1 += r1.read(BLOCK_SIZE);
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    Cowstr t2, t22;
    while (!(t22 = r2.read(BLOCK_SIZE)).empty()) {
      t2 += t22;
    }
    t2 += r2.read(BLOCK_SIZE);
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }

  SECTION("append") {
    const Cowstr f1 = "FileTest-append1.log";
    const Cowstr f2 = "FileTest-append2.log";

    if (boost::filesystem::exists(f1.str())) {
      boost::filesystem::remove(f1.str());
    }
    if (boost::filesystem::exists(f2.str())) {
      boost::filesystem::remove(f2.str());
    }

    FileAppender a1(f1);
    a1.write(text1);
    a1.flush();
    FileReader r1(f1);
    Cowstr t1 = r1.readall();
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileAppender a2(f2);
    a2.write(l1);
    a2.write(l2);
    a2.write(l3);
    a2.write(l4);
    a2.write(l5);
    a2.flush();
    FileReader r2(f2);
    Cowstr t2 = r2.readall();
    REQUIRE(t2.length() == l6.length());
    REQUIRE(t2 == l6);
  }
}
