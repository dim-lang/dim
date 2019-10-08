// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Block.h"
#include "Logging.h"
#include "Random.h"
#include "catch2/catch.hpp"
#include <cstring>

#define TEST_MAX 128

TEST_CASE("Block", "[Block]") {
  fastype::Random::initialize();

  SECTION("Empty Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      fastype::Block b;
      REQUIRE(b.capacity() == 0);
      REQUIRE(b.empty());
      REQUIRE(b.size() == 0);
      REQUIRE(b.tail() == b.head());
    }
  }

  SECTION("Capacity Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      int r = fastype::Random::nextInt(TEST_MAX);
      fastype::Block b(r);
      REQUIRE(b.capacity() >= r);
      REQUIRE(b.empty());
      REQUIRE(b.size() == 0);
      REQUIRE(b.tail() == b.head());
    }
  }

  SECTION("Copy Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      int r = fastype::Random::nextInt(TEST_MAX);
      fastype::Block b1(r);
      REQUIRE(b1.capacity() >= r);
      REQUIRE(b1.empty());
      REQUIRE(b1.size() == 0);
      REQUIRE(b1.tail() == b1.head());

      fastype::Block b2(b1);
      REQUIRE(b2.capacity() >= 0);
      REQUIRE(b2.empty());
      REQUIRE(b2.size() == 0);
      REQUIRE(b2.tail() == b2.head());

      std::string rs = fastype::Random::nextAlphaNumeric(i);
      fastype::Block b3(rs);
      fastype::Block b4(b3);

      REQUIRE(b3.capacity() >= (int)rs.length());
      REQUIRE(b4.capacity() >= (int)rs.length());
      REQUIRE(b3.size() == (int)rs.length());
      REQUIRE(b3.size() == b4.size());
      REQUIRE(b3.empty() == rs.empty());
      REQUIRE(b3.empty() == b4.empty());
      REQUIRE(std::memcmp(b3.head(), rs.data(), b3.size()) == 0);
      REQUIRE(std::memcmp(b3.head(), b4.head(), b3.size()) == 0);
      REQUIRE(b3.tail() == b3.head() + b3.size());
      REQUIRE(b4.tail() == b4.head() + b4.size());
    }
  }

  SECTION("Copy Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      int r = fastype::Random::nextInt(TEST_MAX);
      fastype::Block b1(r);
      REQUIRE(b1.capacity() >= r);
      REQUIRE(b1.empty());
      REQUIRE(b1.size() == 0);
      REQUIRE(b1.tail() == b1.head());

      fastype::Block b2(b1);
      REQUIRE(b2.capacity() >= 0);
      REQUIRE(b2.empty());
      REQUIRE(b2.size() == 0);
      REQUIRE(b2.tail() == b2.head());

      std::string rs = fastype::Random::nextAlphaNumeric(i);
      fastype::Block b3(rs);
      fastype::Block b4(b3);

      REQUIRE(b3.capacity() >= (int)rs.length());
      REQUIRE(b4.capacity() >= (int)rs.length());
      REQUIRE(b3.size() == (int)rs.length());
      REQUIRE(b3.size() == b4.size());
      REQUIRE(b3.empty() == rs.empty());
      REQUIRE(b3.empty() == b4.empty());
      REQUIRE(std::memcmp(b3.head(), rs.data(), b3.size()) == 0);
      REQUIRE(std::memcmp(b3.head(), b4.head(), b3.size()) == 0);
      REQUIRE(b3.tail() == b3.head() + b3.size());
      REQUIRE(b4.tail() == b4.head() + b4.size());
    }
  }

  SECTION("String Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      char c = fastype::Random::nextAlphaNumericChar();
      fastype::Block b1(c);
      fastype::Block b2(b1);

      REQUIRE(b1.capacity() >= 1);
      REQUIRE(!b1.empty());
      REQUIRE(b1.size() == 1);
      REQUIRE(b1.tail() == b1.head() + 1);

      REQUIRE(b2.capacity() >= 1);
      REQUIRE(!b2.empty());
      REQUIRE(b2.size() == 1);
      REQUIRE(b2.tail() == b2.head() + 1);

      REQUIRE(std::memcmp(b1.head(), &c, 1) == 0);
      REQUIRE(std::memcmp(b2.head(), &c, 1) == 0);

      std::string s = fastype::Random::nextAlphaNumeric(i + 1);
      fastype::Block b3(s);
      fastype::Block b4(b3);

      REQUIRE(b3.capacity() >= s.length());
      REQUIRE(!b3.empty());
      REQUIRE(b3.size() == s.length());
      REQUIRE(b3.tail() == b3.head() + s.length());

      REQUIRE(b4.capacity() >= s.length());
      REQUIRE(!b4.empty());
      REQUIRE(b4.size() == s.length());
      REQUIRE(b4.tail() == b4.head() + s.length());

      REQUIRE(std::memcmp(b3.head(), s.data(), s.length()) == 0);
      REQUIRE(std::memcmp(b4.head(), s.data(), s.length()) == 0);
    }
  }

  SECTION("String Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      char c = fastype::Random::nextAlphaNumericChar();
      fastype::Block b1(c);
      fastype::Block b2(b1);
      fastype::Block b5 = b2;

      REQUIRE(b1.capacity() >= 1);
      REQUIRE(!b1.empty());
      REQUIRE(b1.size() == 1);
      REQUIRE(b1.tail() == b1.head() + 1);

      REQUIRE(b2.capacity() >= 1);
      REQUIRE(!b2.empty());
      REQUIRE(b2.size() == 1);
      REQUIRE(b2.tail() == b2.head() + 1);

      REQUIRE(b5.capacity() >= 1);
      REQUIRE(!b5.empty());
      REQUIRE(b5.size() == 1);
      REQUIRE(b5.tail() == b5.head() + 1);

      REQUIRE(std::memcmp(b1.head(), &c, 1) == 0);
      REQUIRE(std::memcmp(b2.head(), &c, 1) == 0);
      REQUIRE(std::memcmp(b5.head(), &c, 1) == 0);

      std::string s = fastype::Random::nextAlphaNumeric(i + 1);
      fastype::Block b3(s);
      fastype::Block b4(b3);
      fastype::Block b6 = b4;

      REQUIRE(b3.capacity() >= s.length());
      REQUIRE(!b3.empty());
      REQUIRE(b3.size() == s.length());
      REQUIRE(b3.tail() == b3.head() + s.length());

      REQUIRE(b4.capacity() >= s.length());
      REQUIRE(!b4.empty());
      REQUIRE(b4.size() == s.length());
      REQUIRE(b4.tail() == b4.head() + s.length());

      REQUIRE(b6.capacity() >= s.length());
      REQUIRE(!b6.empty());
      REQUIRE(b6.size() == s.length());
      REQUIRE(b6.tail() == b6.head() + s.length());

      REQUIRE(std::memcmp(b3.head(), s.data(), s.length()) == 0);
      REQUIRE(std::memcmp(b4.head(), s.data(), s.length()) == 0);
      REQUIRE(std::memcmp(b6.head(), s.data(), s.length()) == 0);
    }
  }

  SECTION("move") {
    for (int i = 0; i < TEST_MAX; i++) {
      {
        char s = fastype::Random::nextAlphaNumericChar();
        fastype::Block b1(s);

        REQUIRE(b1.capacity() >= 1);
        REQUIRE(!b1.empty());
        REQUIRE(b1.size() == 1);
        REQUIRE(b1.tail() == b1.head() + 1);
        REQUIRE(std::memcmp(b1.head(), &s, 1) == 0);

        fastype::Block b2(std::move(b1));
        REQUIRE(b1.capacity() == 0);
        REQUIRE(b1.empty());
        REQUIRE(b1.size() == 0);
        REQUIRE(b1.head() == nullptr);
        REQUIRE(b1.tail() == nullptr);

        REQUIRE(b2.capacity() >= 1);
        REQUIRE(!b2.empty());
        REQUIRE(b2.size() == 1);
        REQUIRE(b2.tail() == b2.head() + 1);
        REQUIRE(std::memcmp(b2.head(), &s, 1) == 0);

        fastype::Block b3 = std::move(b2);
        REQUIRE(b1.capacity() == 0);
        REQUIRE(b1.empty());
        REQUIRE(b1.size() == 0);
        REQUIRE(b1.head() == nullptr);
        REQUIRE(b1.tail() == nullptr);

        REQUIRE(b2.capacity() == 0);
        REQUIRE(b2.empty());
        REQUIRE(b2.size() == 0);
        REQUIRE(b2.head() == nullptr);
        REQUIRE(b2.tail() == nullptr);

        REQUIRE(b3.capacity() >= 1);
        REQUIRE(!b3.empty());
        REQUIRE(b3.size() == 1);
        REQUIRE(b3.tail() == b3.head() + 1);
        REQUIRE(std::memcmp(b3.head(), &s, 1) == 0);
      }
      {
        std::string s = fastype::Random::nextAlphaNumeric(i + 1);
        fastype::Block b1(s);

        REQUIRE(b1.capacity() >= s.length());
        REQUIRE(!b1.empty());
        REQUIRE(b1.size() == s.length());
        REQUIRE(b1.tail() == b1.head() + s.length());
        REQUIRE(std::memcmp(b1.head(), s.data(), s.length()) == 0);

        fastype::Block b2(std::move(b1));
        REQUIRE(b1.capacity() == 0);
        REQUIRE(b1.empty());
        REQUIRE(b1.size() == 0);
        REQUIRE(b1.head() == nullptr);
        REQUIRE(b1.tail() == nullptr);

        REQUIRE(b2.capacity() >= s.length());
        REQUIRE(!b2.empty());
        REQUIRE(b2.size() == s.length());
        REQUIRE(b2.tail() == b2.head() + s.length());
        REQUIRE(std::memcmp(b2.head(), s.data(), s.length()) == 0);

        fastype::Block b3 = std::move(b2);
        REQUIRE(b1.capacity() == 0);
        REQUIRE(b1.empty());
        REQUIRE(b1.size() == 0);
        REQUIRE(b1.head() == nullptr);
        REQUIRE(b1.tail() == nullptr);

        REQUIRE(b2.capacity() == 0);
        REQUIRE(b2.empty());
        REQUIRE(b2.size() == 0);
        REQUIRE(b2.head() == nullptr);
        REQUIRE(b2.tail() == nullptr);

        REQUIRE(b3.capacity() >= s.length());
        REQUIRE(!b3.empty());
        REQUIRE(b3.size() == s.length());
        REQUIRE(b3.tail() == b3.head() + s.length());
        REQUIRE(std::memcmp(b3.head(), s.data(), s.length()) == 0);
      }
    }
  }

  SECTION("swap") {
    for (int i = 0; i < TEST_MAX; i++) {
      int n = fastype::Random::nextInt(TEST_MAX);
      std::string s1 = fastype::Random::nextAlphaNumeric(n + 1);
      n = fastype::Random::nextInt(TEST_MAX);
      std::string s2 = fastype::Random::nextAlphaNumeric(n + 1);
      fastype::Block b1(s1);
      fastype::Block b2(s2);
      REQUIRE(std::memcmp(b1.head(), s1.data(), b1.size()) == 0);
      REQUIRE(std::memcmp(b2.head(), s2.data(), b2.size()) == 0);

      b1.swap(b2);
      REQUIRE(std::memcmp(b1.head(), s2.data(), b1.size()) == 0);
      REQUIRE(std::memcmp(b2.head(), s1.data(), b2.size()) == 0);
    }
  }

  SECTION("concat") {
    for (int i = 0; i < TEST_MAX; i++) {
      fastype::Block b;
      std::string s;
      for (int j = 0; j < TEST_MAX; j++) {
        std::string t = fastype::Random::nextAlphaNumeric(TEST_MAX);
        b.concat(t);
        s += t;
        REQUIRE(b.size() == s.length());
        REQUIRE(std::memcmp(b.head(), s.data(), b.size()) == 0);

        b += t;
        s += t;
        REQUIRE(b.size() == s.length());
        REQUIRE(std::memcmp(b.head(), s.data(), b.size()) == 0);
      }
    }
  }

  SECTION("concatHead") {
    for (int i = 0; i < TEST_MAX; i++) {
      fastype::Block b;
      std::string s;
      for (int j = 0; j < TEST_MAX; j++) {
        std::string t = fastype::Random::nextAlphaNumeric(TEST_MAX);
        b.concatHead(t);
        s = t + s;
        REQUIRE(b.size() == s.length());
        if (std::memcmp(b.head(), s.data(), b.size()) != 0) {
          F_ERRORF("i:{}, j:{}, b#head {}, s#data {}, b#size {}, s#length {}",
                   i, j, b.head(), s.data(), b.size(), s.length());
        }
        REQUIRE(std::memcmp(b.head(), s.data(), b.size()) == 0);
      }
    }
  }

  SECTION("truncate") {
    for (int i = 0; i < TEST_MAX; i++) {
      std::string s = fastype::Random::nextAlphaNumeric(TEST_MAX);
      fastype::Block b1(s), b2(s);
      int len = fastype::Random::nextInt(TEST_MAX);
      int start = fastype::Random::nextInt(TEST_MAX);
      start = std::min<int>(start, s.length() - len);
      b1.truncate(start);
      REQUIRE(b1.size() == s.length() - start);
      REQUIRE(std::memcmp(b1.head(), s.data() + start, b1.size()) == 0);
      b2.truncate(start, len);
      REQUIRE(b2.size() == len);
      REQUIRE(std::memcmp(b2.head(), s.data() + start, b2.size()) == 0);
    }
  }

  SECTION("removeHead") {
    for (int i = 0; i < TEST_MAX; i++) {
      std::string s = fastype::Random::nextAlphaNumeric(TEST_MAX);
      fastype::Block b1(s), b2(s);
      int len = std::min<int>(fastype::Random::nextInt(TEST_MAX), s.length());
      b1.removeHead(len);
      REQUIRE(b1.size() == s.length() - len);
      REQUIRE(std::memcmp(b1.head(), s.data() + len, b1.size()) == 0);
      int len2 = std::min<int>(len, s.length() - len);
      b1.removeHead(len2);
      if (b1.size() != (int)s.length() - len - len2) {
        F_ERRORF("i:{}, b1#size:{}, s#length:{}, len:{}, len2:{}", i, b1.size(),
                 s.length(), len, len2);
      }
      REQUIRE(b1.size() == (int)s.length() - len - len2);
      REQUIRE(std::memcmp(b1.head(), s.data() + len + len2, b1.size()) == 0);
    }
  }

  SECTION("removeTail") {
    for (int i = 0; i < TEST_MAX; i++) {
      std::string s = fastype::Random::nextAlphaNumeric(TEST_MAX);
      fastype::Block b1(s), b2(s);
      int len = std::min<int>(fastype::Random::nextInt(TEST_MAX), s.length());
      b1.removeTail(len);
      REQUIRE(b1.size() == s.length() - len);
      REQUIRE(std::memcmp(b1.head(), s.data(), b1.size()) == 0);
      int len2 = std::min<int>(len, s.length() - len);
      b1.removeTail(len2);
      if (b1.size() != (int)s.length() - len - len2) {
        F_ERRORF("i:{}, b1#size:{}, s#length:{}, len:{}, len2:{}", i, b1.size(),
                 s.length(), len, len2);
      }
      REQUIRE(b1.size() == (int)s.length() - len - len2);
      REQUIRE(std::memcmp(b1.head(), s.data(), b1.size()) == 0);
    }
  }

  SECTION("toString") {
    std::string s = fastype::Random::nextAlphaNumeric(TEST_MAX);
    fastype::Block b1, b2(s);
    F_INFOF("b1: {}", b1.toString());
    F_INFOF("b2: {}", b2.toString());
    REQUIRE(b1.toString().length() > 0);
  }

  SECTION("attribute") {
    for (int i = 0; i < TEST_MAX; i++) {
      std::string s = fastype::Random::nextAlphaNumeric(TEST_MAX);
      fastype::Block b1, b2(s);
      REQUIRE(b1.head() == nullptr);
      REQUIRE(b1.tail() == nullptr);
      REQUIRE(b2.head() != nullptr);
      REQUIRE(b2.tail() == b2.head() + b2.size());
      for (int j = 0; j < b2.size(); j++) {
        REQUIRE(b2[j] == s[j]);
        REQUIRE(b2.at(j) == s[j]);
      }
      REQUIRE(b1.empty());
      REQUIRE(b1.size() == 0);
      REQUIRE(b1.capacity() == 0);
      REQUIRE(b1.leftCapacity() == 0);
      REQUIRE(!b2.empty());
      REQUIRE(b2.size() == s.length());
      REQUIRE(b2.capacity() >= s.length());
      REQUIRE(b2.leftCapacity() == b2.capacity() - b2.size());
    }
  }

  SECTION("compare") {
    for (int i = 0; i < TEST_MAX; i++) {
      std::string s1 = fastype::Random::nextAlphaNumeric(TEST_MAX);
      std::string s2 = fastype::Random::nextAlphaNumeric(TEST_MAX);
      fastype::Block b1(s1), b2(s2);
      REQUIRE(s1.compare(s2) == b1.compare(b2));
      REQUIRE((s1 == s2) == (b1 == b2));
      REQUIRE((s1 != s2) == (b1 != b2));
      REQUIRE((s1 > s2) == (b1 > b2));
      REQUIRE((s1 >= s2) == (b1 >= b2));
      REQUIRE((s1 < s2) == (b1 < b2));
      REQUIRE((s1 <= s2) == (b1 <= b2));
    }
  }
}
