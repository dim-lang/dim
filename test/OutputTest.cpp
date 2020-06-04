// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Output.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Semantic.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void testIr(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  IrContext context(fileName);
  IrTranslateUnit tunit(scanner.translateUnit());
  LOG_INFO("output string ir:{}\n\n{}", fileName,
           output_string(&tunit, &context));
  std::string llFileName = std::string(fileName) + std::string("ll");
  std::error_code errcode = output_fd(&tunit, &context, llFileName);
  LOG_INFO("output fd ir:{}, errcode value:{}, category:{}, message:{}",
           llFileName, errcode.value(), errcode.category().name(),
           errcode.message());
}

TEST_CASE("Output", "[Output]") {
  SECTION("output Ir") {
    testIr("test/case/IrTest1.shp");
    testIr("test/case/IrTest2.shp");
    testIr("test/case/IrTest3.shp");
  }
}
