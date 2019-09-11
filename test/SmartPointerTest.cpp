// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "SmartPointer.h"
#include "catch2/catch.hpp"

TEST_CASE("SmartPointer", "[SmartPointer]") {

  SECTION("constructor and copy") {
    for (int i = 0; i < 1024; i++) {
      int *t = new int(i);
      fastype::sptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      fastype::sptr<int> p2(p1);
      fastype::sptr<int> p3(p1);
      fastype::sptr<int> p4(p3);
      REQUIRE(p1.useCount() == 4);
      REQUIRE(p2.useCount() == 4);
      REQUIRE(p3.useCount() == 4);
      REQUIRE(p4.useCount() == 4);
      REQUIRE(p1.get() == p2.get());
      REQUIRE(p2.get() == p3.get());
      REQUIRE(p3.get() == p4.get());
      REQUIRE(*p1 == i);
      REQUIRE(*p2 == i);
      REQUIRE(*p3 == i);
      REQUIRE(*p4 == i);
    }
  }

  SECTION("move") {
    for (int i = 0; i < 1024; i++) {
      int *t = new int(i);
      fastype::sptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      fastype::sptr<int> p2(std::move(p1));
      fastype::sptr<int> p3(std::move(p2));
      fastype::sptr<int> p4(std::move(p3));
      REQUIRE(p1.useCount() == 0);
      REQUIRE(p2.useCount() == 0);
      REQUIRE(p3.useCount() == 0);
      REQUIRE(p4.useCount() == 1);
      REQUIRE(p1.get() == nullptr);
      REQUIRE(p1.get() == p2.get());
      REQUIRE(p2.get() == p3.get());
      REQUIRE(p4.get() != nullptr);
      REQUIRE(*p1 == i);
      REQUIRE(*p2 == i);
      REQUIRE(*p3 == i);
      REQUIRE(*p4 == i);
    }
  }
}
