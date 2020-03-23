// Copyright 2019- <collie.org>
// Apache License Version 2.0

#include "Exception.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Exception", "[Exception]") {
  SECTION("exception") {
    try {
      CTHROW("throw: {}", "hello world");
    } catch (const std::string &ex) {
      CERROR("catch: {}", ex);
    }
    try {
      CASSERT(false, "assert false");
    } catch (const std::string &ex) {
      CERROR("catch assert: {}", ex);
    }
  }
}
