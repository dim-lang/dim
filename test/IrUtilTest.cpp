// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrUtil.h"
#include "Ir.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void testOutput(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  IrContext context(fileName);
  IrTranslateUnit tunit(&context, scanner.translateUnit());
  tunit.buildSymbol();
  tunit.codeGen();
  IrUtil::toStdout(&context);
  LOG_INFO("output string ir:{}\n\n{}", fileName,
           IrUtil::toStringOstream(&context));
  std::error_code errcode = IrUtil::toFileOstream(&context, fileName);
  LOG_INFO("output file ir:{}, errcode value:{}, category:{}, message:{}",
           fileName, errcode.value(), errcode.category().name(),
           errcode.message());
}

TEST_CASE("IrUtil", "[IrUtil]") {
  SECTION("IrUtil::namegen") {
    REQUIRE(IrUtil::namegen("a").substr(0, 9) == std::string("shp.ir.a."));
    REQUIRE(IrUtil::namegen("b").substr(0, 9) == std::string("shp.ir.b."));
  }
  SECTION("IrUtil::toLLVMName") {
    REQUIRE(IrUtil::toLLVMName("hello_world") ==
            std::string("shp.ir.hello.world"));
    REQUIRE(IrUtil::toLLVMName("HelloWorld") ==
            std::string("shp.ir.HelloWorld"));
  }
  SECTION("IrUtil::fromLLVMName") {
    REQUIRE(IrUtil::fromLLVMName("shp.ir.hello.world") ==
            std::string("hello_world"));
    REQUIRE(IrUtil::fromLLVMName("shp.ir.HelloWorld") ==
            std::string("HelloWorld"));
  }
  SECTION("Output") {
    testOutput("test/case/IrTest1.shp");
    testOutput("test/case/IrTest2.shp");
    testOutput("test/case/IrTest3.shp");
  }
}
