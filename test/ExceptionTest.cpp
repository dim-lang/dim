// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Exception.h"
#include "catch2/catch.hpp"

TEST_CASE("Exception", "[Exception]") {
  SECTION("exception") {
    try {
      CTHROW("throw exception with args: {}", "hello world");
    } catch (const std::string &ex) {
      CERROR("throw exception: {}", ex);
    }
  }
}
