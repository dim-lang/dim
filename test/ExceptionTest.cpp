// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Exception.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Exception", "[Exception]") {
  SECTION("exception") {
    try {
      CTHROW("throw: {}", "hello world");
    } catch (const std::string &ex) {
      LOG_ERROR("catch: {}", ex);
    }
    try {
      LOG_ASSERT(false, "assert false");
    } catch (const std::string &ex) {
      LOG_ERROR("catch assert: {}", ex);
    }
  }
}
