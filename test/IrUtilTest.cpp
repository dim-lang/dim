// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrUtil.h"
#include "Ir.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

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
}
