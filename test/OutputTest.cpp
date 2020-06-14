// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Output.h"
#include "Buffer.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Semantic.h"
#include "catch2/catch.hpp"

static void testIr(const char *fileName) {
  {
    Scanner scanner;
    scanner.pushBuffer(fileName);
    REQUIRE(scanner.parse() == 0);
    IrContext context(fileName);
    IrTranslateUnit tunit(scanner.translateUnit());
    LOG_INFO("output string ir:{}\n\n{}", fileName,
             output_string(&tunit, &context));
  }
  {
    Scanner scanner;
    scanner.pushBuffer(fileName);
    REQUIRE(scanner.parse() == 0);
    IrContext context(fileName);
    IrTranslateUnit tunit(scanner.translateUnit());
    std::error_code errcode = output_fd(&tunit, &context, fileName);
    LOG_INFO("output fd ir:{}, errcode value:{}, category:{}, message:{}",
             fileName, errcode.value(), errcode.category().name(),
             errcode.message());
  }
}

TEST_CASE("Output", "[Output]") {
  SECTION("output Ir") {
    testIr("test/case/IrTest1.shp");
    testIr("test/case/IrTest2.shp");
    testIr("test/case/IrTest3.shp");
  }
}
