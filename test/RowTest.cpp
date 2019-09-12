// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Row.h"
#include "Cowstr.h"
#include "catch2/catch.hpp"
#include <cstring>

#define HELLO_WORLD "hello world"
#define GOODBYE_WORLD "goodbye world"

TEST_CASE("Row", "[Row]") {

  SECTION("Args Constructor") {
    // fastype::Cowstr cs(100);
    // cs.concat(HELLO_WORLD, std::strlen(HELLO_WORLD));

    // fastype::Row r1(cs, 3, true);
    // REQUIRE((void *)r1.str().head() != nullptr);
    // REQUIRE(r1.str().size() == std::strlen(HELLO_WORLD));
    // REQUIRE(r1.str().capacity() >= 100);
    // REQUIRE(!r1.dirty());
    // REQUIRE(r1.lineNumber() == 3);

    // cs.concat(GOODBYE_WORLD, std::strlen(GOODBYE_WORLD));
    // fastype::Row r2(cs, 17, false);
    // REQUIRE((void *)r2.str().head() != nullptr);
    // REQUIRE(r2.str().size() == std::strlen(GOODBYE_WORLD));
    // REQUIRE(r2.str().capacity() >= 0);
    // REQUIRE(r2.dirty());
    // REQUIRE(r2.lineNumber() == 17);
  }
}
