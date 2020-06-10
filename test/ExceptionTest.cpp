// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Exception.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Exception", "[Exception]") {
  SECTION("assert") {
    try {
      EX_ASSERT(false, "Don't worry, just assert testing message.");
    } catch (const Exception &e) {
      LOG_INFO("assertion: {} {} {} {}", e.fileName(), e.lineNumber(),
               e.functionName(), e.message());
    }
  }
}
