// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "exception/Exception.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Exception", "[Exception]") {
  SECTION("assert") {
    try {
      X_ASSERT(false, "assert message");
    } catch (const Exception &e) {
      LOG_INFO("assertion: {} {} {} {}", e.fileName(), e.lineNumber(),
               e.functionName(), e.message());
    }
  }
}
