// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "catch2/catch.hpp"

TEST_CASE("Line", "[Line]") {
  SECTION("No Args Constructor") {
    fastype::Line l;
    REQUIRE((void *)l.data() == nullptr);
    REQUIRE(l.size() == 0);
    REQUIRE(l.capacity() == 0);
    REQUIRE(l.dirty() == false);
    REQUIRE(l.lineNumber() == -1);
  }

  SECTION("Args Constructor") {
    fastype::Line l1(100, 3, true);
    REQUIRE((void *)l1.data() != nullptr);
    REQUIRE(l1.size() == 0);
    REQUIRE(l1.capacity() >= 100);
    REQUIRE(l1.dirty() == true);
    REQUIRE(l1.lineNumber() == 3);

    fastype::Line l2(490, 17, false);
    REQUIRE((void *)l2.data() != nullptr);
    REQUIRE(l2.size() == 0);
    REQUIRE(l2.capacity() >= 0);
    REQUIRE(l2.dirty() == false);
    REQUIRE(l2.lineNumber() == 17);
  }

  SECTION("Attribute") {
    fastype::Line l;
    l.expand(100);
    l.setLineNumber(13);
    REQUIRE((void *)l.data() == nullptr);
    REQUIRE(l.capacity() >= 100);
    REQUIRE(l.size() == 0);
    REQUIRE(l.lineNumber() == 13);
    REQUIRE(l.dirty() == false);
    for (int i = 0; i < 99; i++) {
      l[i] = 'A';
    }
    l[99] = '\0';
    l.setSize(100);
    l.setDirty(true);
    l.setLineNumber(69);
    REQUIRE((void *)l.data() != nullptr);
    REQUIRE(l.capacity() >= 100);
    REQUIRE(l.size() == 100);
    REQUIRE(l.lineNumber() == 69);
    REQUIRE(l.dirty() == false);
  }
}
