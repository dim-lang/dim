// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Buffer.h"
#include "Ir.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void semantic(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("translateUnit: {}", scanner.translateUnit()->toString());
  IrContext context(fileName);
  IrTranslateUnit tunit(&context, scanner.translateUnit());
}

TEST_CASE("Semantic", "[Semantic]") {
  SECTION("symbol") {
    semantic("test/case/Parser1.shp");
    semantic("test/case/Parser2.shp");
  }
}
