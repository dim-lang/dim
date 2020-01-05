// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "catch2/catch.hpp"
#include <cstdio>

TEST_CASE("Token", "[Token]") {

  SECTION("DslTest1.fast") {
    yyin = fopen("DslTest1.fast", "r");
    yylex();
    fclose(yyin);
  }
  SECTION("DslTest2.fast") {
    yyin = fopen("DslTest2.fast", "r");
    yylex();
    fclose(yyin);
  }
}
