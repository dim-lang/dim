// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "File.h"
#include "catch2/catch.hpp"
#include <numeric>

#define BLOCK_SIZE 16

TEST_CASE("File", "[File]") {
  const std::string text1 =
      "This is a long string for testing File::write and File::read "
      "static methods.\n"
      "Test cases will firstly write this unicode string into a temporary "
      "file named \"FileTest1.log\",\n"
      "then read text from this temporary file. Finally compare the read "
      "text with the original text.\n"
      "For convenience, our test cases do not compare substring, but only "
      "the whole text.\n";
  const std::string text2 =
      "This is a simple string for testing File::append and File::readline "
      "static methods.\n"
      "Test cases will firstly append to a temporary file named "
      "\"FileTest2.log\",\n"
      "then read text line by line from this temporary file. Finally compare "
      "all the lines text with the original text.\n";

  const std::string l1 = "Hello World\n";
  const std::string l2 = "What a greate morning! What a lovely weather!\n";
  const std::string l3 = "Let's go to KFC and have a good meal!\n";
  const std::string l4 = "I cannot wait anymore!\n";
  const std::string tmp = "Test a very long string in one line!";
  std::string l5;
  for (int i = 0; i < 256; i++) {
    l5 += tmp;
  }
  l5 += "\n";
  const std::string l6 = l1 + l2 + l3 + l4 + l5;

  SECTION("read/write") {
    const std::string f1 = "FileTest-rw1.log";
    const std::string f2 = "FileTest-rw2.log";

    if (boost::filesystem::exists(f1)) {
      boost::filesystem::remove(f1);
    }
    if (boost::filesystem::exists(f2)) {
      boost::filesystem::remove(f2);
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    std::string t1 = r1.read();
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    std::string t2 = r2.read();
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }
  SECTION("readline/write") {
    const std::string f1 = "FileTest-line1.log";
    const std::string f2 = "FileTest-line2.log";

    if (boost::filesystem::exists(f1)) {
      boost::filesystem::remove(f1);
    }
    if (boost::filesystem::exists(f2)) {
      boost::filesystem::remove(f2);
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    FileReader::LineIterator line1 = r1.lineIterator();
    std::string t1;
    while (line1.hasNext()) {
      t1 += line1.next();
    }
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    FileReader::LineIterator line2 = r2.lineIterator();
    std::string t2;
    while (line2.hasNext()) {
      t2 += line2.next();
    }
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }
  SECTION("readchar/write") {
    const std::string f1 = "FileTest-char1.log";
    const std::string f2 = "FileTest-char2.log";

    if (boost::filesystem::exists(f1)) {
      boost::filesystem::remove(f1);
    }
    if (boost::filesystem::exists(f2)) {
      boost::filesystem::remove(f2);
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    FileReader::CharIterator c1 = r1.charIterator();
    std::string t1;
    while (c1.hasNext()) {
      t1 += c1.next();
    }
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    FileReader::CharIterator c2 = r2.charIterator();
    std::string t2;
    while (c2.hasNext()) {
      t2 += c2.next();
    }
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }
  SECTION("readblock/write") {
    const std::string f1 = "FileTest-block1.log";
    const std::string f2 = "FileTest-block2.log";

    if (boost::filesystem::exists(f1)) {
      boost::filesystem::remove(f1);
    }
    if (boost::filesystem::exists(f2)) {
      boost::filesystem::remove(f2);
    }

    FileWriter w1(f1);
    w1.write(text1);
    w1.flush();
    FileReader r1(f1);
    FileReader::BlockIterator block1 = r1.blockIterator();
    std::string t1;
    while (block1.hasNext(BLOCK_SIZE)) {
      t1 += block1.next(BLOCK_SIZE);
    }
    t1 += block1.next(BLOCK_SIZE);
    REQUIRE(t1.length() == text1.length());
    REQUIRE(t1 == text1);

    FileWriter w2(f2);
    w2.write(text2);
    w2.flush();
    FileReader r2(f2);
    FileReader::BlockIterator block2 = r2.blockIterator();
    std::string t2;
    while (block2.hasNext(BLOCK_SIZE)) {
      t2 += block2.next(BLOCK_SIZE);
    }
    t2 += block2.next(BLOCK_SIZE);
    REQUIRE(t2.length() == text2.length());
    REQUIRE(t2 == text2);
  }

  SECTION("append") {
    const std::string f1 = "FileTest-append1.log";
    const std::string f2 = "FileTest-append2.log";

    if (boost::filesystem::exists(f1)) {
      boost::filesystem::remove(f1);
    }
    if (boost::filesystem::exists(f2)) {
      boost::filesystem::remove(f2);
    }

    FileAppender a1(f1);
    a1.write(text1);
    a1.flush();
    FileReader r1(f1);
    std::string t1 = r1.read();
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
    std::string t2 = r2.read();
    REQUIRE(t2.length() == l6.length());
    REQUIRE(t2 == l6);
  }
}
